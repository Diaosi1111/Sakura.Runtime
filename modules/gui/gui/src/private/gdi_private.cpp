#include "platform/debug.h"
#include "platform/memory.h"
#include "SkrGui/interface/gdi_renderer.hpp"
#include "private/gdi_private.hpp"

#include "nanovg/gdi_nanovg.hpp"

namespace skr
{
namespace gdi
{

void GDICanvasPrivate::add_element(GDIElement* element) SKR_NOEXCEPT
{
    all_elements_.emplace_back(element);
}

void GDICanvasPrivate::remove_element(GDIElement* element) SKR_NOEXCEPT
{
    auto it = eastl::find(all_elements_.begin(), all_elements_.end(), element);
    if (it != all_elements_.end())
    {
        all_elements_.erase(it);
    }
}

void GDICanvasPrivate::clear_elements() SKR_NOEXCEPT
{
    all_elements_.clear();
}

Span<GDIElement*> GDICanvasPrivate::all_elements() SKR_NOEXCEPT
{
    return { all_elements_.data(), all_elements_.size() };
}

void GDIViewportPrivate::add_canvas(GDICanvas* canvas) SKR_NOEXCEPT
{
    all_canvas_.emplace_back(canvas);
}

void GDIViewportPrivate::remove_canvas(GDICanvas* canvas) SKR_NOEXCEPT
{
    auto it = eastl::find(all_canvas_.begin(), all_canvas_.end(), canvas);
    if (it != all_canvas_.end())
    {
        all_canvas_.erase(it);
    }
}

void GDIViewportPrivate::clear_canvas() SKR_NOEXCEPT
{
    all_canvas_.clear();
}

Span<GDICanvas*> GDIViewportPrivate::all_canvas() SKR_NOEXCEPT
{
    return { all_canvas_.data(), all_canvas_.size() };
}

GDIDevice* GDIDevice::Create(EGDIBackend backend)
{
    switch (backend)
    {
        case EGDIBackend::NANOVG: {
            auto nvgDevice = SkrNew<GDIDeviceNVG>();
            nvgDevice->initialize();
            return nvgDevice;
        }
        default:
            SKR_UNREACHABLE_CODE();
            return nullptr;
    }
}

void GDIDevice::Free(GDIDevice* device)
{
    SkrDelete(device);
}

GDIViewport* GDIDevice::create_viewport()
{
    return SkrNew<GDIViewportPrivate>();
}

void GDIDevice::free_viewport(GDIViewport* canvas)
{
    SkrDelete(canvas);
}

GDICanvas* GDIDevice::create_canvas()
{
    return SkrNew<GDICanvasPrivate>();
}

void GDIDevice::free_canvas(GDICanvas* render_group)
{
    SkrDelete(render_group);
}

Span<GDIVertex> IGDIRenderer::fetch_element_vertices(GDIElement* element) SKR_NOEXCEPT
{
    const auto element_private = static_cast<GDIElementPrivate*>(element);
    return { element_private->vertices.data(), element_private->vertices.size() };
}

Span<index_t> IGDIRenderer::fetch_element_indices(GDIElement* element) SKR_NOEXCEPT
{
    const auto element_private = static_cast<GDIElementPrivate*>(element);
    return { element_private->indices.data(), element_private->indices.size() };
}

Span<GDIElementDrawCommand> IGDIRenderer::fetch_element_draw_commands(GDIElement* element) SKR_NOEXCEPT
{
    const auto element_private = static_cast<GDIElementPrivate*>(element);
    return { element_private->commands.data(), element_private->commands.size() };
}

} // namespace gdi
} // namespace skr