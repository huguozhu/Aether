module Aether:Job;

import :Job;

namespace Aether
{
    JobSystem::JobSystem(uint32_t threadCount) 
    {
        if (threadCount == 0) {
            threadCount = std::thread::hardware_concurrency() - 1;
        }

        //std::cout << "Initializing JobSystem with " << threadCount << " worker threads\n";

        for (uint32_t i = 0; i < threadCount; ++i) {
            m_workers.emplace_back(&JobSystem::WorkerThread, this, i);
        }
    }

    JobSystem::~JobSystem() {
        m_stop = true;
        m_queueCondVar.notify_all();

        for (auto& worker : m_workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    AResult JobSystem::Initialize()
    {

        return A_Success;
    }

    uint64_t JobSystem::Submit(Job&& job)
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);

        job.id = m_nextJobId.fetch_add(1, std::memory_order_relaxed);
        m_jobQueue.push(std::move(job));

        m_jobsInFlight.fetch_add(1, std::memory_order_relaxed);
        m_queueCondVar.notify_one();

        return job.id;
    }

    void JobSystem::WorkerThread(uint32_t threadIndex)
    {
        while (!m_stop) 
        {
            Job job;
            if (GetJob(job)) {
                // 执行任务
                job.task();

                // 递减计数器（如果有）
                if (job.counter) {
                    job.counter->fetch_sub(job.counter_decrement,
                        std::memory_order_release);
                }

                // 更新飞行中的任务计数
                m_jobsInFlight.fetch_sub(1, std::memory_order_relaxed);

                // 检查是否需要进行帧同步
                if (m_jobsInFlight.load(std::memory_order_acquire) == 0) {
                    m_frameSyncCondVar.notify_all();
                }
            }
        }
    }

    void JobSystem::FrameSync() 
    {
        std::unique_lock<std::mutex> lock(m_frameSyncMutex);

        // 等待所有进行中的任务完成
        m_frameSyncCondVar.wait(lock, [this]() {
            return m_jobsInFlight.load(std::memory_order_acquire) == 0;
            });

        // 递增帧索引
        m_frameIndex.fetch_add(1, std::memory_order_release);

        //std::cout << "Frame sync completed for frame " << m_frameIndex.load() << std::endl;
    }

    template<typename Func>
    void JobSystem::ParallelFor(int32_t start, int32_t end, Func&& func, int32_t batchSize) 
    {
        const int32_t total = end - start;
        const int32_t numBatches = (total + batchSize - 1) / batchSize;

        std::atomic<int32_t> counter{ numBatches };

        for (int32_t i = 0; i < numBatches; ++i) {
            Submit(Job{
                .task = [=, &func]() {
                    const int32_t batchStart = start + i * batchSize;
                    const int32_t batchEnd = std::min(batchStart + batchSize, end);
                    for (int32_t j = batchStart; j < batchEnd; ++j) {
                        func(j);
                    }
                },
                .counter = &counter,
                .counter_decrement = 1
                });
        }

        // 等待所有批次完成
        while (counter.load(std::memory_order_acquire) > 0) {
            std::this_thread::yield();
        }
    }


};