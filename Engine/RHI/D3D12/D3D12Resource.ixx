export module Aether:D3D12Resource;
import :RHIResource;
import :D3D12Definition;
import :RHIStruct;
import :Engine;
import :EngineDefinition;

export namespace Aether
{
	/******************************************************************************
	 * D3D12Buffer
	 *******************************************************************************/
	class D3D12Buffer: public RHIBuffer
	{
	public:
		D3D12Buffer(AetherEngine* engine, size_t size, ResourceFlags flags)
			:RHIBuffer(engine, size, flags)
		{
		}
		virtual AResult Create(uint32_t dataSize, const void* data) override;

	public:
		void* GetNativeResource() override { return m_pD3dResource.Get(); }
		void TransitionBarrier(RHICommandList* cmdList, EResourceState from, EResourceState to) override {}

	protected:
		ID3D12ResourcePtr	m_pD3dResource = nullptr;
		uint32_t			m_iD3dResourceOffset;
		std::vector<D3D12_RESOURCE_STATES> m_vCurrStates;

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
		D3D12ConstantBuffer(AetherEngine* engine, uint32_t size, ResourceFlags flags);
	};
	using D3D12ConstantBufferPtr = std::shared_ptr<D3D12ConstantBuffer>;

};