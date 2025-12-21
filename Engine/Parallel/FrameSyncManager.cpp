module Aether.Parallel:FrameSyncManager;

import :FrameSyncManager;
import <condition_variable>;

namespace Aether
{
    uint64_t FrameSyncManager::BeginFrame() 
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // 检查是否有可用的帧槽
        uint32_t nextWrite = (m_writeIndex.load() + 1) % m_maxFramesInFlight;
        if (nextWrite == m_readIndex.load()) {
            // 等待帧完成
            /*m_condVar.wait(m_mutex, [this, nextWrite]() {
                return nextWrite != m_readIndex.load();
                });*/
            m_condVar.wait(m_mutex, [=] { return true; });
        }

        // 获取帧数据
        uint32_t frameIndex = m_writeIndex.load();
        auto& frame = m_framePool[frameIndex];

        // 重置帧数据
        frame->frameId = ++m_currentFrameId;
        frame->dependencyCount.store(0);
        frame->completedJobs.store(0);
		frame->readyForRender.store(false);

        // 更新写索引
        m_writeIndex.store((frameIndex + 1) % m_maxFramesInFlight);

        return frame->frameId;
    }

    void FrameSyncManager::EndFrame() 
    {
        // 更新读索引，释放帧槽
        m_readIndex.store((m_readIndex.load() + 1) % m_maxFramesInFlight);
        m_condVar.notify_all();
    }


};
