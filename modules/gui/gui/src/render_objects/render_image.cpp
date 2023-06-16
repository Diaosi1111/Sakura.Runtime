#include "SkrGui/render_objects/render_image.hpp"
#include "SkrGui/dev/gdi/gdi.hpp"
#include "SkrGui/framework/window_context.hpp"
#include "SkrGui/dev/interface/window.hpp"

namespace skr
{
namespace gui
{
RenderImage::RenderImage(skr_gdi_device_id gdi_device)
    : RenderBox(gdi_device)
{
    gdi_element = gdi_device->create_element();
    diagnostic_builder.add_properties(
    SkrNew<TextDiagnosticProperty>(u8"type", u8"image", u8"draw an image"));
}

RenderImage::~RenderImage()
{
    gdi_device->free_element(gdi_element);
}

void RenderImage::layout(BoxConstraint constraints, bool needSize)
{
    size = constraints.constrain(size);
}

void RenderImage::draw(const DrawParams* params)
{
    gdi_element->begin_frame(1.0f);
    gdi_element->begin_path();
    gdi_element->rect(pos.x, pos.y, size.width, size.height);
    gdi_element->fill_color(color.x, color.y, color.z, color.w);
    gdi_element->fill();
    // gdi_element->restore();
    params->canvas->add_element(gdi_element);
    RenderBox::draw(params);
}

} // namespace gui
} // namespace skr