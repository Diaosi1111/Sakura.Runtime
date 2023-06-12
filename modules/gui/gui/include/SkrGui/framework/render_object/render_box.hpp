#pragma once
#include "SkrGui/framework/render_object/render_object.hpp"

SKR_DECLARE_TYPE_ID_FWD(skr::gdi, GDIDevice, skr_gdi_device)
SKR_DECLARE_TYPE_ID_FWD(skr::gdi, GDIElement, skr_gdi_element)

namespace skr::gui
{
using RenderBoxSizeType = BoxSizeType;

struct BoxConstraint {
    RenderBoxSizeType max_size;
    RenderBoxSizeType min_size;
    SKR_GUI_API RenderBoxSizeType apply(const RenderBoxSizeType& size) const;
};

struct Ray {
    skr_float3_t origin;
    skr_float3_t direction;
};

struct HitTestRecord {
};

struct SKR_GUI_API RenderBox : public RenderObject {
    SKR_GUI_TYPE(RenderBox, "01a2eb19-1299-4069-962f-88db0c719134", RenderObject);

public:
    RenderBox(skr_gdi_device_id gdi_device);
    virtual ~RenderBox();

    virtual void layout(BoxConstraint constraints, bool needSize = false) = 0;
    virtual void before_draw(const DrawParams* params) override;
    virtual void draw(const DrawParams* params) override;
    virtual bool hit_test(const Ray& point, HitTestRecord* record) const;

    virtual RenderBoxSizeType get_size() const;
    virtual void set_size(const RenderBoxSizeType& size);
    virtual void set_position(const RenderBoxSizeType& position);
    RenderBox* get_child_as_box(int index) const { return (RenderBox*)get_child(index); }

    virtual void enable_debug_draw(bool enable);

protected:
    bool draw_debug_rect = false;
    RenderBoxSizeType pos = { 0, 0 };
    RenderBoxSizeType size = { 0, 0 };

    skr_gdi_device_id gdi_device = nullptr;
    skr_gdi_element_id debug_element = nullptr;
};

} // namespace skr::gui

SKR_DECLARE_TYPE_ID(skr::gui::RenderBox, skr_gui_render_box);