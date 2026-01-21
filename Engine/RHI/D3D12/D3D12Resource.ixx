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
	class D3D12Buffer : public RHIBuffer
	{
	public:
		D3D12Buffer(AetherEngine* engine)
			:m_pEngine(engine)
		{
		}

		ERHIResourceType GetType() const override { return ERHIResourceType::Buffer; }

		void* Map(size_t offset, size_t size) { return nullptr; }
		void Unmap() {}

		void* GetNativeResource() override { return m_pD3dResource.Get(); }
		void TransitionBarrier(RHICommandList* cmdList, EResourceState from, EResourceState to) override {}
	protected:
		AetherEngine*			m_pEngine = nullptr;
		ID3D12ResourcePtr		m_pD3dResource = nullptr;
	};

	/*****************************************************************************
	 D3D12VertexBuffer
	******************************************************************************/
	class D3D12VertexBuffer : public D3D12Buffer
	{
	public:
		D3D12VertexBuffer(AetherEngine* engine, uint32_t size)
			:D3D12Buffer(engine)
		{
		}

	protected:
	};
	using D3D12VertexBufferPtr = std::shared_ptr<D3D12VertexBuffer>;


	/******************************************************************************
	 * D3D12IndexBuffer
	 *******************************************************************************/
	class D3D12IndexBuffer : public D3D12Buffer
	{
	public:
		D3D12IndexBuffer(AetherEngine* engine, uint32_t size)
			:D3D12Buffer(engine)
		{
		}

	protected:
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