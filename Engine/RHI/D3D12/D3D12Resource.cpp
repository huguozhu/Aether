module;
#include "d3d12.h"
module Aether:D3D12Resource;
import :D3D12Resource;
import :RHIResource;
import :D3D12Definition;
import :D3D12Context;
import :RHIStruct;
import :Engine;
import :EngineDefinition;

namespace Aether
{
	/******************************************************************************
	 * D3D12Resource
	 *******************************************************************************/
	D3D12Resource::D3D12Resource(AetherEngine* engine)
		:m_pEngine(engine)
	{
	}

	/*****************************************************************************
	 * D3D12Buffer
	 ******************************************************************************/
	AResult D3D12Buffer::Create(uint32_t dataSize, const void* data)
	{
		D3D12Context& rc = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance());
		ID3D12Device* pDevice = rc.GetD3D12Device();

		uint32_t total_size = m_iSize;
		if ((m_Flags & RESOURCE_FLAG_GPU_WRITE) && !((m_Flags & RESOURCE_FLAG_GPU_STRUCTURED) || (m_Flags & RESOURCE_FLAG_UAV)))
		{
			total_size = ((m_iSize + sizeof(uint64_t) - 1) & ~(sizeof(uint64_t) - 1)) + sizeof(uint64_t);
		}
		else if ((m_Flags & RESOURCE_FLAG_UAV) && (m_iStructureStride != 0)
			&& ((m_Flags & RESOURCE_FLAG_APPEND) || (m_Flags & RESOURCE_FLAG_COUNTER)))
		{
			total_size = ((m_iSize + D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1) & ~(D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1))
				+ sizeof(uint64_t);
		}

		if ((0 == m_Flags) || (RESOURCE_FLAG_CPU_WRITE == m_Flags) || ((RESOURCE_FLAG_CPU_WRITE | RESOURCE_FLAG_GPU_READ) == m_Flags))
		{
			m_GpuMemoryBlock = rc.AllocUploadMemBlock(m_iSize, ConstantDataAligment);
			m_pD3dResource = m_GpuMemoryBlock.GetResource();
			m_iD3dResourceOffset = m_GpuMemoryBlock.GetOffset();
			m_GpuVAddr = m_GpuMemoryBlock.GetGpuAddress();

			if (data != nullptr)
			{
				memcpy(m_GpuMemoryBlock.GetCpuAddress(), data, dataSize);
			}
			m_vCurrStates[0] = D3D12_RESOURCE_STATE_GENERIC_READ;
		}
		else
		{
			m_GpuMemoryBlock.Reset();

			D3D12_RESOURCE_STATES init_state;
			D3D12_HEAP_PROPERTIES heap_prop;
			if (RESOURCE_FLAG_CPU_READ == m_Flags)
			{
				init_state = D3D12_RESOURCE_STATE_COPY_DEST;
				heap_prop.Type = D3D12_HEAP_TYPE_READBACK;
			}
			else if ((0 == m_Flags) || (m_Flags & RESOURCE_FLAG_CPU_READ) || (m_Flags & RESOURCE_FLAG_CPU_WRITE))
			{
				init_state = D3D12_RESOURCE_STATE_GENERIC_READ;
				heap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
			}
			else
			{
				init_state = D3D12_RESOURCE_STATE_GENERIC_READ;
				heap_prop.Type = D3D12_HEAP_TYPE_DEFAULT;
			}
			heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heap_prop.CreationNodeMask = 0;
			heap_prop.VisibleNodeMask = 0;

			D3D12_RESOURCE_DESC res_desc;
			res_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			res_desc.Alignment = 0;
			res_desc.Width = total_size;
			res_desc.Height = 1;
			res_desc.DepthOrArraySize = 1;
			res_desc.MipLevels = 1;
			res_desc.Format = DXGI_FORMAT_UNKNOWN;
			res_desc.SampleDesc.Count = 1;
			res_desc.SampleDesc.Quality = 0;
			res_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			res_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
			if (m_Flags & RESOURCE_FLAG_UAV)
			{
				res_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
			}

			ThrowIfFailed(rc.GetD3D12Device()->CreateCommittedResource(
				&heap_prop, D3D12_HEAP_FLAG_NONE, &res_desc, init_state, nullptr, IID_PPV_ARGS(m_pD3dResource.ReleaseAndGetAddressOf())));

			m_iD3dResourceOffset = 0;
			m_GpuVAddr = m_pD3dResource->GetGPUVirtualAddress();

			m_vCurrStates[0] = init_state;

			if (data != nullptr)
			{
				auto upload_mem_block = rc.AllocUploadMemBlock(m_iSize, StructuredDataAligment);
				memcpy(upload_mem_block.GetCpuAddress(), data, m_iSize);

				{
					////rc.ResetLoadCmd();
					//ID3D12GraphicsCommandList* cmd_list = rc.D3DLoadCmdList();

					//this->UpdateResourceBarrier(cmd_list, 0, D3D12_RESOURCE_STATE_COPY_DEST);
					//rc.FlushResourceBarriers(cmd_list);

					//cmd_list->CopyBufferRegion(
					//	m_pD3dResource.Get(), m_iD3dResourceOffset, upload_mem_block.GetResource(), upload_mem_block.GetOffset(), m_iSize);

					//m_vCurrStates[0] = init_state;

					//rc.CommitLoadCmd();
				}

				rc.DeallocUploadMemBlock(std::move(upload_mem_block));
			}
		}

		if ((m_Flags & RESOURCE_FLAG_GPU_WRITE)
			&& !((m_Flags & RESOURCE_FLAG_GPU_STRUCTURED) || (m_Flags & RESOURCE_FLAG_UAV)))
		{
			m_iCounterOffset = (m_iSize + sizeof(uint64_t) - 1) & ~(sizeof(uint64_t) - 1);
		}
		else if ((m_Flags & RESOURCE_FLAG_UAV) && (m_iStructureStride != 0))
		{
			if ((m_Flags & RESOURCE_FLAG_APPEND) || (m_Flags & RESOURCE_FLAG_COUNTER))
			{
				m_iCounterOffset = (m_iSize + D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1)
					& ~(D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1);
			}
			else
			{
				m_iCounterOffset = 0;
			}
		}
		return A_Success;
	}







};