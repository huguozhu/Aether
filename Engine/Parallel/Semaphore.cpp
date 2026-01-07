module;
#if defined(AETHER_PLATFORM_WINDOWS)
#include "windows.h"
#endif

module Aether:Semaphore;
import :Semaphore;

namespace Aether
{
    Semaphore::Semaphore()
    {
        m_Handle = CreateSemaphoreA(nullptr, 0, LONG_MAX, nullptr);
    }

    Semaphore::~Semaphore()
    {
        if (m_Handle)
        {
            CloseHandle(m_Handle);
            m_Handle = nullptr;
        }
    }

    void Semaphore::Post(uint32_t count)
    {
        ReleaseSemaphore(m_Handle, count, nullptr);
    }
    bool Semaphore::Wait(uint32_t msecs)
    {
        return WAIT_OBJECT_0 == WaitForSingleObject(m_Handle, msecs);
    }
};