export module Aether:D3D12Buffer;
import :D3D12Resource;
import :D3D12Definition;
import :RHIStruct;
import :Engine;
import :EngineDefinition;
import :D3D12GpuMemoryAllocator;
import :D3D12RenderView;

export namespace Aether
{
	/******************************************************************************
	 * D3D12Buffer
	 *******************************************************************************/
	class D3D12Buffer : public RHIBuffer, public D3D12Resource
	{
	public:
		D3D12Buffer(AetherEngine* engine, size_t size, ResourceFlags flags)
			:RHIBuffer(size, flags), D3D12Resource(engine)
		{
			m_vCurrStates.resize(1, D3D12_RESOURCE_STATE_GENERIC_READ);
		}

		D3D12SrvPtr const& GetD3DSrv(PixelFormat format, uint32_t first_elem, uint32_t num_elems);
		D3D12RtvPtr const& GetD3DRtv(PixelFormat format, uint32_t first_elem, uint32_t num_elems);
		D3D12UavPtr const& GetD3DUav(PixelFormat format, uint32_t first_elem, uint32_t num_elems);

		virtual AResult Create(uint32_t dataSize, const void* data) override;

		D3D12_GPU_VIRTUAL_ADDRESS GpuVirtualAddress() const { return m_GpuVAddr; }

	public:
		void* GetNativeResource() override { return m_pD3dResource.Get(); }
		void TransitionBarrier(RHICommandList* cmdList, EResourceState from, EResourceState to) override {}

	protected:
		D3D12GpuMemoryBlock m_GpuMemoryBlock;
		D3D12_GPU_VIRTUAL_ADDRESS m_GpuVAddr;
		uint32_t m_iCounterOffset;

		std::unordered_map<size_t, D3D12SrvPtr> m_vD3dSrvs;
		std::unordered_map<size_t, D3D12RtvPtr> m_vD3dRtvs;
		std::unordered_map<size_t, D3D12UavPtr> m_vD3dUavs;
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