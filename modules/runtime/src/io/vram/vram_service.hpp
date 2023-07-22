#pragma once
#include "../common/io_runnner.hpp"
#include "../common/processors.hpp"
#include "SkrRT/io/vram_io.hpp"
#include "vram_batch.hpp"
#include "vram_request.hpp"

namespace skr {
namespace io {

struct VRAMService final : public IVRAMService
{
    VRAMService(const VRAMServiceDescriptor* desc) SKR_NOEXCEPT;
    
    [[nodiscard]] IOBatchId open_batch(uint64_t n) SKR_NOEXCEPT;
    [[nodiscard]] IORequestId open_request() SKR_NOEXCEPT;
    VRAMIOBufferId request(IORequestId request, IOFuture* future, SkrAsyncServicePriority priority) SKR_NOEXCEPT;
    void request(IOBatchId request) SKR_NOEXCEPT;
    
    void cancel(skr_io_future_t* future) SKR_NOEXCEPT 
    { 
        skr_atomicu32_store_relaxed(&future->request_cancel, 1); 
    }
    void stop(bool wait_drain = false) SKR_NOEXCEPT;
    void run() SKR_NOEXCEPT;
    void drain(SkrAsyncServicePriority priority = SKR_ASYNC_SERVICE_PRIORITY_COUNT) SKR_NOEXCEPT;
    void set_sleep_time(uint32_t time) SKR_NOEXCEPT;
    SkrAsyncServiceStatus get_service_status() const SKR_NOEXCEPT;
    void poll_finish_callbacks() SKR_NOEXCEPT;

    struct Runner final : public RunnerBase
    {
        Runner(VRAMService* service, skr::JobQueue* job_queue) SKR_NOEXCEPT;
        void enqueueBatch(const IOBatchId& batch) SKR_NOEXCEPT;
        void set_resolvers() SKR_NOEXCEPT;

        IOBatchBufferId batch_buffer = nullptr;
        IOReaderId<IIORequestProcessor> reader = nullptr;
        IOReaderId<IIOBatchProcessor> batch_reader = nullptr;

        VRAMService* service = nullptr;
    };

    const skr::string name;    
    const bool awake_at_request = false;
    Runner runner;
    
    SmartPoolPtr<VRAMIORequest, IIORequest> request_pool = nullptr;
    // SmartPoolPtr<VRAMIOBuffer, IVRAMIOBuffer> vram_buffer_pool = nullptr;
    SmartPoolPtr<VRAMIOBatch, IIOBatch> vram_batch_pool = nullptr;

private:
    static uint32_t global_idx;
    SAtomicU64 request_sequence = 0;
    SAtomicU64 batch_sequence = 0;
};

} // namespace io
} // namespace skr