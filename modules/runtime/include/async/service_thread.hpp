#pragma once
#include "async/named_thread.hpp"

namespace skr
{
struct ServiceThreadDesc
{
    const char8_t* name = nullptr;
    SThreadPriority priority = SKR_THREAD_NORMAL;
};

struct RUNTIME_STATIC_API ServiceThread
{
public:
    ServiceThread(const ServiceThreadDesc& desc) SKR_NOEXCEPT;
    virtual ~ServiceThread() SKR_NOEXCEPT;
    
    enum Status
    {
        kStatusStopped = 0,
        kStatusWaking = 1,
        kStatusRunning = 2,
        kStatusStopping = 3,
        kStatusExiting = 4,
        kStatusExitted = 5
    };
    virtual Status get_status() const SKR_NOEXCEPT;
    
    virtual void request_stop() SKR_NOEXCEPT;
    virtual void stop() SKR_NOEXCEPT;
    virtual void wait_stop() SKR_NOEXCEPT;

    virtual void run() SKR_NOEXCEPT;

    virtual void request_exit() SKR_NOEXCEPT;
    virtual void exit() SKR_NOEXCEPT;
    virtual void wait_exit() SKR_NOEXCEPT;

    virtual AsyncResult serve() SKR_NOEXCEPT = 0;

protected:
    void waitJoin() SKR_NOEXCEPT;

    struct ServiceFunc : public NamedThreadFunction
    {
        AsyncResult run() SKR_NOEXCEPT;
        ServiceThread* _service = nullptr;
    };
    friend struct ServiceFunc;
    ServiceFunc f;
    NamedThread t;
    SAtomicU32 status = kStatusStopped;
    SAtomicU32 rid = 0;
};
}