export module Aether:D3D12Resource;
import :RHIResource;
import :D3D12Definition;
import :RHIStruct;
import :Engine;
import :EngineDefinition;
import :D3D12GpuMemoryAllocator;

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
		ID3D12Resource* D3DResource() const noexcept { return d3d_resource_.Get(); }
		uint32_t D3DResourceOffset() const noexcept { return d3d_resource_offset_; }

		void UpdateResourceBarrier(ID3D12GraphicsCommandList* cmd_list, uint32_t sub_res, D3D12_RESOURCE_STATES target_state);

	protected:
		AetherEngine* m_pEngine = nullptr;
		ID3D12ResourcePtr d3d_resource_;
		uint32_t d3d_resource_offset_;

		std::vector<D3D12_RESOURCE_STATES> curr_states_;
	};
	typedef std::shared_ptr<D3D12Resource> D3D12ResourcePtr;

	/******************************************************************************
	 * D3D12Buffer
	 *******************************************************************************/
	class D3D12Buffer: public RHIBuffer, public D3D12Resource
	{
	public:
		D3D12Buffer(AetherEngine* engine, size_t size, ResourceFlags flags)
			:RHIBuffer(size, flags), D3D12Resource(engine)
		{
			m_vCurrStates.resize(1, D3D12_RESOURCE_STATE_GENERIC_READ);
		}
		virtual AResult Create(uint32_t dataSize, const void* data) override;

		D3D12_GPU_VIRTUAL_ADDRESS GpuVirtualAddress() const { return m_GpuVAddr; }

	public:
		void* GetNativeResource() override { return m_pD3dResource.Get(); }
		void TransitionBarrier(RHICommandList* cmdList, EResourceState from, EResourceState to) override {}

	protected:		
		ID3D12ResourcePtr	m_pD3dResource = nullptr;
		uint32_t			m_iD3dResourceOffset;
		std::vector<D3D12_RESOURCE_STATES> m_vCurrStates;

		D3D12GpuMemoryBlock m_GpuMemoryBlock;
		uint32_t m_iCounterOffset = 0;
		D3D12_GPU_VIRTUAL_ADDRESS m_GpuVAddr;
	};

	/******************************************************************************
	 * D3D12VertexBuffer
	 ******************************************************************************/
	class D3D12VertexBuffer : public D3D12Buffer
	{
	public:
		D3D12VertexBuffer(AetherEngine* engine, size_t size)
			:D3D12Buffer(engine, size, RESOURCE_FLAG_CPU_WRITE | RESOURCE_FLAG_GPU_READ)
		{
		}

	};
	using D3D12VertexBufferPtr = std::shared_ptr<D3D12VertexBuffer>;


	/******************************************************************************
	 * D3D12IndexBuffer
	 *******************************************************************************/
	class D3D12IndexBuffer : public D3D12Buffer
	{
	public:
		D3D12IndexBuffer(AetherEngine* engine, size_t size)
			:D3D12Buffer(engine, size, RESOURCE_FLAG_CPU_WRITE | RESOURCE_FLAG_GPU_READ)
		{
		}

	};
	using D3D12IndexBufferPtr = std::shared_ptr<D3D12IndexBuffer>;



	/******************************************************************************
	 * D3D12ConstantBuffer
	 *******************************************************************************/
	class D3D12ConstantBuffer : public D3D12Buffer
	{
	public:
		D3D12ConstantBuffer(AetherEngine* engine, uint32_t size, ResourceFlags flags)
			: D3D12Buffer(engine, size, flags)
		{
		}
	};
	using D3D12ConstantBufferPtr = std::shared_ptr<D3D12ConstantBuffer>;

};