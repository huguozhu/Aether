export module Aether:Mutex;

namespace Aether
{
    class Mutex
    {
    public:
        Mutex();
        ~Mutex();

        void Lock();
        void Unlock();

    private:
        unsigned char m_Data[64] = { 0 };
    };


    class MutexScope
    {
    public:
        MutexScope(Mutex& m);
        ~MutexScope();

    private:
        Mutex& m_Mutex;
    };
};
