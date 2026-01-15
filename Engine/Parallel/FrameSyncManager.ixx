export module Aether:FrameSyncManager;
import <vector>;
import <memory>;
import <mutex>;
import <condition_variable>;


export namespace Aether
{
    class FrameSyncManager {
    public:
        struct FrameData {
            uint64_t frameId;
            std::atomic<uint32_t> dependencyCount{ 0 };
            std::atomic<uint32_t> completedJobs{ 0 };
            std::atomic<bool> readyForRender{ false };
        };

        FrameSyncManager(uint32_t maxFramesInFlight = 2);

        // 开始新帧
        uint64_t BeginFrame();

        // 结束当前帧
        void EndFrame();

        // 等待帧完成
        void WaitForFrame(uint64_t frameId);

        // 检查帧是否可渲染
        bool IsFrameReady(uint64_t frameId);

        // 获取当前帧数据
        FrameData* GetCurrentFrame();

    private:
        std::vector<std::unique_ptr<FrameData>> m_framePool;
        std::atomic<uint64_t> m_currentFrameId{ 0 };
        uint32_t m_maxFramesInFlight;

        // 环形缓冲索引
        std::atomic<uint32_t> m_writeIndex{ 0 };
        std::atomic<uint32_t> m_readIndex{ 0 };

        std::mutex m_mutex;
        std::condition_variable m_condVar;
    };

};