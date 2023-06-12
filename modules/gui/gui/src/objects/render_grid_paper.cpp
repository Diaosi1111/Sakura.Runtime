#include "SkrGui/render_objects/render_grid_paper.hpp"
#include "SkrGui/gdi/gdi.hpp"
#include "SkrGui/framework/window_context.hpp"
#include "SkrGui/interface/window.hpp"

#include "tracy/Tracy.hpp"

namespace skr
{
namespace gui
{

inline static void draw_grid_paper(gdi::GDIElement* element, float window_width, float window_height)
{
    const bool bDrawRelativeXMesh = false;
    const bool bDrawRelativeYMesh = false;

    element->begin_frame(1.f);
    // draw background
    {
        element->begin_path();
        element->rect(0, 0, window_width, window_height);
        element->fill_color(235u, 235u, 235u, 255u);
        element->fill();
    }
    const auto epsillon = 3.f;
    const auto absUnitX = 10.f;
    const auto absUnitY = absUnitX;
    const auto unitX = window_width / 100.f;
    const auto unitY = window_height / 100.f;

    // draw relative main-meshes
    if (bDrawRelativeXMesh)
    {
        element->begin_path();
        element->stroke_width(2.f);
        element->stroke_color(0u, 200u, 64u, 200u);
        for (uint32_t i = 0; i < 10; ++i)
        {
            const auto pos = eastl::max(i * unitY * 10.f, epsillon);
            element->move_to(0.f, pos);
            element->line_to(window_width, pos);
        }
        element->stroke();
    }
    if (bDrawRelativeYMesh)
    {
        element->begin_path();
        element->stroke_width(2.f);
        element->stroke_color(200u, 0u, 64u, 200u);
        for (uint32_t i = 0; i < 10; ++i)
        {
            const auto pos = eastl::max(i * unitX * 10.f, epsillon);
            element->move_to(pos, 0.f);
            element->line_to(pos, window_height);
        }
        element->stroke();
    }

    // draw absolute main-meshes
    element->begin_path();
    element->stroke_width(2.f);
    element->stroke_color(125u, 125u, 255u, 200u);
    for (uint32_t i = 0; i < window_height / absUnitY / 10; ++i)
    {
        const auto pos = eastl::max(i * absUnitY * 10.f, epsillon);
        element->move_to(0.f, pos);
        element->line_to(window_width, pos);
    }
    for (uint32_t i = 0; i < window_width / absUnitX / 10; ++i)
    {
        const auto pos = eastl::max(i * absUnitX * 10.f, epsillon);
        element->move_to(pos, 0.f);
        element->line_to(pos, window_height);
    }
    element->stroke();

    // draw absolute sub-meshes
    element->begin_path();
    element->stroke_width(1.f);
    element->stroke_color(88u, 88u, 222u, 180u);
    for (uint32_t i = 0; i < window_height / absUnitY; ++i)
    {
        const auto pos = eastl::max(i * absUnitY, epsillon);
        element->move_to(0.f, pos);
        element->line_to(window_width, pos);
    }
    for (uint32_t i = 0; i < window_width / absUnitX; ++i)
    {
        const auto pos = eastl::max(i * absUnitX, epsillon);
        element->move_to(pos, 0.f);
        element->line_to(pos, window_height);
    }
    element->stroke();
}

RenderGridPaper::RenderGridPaper(skr_gdi_device_id gdi_device)
    : RenderBox(gdi_device)
    , gdi_element(nullptr)
{
    gdi_element = gdi_device->create_element();

    diagnostic_builder.add_properties(
        SkrNew<TextDiagnosticProperty>(u8"type", u8"grid_paper", u8"draws grid paper"));

    // TEST
    size.x = 900.f;
    size.y = 900.f;
}

RenderGridPaper::~RenderGridPaper()
{
    gdi_device->free_element(gdi_element);
}

void RenderGridPaper::layout(BoxConstraint constraints, bool needSize)
{
}

void RenderGridPaper::draw(const DrawParams* params)
{
    ZoneScopedN("DrawGridPaper");

    // TEST
    auto platform_window = params->window_context->get_platform_window();
    uint32_t w, h;
    platform_window->get_extent(&w, &h);
    const float window_width = (float)w, window_height = (float)h;
    // END TEST

    draw_grid_paper(gdi_element, window_width, window_height);

    addElementToCanvas(params, gdi_element);

    RenderBox::draw(params);
}

} // namespace gui
} // namespace skr