export module Aether:D3D12CommandList;
import :RHICommandList;
import :D3D12Definition;
import :EngineDefinition;
import std;

export namespace Aether
{
	class D3D12CommandList : public RHICommandList
	{
	public:
		D3D12CommandList(AetherEngine* engine, ECommandListType type);
		virtual ~D3D12CommandList() = default;

		virtual AResult Render(RHIMeshPtr const& mesh) override;
		// 生命周期管理
		void Begin() override;
		void End() override;
		void Reset() override;

		void SetPipelineState(RHIPipelineState* pipeline) override;
		void SetVertexBuffers(uint32_t startSlot, uint32_t count, RHIBuffer** buffers) override;
		void SetIndexBuffer(RHIBuffer* buffer) override;
		void SetDescriptorSets(uint32_t setCount, RHIDescriptorSet** sets) override;

		void Draw(uint32_t vertexCount, uint32_t instanceCount = 1) override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;

	private:
		ID3D12GraphicsCommandListPtr	m_pCommandList = nullptr;
		ID3D12CommandAllocatorPtr		m_pCommandAllocator = nullptr;
		

		ID3D12PipelineState*			m_pCurrentPSO = nullptr;
		ID3D12RootSignature*			m_pCurrentRootSignature = nullptr;
		D3D12_VIEWPORT					m_Viewport;
		D3D12_RECT						m_ScissorRect;

		// 资源追踪
		std::vector<ID3D12ResourcePtr> m_vTrackedResources;
	};

	class D3D12CommandAllocator : public RHICommandAllocator
	{
	public:
		D3D12CommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
		~D3D12CommandAllocator();

		void Reset() override;

		ID3D12CommandAllocator* GetNativeAllocator() const { return m_pAllocator.Get(); }
		bool IsInUse() const { return m_bIsInUse; }
		void SetInUse(bool inUse) { m_bIsInUse = inUse; }

	private:
		ID3D12CommandAllocatorPtr		m_pAllocator;
		D3D12_COMMAND_LIST_TYPE			m_eType;
		std::atomic<bool>				m_bIsInUse;
	};

	class D3D12CommandQueue : public RHICommandQueue
	{
	public:


	private:
		ID3D12CommandQueuePtr		m_pCommandQueue;
		RHIFencePtr					m_pFence;
		D3D12_COMMAND_LIST_TYPE		m_pType;

	};


};