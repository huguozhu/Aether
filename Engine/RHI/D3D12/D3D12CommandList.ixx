export module Aether:D3DCommandList;
import :RHICommandList;
import :D3D12Definition;

namespace Aether
{
	class D3D12CommandList : public RHICommandList
	{
	public:
		D3D12CommandList(ID3D12Device* device, ECommandListType type);
		virtual ~D3D12CommandList() = default;




	private:
		ID3D12GraphicsCommandListPtr	m_pCommandList = nullptr;
		ID3D12CommandAllocatorPtr		m_pAllocator = nullptr;
		ECommandListState				m_eState;
		ECommandListType				m_eType;


		ID3D12PipelineState*			m_pCurrentPSO = nullptr;
		ID3D12RootSignature*			m_pCurrentRootSignature = nullptr;
		D3D12_VIEWPORT					m_Viewport;
		D3D12_RECT						m_ScissorRect;

		// ×ÊÔ´×·×Ù
		std::vector<ID3D12ResourcePtr> m_trackedResources;

	};


};