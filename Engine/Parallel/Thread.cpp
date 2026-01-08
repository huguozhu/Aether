module;
#if defined(AETHER_PLATFORM_WINDOWS)
#include "windows.h"
#endif

module Aether:Thread;
import :Thread;
import :Error;
import <string>;

namespace Aether
{
    struct ThreadInternal
    {
#if defined(AETHER_PLATFORM_WINDOWS)
        static DWORD WINAPI m_pThreadFunc(LPVOID arg)
        {
            Thread* pThread = (Thread*)arg;
            return pThread->Entry();
        }
        HANDLE      m_Handle = INVALID_HANDLE_VALUE;
        uint32_t    m_iThreadId = UINT32_MAX;
#endif
    };


    /******************************************************************************
     * Thread
     ******************************************************************************/
    Thread::Thread()
    {
    }

    AResult Thread::Init(ThreadFn fn, void* user_data, uint32_t stack_size, const char* thread_name)
    {
        m_pThreadFn = fn;
        m_pUserData = user_data;
        m_iStackSize = stack_size;

        ThreadInternal* ti = (ThreadInternal*)m_Internal;
#if defined(AETHER_PLATFORM_WINDOWS)
        ti->m_Handle = CreateThread(nullptr, m_iStackSize,
            ti->m_pThreadFunc, this, 0, (DWORD*)&ti->m_iThreadId);
        if (!ti->m_Handle)
            return ERR_INVALID_INIT;
        m_bRunning = true;
#endif
        return A_Success;
    }
    void Thread::ShutDown()
    {
        ThreadInternal* ti = (ThreadInternal*)m_Internal;
#if defined(AETHER_PLATFORM_WINDOWS)
        WaitForSingleObject(ti->m_Handle, INFINITE);
        GetExitCodeThread(ti->m_Handle, (DWORD*)&m_iExitCode);
        CloseHandle(ti->m_Handle);
        ti->m_Handle = INVALID_HANDLE_VALUE;
#endif
        m_bRunning = false;
    }

    AResult Thread::Entry()
    {
#if defined(AETHER_PLATFORM_WINDOWS)
        ThreadInternal* ti = (ThreadInternal*)m_Internal;
        ti->m_iThreadId = GetCurrentThreadId();
#endif
        return m_pThreadFn(this, m_pUserData);
    }

}

