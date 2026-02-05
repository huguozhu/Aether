export module Aether:D3D12GpuMemoryAllocator;
import :Engine;
import :D3D12Definition;
import <mutex>;

export namespace Aether
{
	extern uint32_t ConstantDataAligment;
	extern uint32_t StructuredDataAligment;
	extern uint32_t TextureDataAligment;

	/******************************************************************************
	* D3D12GpuMemoryPage
	*******************************************************************************/
	class D3D12GpuMemoryPage
	{
	public:
		D3D12GpuMemoryPage(AetherEngine* engine, bool is_upload, uint32_t size_in_bytes);
		D3D12GpuMemoryPage(D3D12GpuMemoryPage&& rhs);
		~D3D12GpuMemoryPage();

		ID3D12Resource* GetResource() const { return m_pResource.Get(); }
		void* GetCpuAddress() const { return m_pCpuAddr; }
		D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress() const { return m_GpuAddr; }

		template <typename T>
		T* GetCpuAddress() const { return reinterpret_cast<T*>(this->GetCpuAddress()); }

	private:
		AetherEngine* m_pEngine = nullptr;
		bool m_bIsUpLoad;
		ID3D12ResourcePtr m_pResource = nullptr;
		void* m_pCpuAddr = nullptr;
		D3D12_GPU_VIRTUAL_ADDRESS m_GpuAddr;
	};


	/******************************************************************************
	* D3D12GpuMemoryBlock
	*******************************************************************************/
	class D3D12GpuMemoryBlock
	{
	public:
		D3D12GpuMemoryBlock();
		D3D12GpuMemoryBlock(D3D12GpuMemoryBlock&& rhs);
		D3D12GpuMemoryBlock& operator=(D3D12GpuMemoryBlock&& other);

		void Reset();
		void Reset(D3D12GpuMemoryPage const& page, uint32_t offset, uint32_t size);

		ID3D12Resource* GetResource() const { return m_pResource; }
		uint32_t GetOffset() const { return m_iOffset; }
		uint32_t GetSize() const { return m_iSize; }
		D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress() const { return m_GpuAddr; }

		void* GetCpuAddress() const { return m_pCpuAddr; }
		template <typename T>
		T* GetCpuAddress() const { return reinterpret_cast<T*>(this->GetCpuAddress()); }

	private:
		ID3D12Resource* m_pResource = nullptr;
		uint32_t m_iOffset = 0;
		uint32_t m_iSize = 0;
		void* m_pCpuAddr = nullptr;
		D3D12_GPU_VIRTUAL_ADDRESS m_GpuAddr;
	};

	/******************************************************************************
	* D3D12GpuMemoryAllocator
	*******************************************************************************/
	class D3D12GpuMemoryAllocator
	{
	public:
		D3D12GpuMemoryAllocator(AetherEngine* Engine, bool is_upload);
		D3D12GpuMemoryAllocator(D3D12GpuMemoryAllocator&& rhs);

		D3D12GpuMemoryBlock Allocate(uint32_t size_in_bytes, uint32_t alignment);
		void Deallocate(D3D12GpuMemoryBlock&& mem_block, uint64_t fence_value);
		void Renew(D3D12GpuMemoryBlock& mem_block, uint64_t fence_value, uint32_t size_in_bytes, uint32_t alignment);

		void ClearStallPages(uint64_t fence_value);
		void Clear();

	private:
		void Allocate(
			std::lock_guard<std::mutex>& proof_of_lock, D3D12GpuMemoryBlock& mem_block, uint32_t size_in_bytes, uint32_t alignment);
		void Deallocate(std::lock_guard<std::mutex>& proof_of_lock, D3D12GpuMemoryBlock& mem_block, uint64_t fence_value);

	private:
		AetherEngine* m_pEngine = nullptr;
		bool const m_bIsUpload;

		std::mutex m_AllocationMutex;

		struct PageInfo
		{
			D3D12GpuMemoryPage page;

			struct FreeRange
			{
				uint32_t first_offset;
				uint32_t last_offset;
			};
			std::vector<FreeRange> free_list;

			struct StallRange
			{
				FreeRange free_range;
				uint64_t fence_value;
			};
			std::vector<StallRange> stall_list;
		};
		std::vector<PageInfo> m_vPages;

		std::vector<D3D12GpuMemoryPage> m_vLargePages;
	};
	using D3D12GpuMemoryAllocatorPtr = std::shared_ptr<D3D12GpuMemoryAllocator>;
};
