module;
#include <windows.h>
#include "Utils/Macros.h"
module Aether:D3D12CommandList;
import :D3D12CommandList;
import :EngineDefinition;
import :D3D12Translate;
import :D3D12Context;
import :Engine;
import :D3D12Definition;
import :D3D12FrameBuffer;
import :D3D12Mesh;
import :Error;
import :Utils;

namespace Aether
{
	D3D12CommandList::D3D12CommandList(AetherEngine* engine, ECommandListType type)
		:RHICommandList(engine), m_pCurrentPSO(nullptr), m_pCurrentRootSignature(nullptr)
	{
		m_eType = type;
		m_eState = ECommandListState::Invalid;
		D3D12_COMMAND_LIST_TYPE cl_type = D3D12Translate::TranslateCommandListType(type);

		D3D12Context* pRHIContext = (D3D12Context*)(&m_pEngine->RHIContextInstance());
		ID3D12DevicePtr pDevice = pRHIContext->GetD3D12Device();
		ThrowIfFailed(pDevice->CreateCommandAllocator(cl_type, IID_PPV_ARGS(&m_pCommandAllocator)));
		ThrowIfFailed(pDevice->CreateCommandList(0, cl_type, m_pCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_pGraphicsCommandList)));
		m_pGraphicsCommandList->Close();
		m_eState = ECommandListState::Invalid;

	}
	AResult D3D12CommandList::Render(RHIMeshPtr const& mesh)
	{
		if (!m_pCurFrameBuffer)
			return ERR_INVALID_ARG;

		this->Begin();
		D3D12FrameBuffer* pFrameBuffer = (D3D12FrameBuffer*)m_pCurFrameBuffer;
		pFrameBuffer->Active(this);
		D3D12Mesh* pMesh = (D3D12Mesh*)mesh.get();
		pMesh->Active(this);
		this->End();
		return A_Success;
	}
	void D3D12CommandList::Begin()
	{
		if (m_eState != ECommandListState::Invalid)
		{
			LOG_ERROR("Command list must be in invalid state to begin.");
			AETHER_ASSERT(false);
		}

		ThrowIfFailed(m_pCommandAllocator->Reset());
		ThrowIfFailed(m_pGraphicsCommandList->Reset(m_pCommandAllocator.Get(), nullptr));
		m_eState = ECommandListState::Recording;

		m_pCurrentPSO = nullptr;
		m_pCurrentRootSignature = nullptr;

	}
	void D3D12CommandList::End()
	{
		if (m_eState != ECommandListState::Recording) 
		{
			LOG_ERROR("Command list must be in recording state to end");
			AETHER_ASSERT(false);
		}
		ThrowIfFailed(m_pGraphicsCommandList->Close());
		m_eState = ECommandListState::Executable;
	}
	void D3D12CommandList::Reset()
	{
		if (m_eState == ECommandListState::Recording)
		{
			LOG_ERROR("Cannot reset command list while recording");
		}

		// 释放追踪的资源引用
		m_vTrackedResources.clear();
		m_eState = ECommandListState::Invalid;

		if (m_eType == ECommandListType::Graphics)
			ThrowIfFailed(m_pGraphicsCommandList->Reset(m_pCommandAllocator.Get(), nullptr));
	}

	ID3D12GraphicsCommandList* D3D12CommandList::GetD3dCommandList()
	{
		if (m_eType == ECommandListType::Graphics)
			return m_pGraphicsCommandList.Get();
		return nullptr;
	}

};
