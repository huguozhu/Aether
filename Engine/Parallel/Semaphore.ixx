export module Aether:Semaphore;

import std;
import <string>;

namespace Aether
{
    class Semaphore
    {
    public:
        Semaphore();
        ~Semaphore();

        void Post(uint32_t count = 1);
        bool Wait(uint32_t msecs = 0xFFFFFFFF);

    private:
#if defined(AETHER_PLATFORM_WINDOWS)
        void* m_Handle;
#endif
    };
};