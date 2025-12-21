export module Aether.Parallel:Job;

import <thread>;
import <vector>;
import <mutex>;

import <atomic>;
import <vector>;
import <queue>;
import <functional>;
import <condition_variable>;
import <future>;

export namespace Aether
{
    class JobSystem 
    {
    public:
        struct Job 
        {
            uint64_t id;
            std::function<void()> task;
            std::atomic<int32_t>* counter = nullptr;
            int32_t counter_decrement = 1;
        };

        explicit JobSystem(uint32_t threadCount = 0);
        ~JobSystem();

        // 提交Job
        uint64_t Submit(Job&& job);

        // 并行for循环
        template<typename Func>
        void ParallelFor(int32_t start, int32_t end, Func&& func, int32_t batchSize = 1);

        // 等待所有Job完成
        void Wait();

        // 帧同步点
        void FrameSync();

    private:
        void WorkerThread(uint32_t threadIndex);
        bool GetJob(Job& outJob);

        std::vector<std::thread> m_workers;
        std::queue<Job> m_jobQueue;
        std::mutex m_queueMutex;
        std::condition_variable m_queueCondVar;

        std::atomic<bool> m_stop{ false };
        std::atomic<uint64_t> m_nextJobId{ 1 };

        // 帧同步相关
        std::atomic<uint32_t> m_frameIndex{ 0 };
        std::atomic<uint32_t> m_jobsInFlight{ 0 };
        std::condition_variable m_frameSyncCondVar;
        std::mutex m_frameSyncMutex;
    };


    class RenderJobDependencies 
    {
    public:
        struct JobNode {
            uint64_t jobId;
            std::atomic<uint32_t> dependencyCount{ 0 };
            std::vector<uint64_t> dependents;
            std::function<void()> execute;
            bool isCompleted{ false };
        };

        void AddJob(uint64_t jobId, std::function<void()> jobFunc,
            const std::vector<uint64_t>& dependencies = {});

        void MarkJobCompleted(uint64_t jobId);

        bool IsFrameComplete(uint64_t frameId);

        void ClearFrame(uint64_t frameId);

    private:
        std::unordered_map<uint64_t, JobNode> m_jobs;
        std::mutex m_mutex;

        // 按帧分组
        std::unordered_map<uint64_t, std::vector<uint64_t>> m_frameJobs;
    };

};