export module Aether:Thread;
import :Engine;
import :EngineDefinition;
import :Semaphore;

namespace Aether
{
    typedef AResult(*ThreadFn)(AetherEngine* engine, class Thread* thread, void* _userData);

    class Thread
    {
    public:
        Thread(AetherEngine* engine);
        ~Thread() = default;

        AResult Init(ThreadFn fn, void* user_data = nullptr, uint32_t stack_size = 0, const char* thread_name = nullptr);
        void    ShutDown();
        bool    IsRunning() const { return m_bRunning; }
        uint32_t GetExitCode() const { return m_iExitCode; }

        Semaphore& GetSemaphore() { return m_Semaphore; }

    private:
        friend struct ThreadInternal;
        AResult     Entry();

    private:
        AetherEngine* m_pEngine;
        uint8_t     m_Internal[128];

        ThreadFn    m_pThreadFn = nullptr;
        void*       m_pUserData = nullptr;

        Semaphore   m_Semaphore;
        uint32_t    m_iStackSize = 0;
        uint32_t    m_iExitCode = 0;
        bool        m_bRunning = false;
    };
};