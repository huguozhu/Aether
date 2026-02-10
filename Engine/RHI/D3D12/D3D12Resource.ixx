export module Aether:D3D12Resource;
import :RHIResource;
import :D3D12Definition;
import :RHIStruct;
import :Engine;
import :EngineDefinition;

export namespace Aether
{
	/******************************************************************************
	 * D3D12Resource
	 *******************************************************************************/
	class D3D12Resource : public RHIResource
	{
	public:
		D3D12Resource(AetherEngine* engine);
		~D3D12Resource() = default;
		ID3D12Resource* D3DResource() const noexcept { return m_pD3dResource.Get(); }
		uint32_t D3DResourceOffset() const noexcept { return m_iD3dResourceOffset; }

		void UpdateResourceBarrier(ID3D12GraphicsCommandList* cmd_list, uint32_t sub_res, D3D12_RESOURCE_STATES target_state);
		void* GetNativeResource() override { return (void*)m_pD3dResource.Get(); }
		void TransitionBarrier(RHICommandList* cmdList, EResourceState from, EResourceState to) override;

	protected:
		AetherEngine*	m_pEngine = nullptr;
		ID3D12ResourcePtr	m_pD3dResource;
		uint32_t			m_iD3dResourceOffset;

		std::vector<D3D12_RESOURCE_STATES> m_vCurrStates;
	};
	typedef std::shared_ptr<D3D12Resource> D3D12ResourcePtr;

};