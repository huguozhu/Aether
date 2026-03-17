export module Aether:RHICommandList;
import :Engine;
import :EngineDefinition;
import :RHIStruct;
import :RHIViewport;
import :Rect;
import <string>; 

export namespace Aether
{
    class RHIPipelineState;
    class RHIBuffer;
    class RHIDescriptorSet;
    class RHIFence;

    // 命令列表基类
    class RHICommandList
    {
    public:
        RHICommandList(AetherEngine* engine)
            :m_pEngine(engine)
        {
        }
        virtual ~RHICommandList() = default;

        virtual AResult Render(RHIMeshPtr const& mesh) { return A_Success; }

        // 生命周期管理
        virtual void Begin() {}
        virtual void End() {}
        virtual void Reset() {}
        virtual void SetRootSignature(RHIRootSignaturePtr sig) { m_pRootSignature = sig; }       
        virtual void SetFrameBuffer(RHIFrameBuffer* pFB) { m_pCurFrameBuffer = pFB; }
       
        // 绘制命令
        //virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1) {}
        //virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) {}

        //// 网格着色器绘制命令
        //virtual void DispatchMesh(uint32_t x, uint32_t y, uint32_t z) {}

        //// 计算命令
        //virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {}

        //// 光线追踪命令
        //virtual void TraceRays(void* rtPSO, uint32_t width, uint32_t height) {}

        // 查询状态
        virtual ECommandListState GetState() const { return m_eState; }
        virtual ECommandListType GetType() const { return m_eType; }
        

    protected:
        AetherEngine*       m_pEngine = nullptr;
        RHIRootSignaturePtr m_pRootSignature = nullptr;
        EMeshTopologyType   m_eTopologyType;
        ECommandListState   m_eState;
        ECommandListType    m_eType;

        RHIFrameBuffer*     m_pCurFrameBuffer = nullptr;
    };

    // 命令分配器
    class RHICommandAllocator 
    {
    public:
        virtual ~RHICommandAllocator() = default;
        virtual void Reset() = 0;
    };

    // 命令队列
    class RHICommandQueue
    {
    public:
        virtual ~RHICommandQueue() = default;
        virtual void ExecuteCommandLists(uint32_t count, RHICommandList** lists) = 0;
        virtual void Signal(RHIFence* fence, uint64_t value) = 0;
        virtual void Wait(RHIFence* fence, uint64_t value) = 0;
    };

};