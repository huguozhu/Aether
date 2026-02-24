module Aether:D3D12Mesh;
import :D3D12Mesh;
import :RHIResource;
import :RHIStruct;
import :D3D12CommandList;
import :D3D12Definition;
import :D3D12Resource;
import :D3D12Translate;
import :D3D12Buffer;

namespace Aether
{

	AResult D3D12Mesh::Active(D3D12CommandList* pCmd)
	{
		if (m_bDataDirty)
		{
			this->UpdateStates();
			m_bDataDirty = true;
		}
		ID3D12GraphicsCommandList* cmd_list = pCmd->GetD3dCommandList();
		uint32_t const num_vertex_streams = this->NumVertexStream();
		if (num_vertex_streams > 0)
		{
			cmd_list->IASetVertexBuffers(0, num_vertex_streams, m_vVertexBufferViews.data());
		}
		if (this->IsUseIndices())
		{
			cmd_list->IASetIndexBuffer(&m_IndexBufferView);
		}
		D3D_PRIMITIVE_TOPOLOGY d3d_topo_type = D3D12Translate::TranslatePrimitiveTopology(m_eTopoType);
		cmd_list->IASetPrimitiveTopology(d3d_topo_type);

		
		if (this->IsUseIndices())
		{
			uint32_t index_count = this->GetNumIndices();
			cmd_list->DrawIndexedInstanced(index_count, 1, 0, 0, 0);
		}
		else
		{
			uint32_t vertex_count = this->GetNumVertex();
			cmd_list->DrawInstanced(vertex_count, 1, 0, 0);
		}

		return A_Success;
	}

	void D3D12Mesh::UpdateStates()
	{
		uint32_t const num_vertex_streams = this->NumVertexStream();

		m_vVertexBufferViews.resize(num_vertex_streams);
		for (uint32_t i = 0; i < num_vertex_streams; ++i)
		{
			VertexStream& vs = this->GetVertexStreamByIndex(i);
			D3D12Buffer* d3dvb = (D3D12Buffer*)(vs.render_buffer.get());
			m_vVertexBufferViews[i].BufferLocation = d3dvb->GpuVirtualAddress();
			m_vVertexBufferViews[i].SizeInBytes = d3dvb->GetSize();
			m_vVertexBufferViews[i].StrideInBytes = vs.stride;
		}

		if (this->IsUseIndices())
		{
			D3D12Buffer* ib = (D3D12Buffer*)(this->GetIndexBuffer().get());
			m_IndexBufferView.BufferLocation = ib->GpuVirtualAddress();
			m_IndexBufferView.SizeInBytes = ib->GetSize();
			m_IndexBufferView.Format = m_eIndexBufferType == EIndexBufferType::UInt16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		}
	}

};