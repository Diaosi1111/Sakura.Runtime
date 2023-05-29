#pragma once
#include "misc/function_ref.hpp"
#include "containers/not_null.hpp"
#include "SkrGui/framework/diagnostics.hpp"

SKR_DECLARE_TYPE_ID_FWD(skr::gui, RenderObject, skr_gui_render_object)
SKR_DECLARE_TYPE_ID_FWD(skr::gui, Widget, skr_gui_widget)
SKR_DECLARE_TYPE_ID_FWD(skr::gui, Element, skr_gui_element)
SKR_DECLARE_TYPE_ID_FWD(skr::gui, Slot, skr_gui_slot)

namespace skr {
namespace gui {
struct BuildOwner;
struct Key;
enum class ElementLifecycle : uint32_t
{
    initial,
    active,
    inactive,
    defunct,
};

struct SKR_GUI_API BuildContext : public DiagnosticableTreeNode
{
    virtual bool mounted() SKR_NOEXCEPT = 0;
    virtual Widget* get_widget() SKR_NOEXCEPT = 0;
    virtual RenderObject* find_render_object() SKR_NOEXCEPT = 0;
    virtual BoxSizeType get_size() SKR_NOEXCEPT = 0;
};

struct SKR_GUI_API Element : public BuildContext
{
    friend struct BuildOwner;

    Element(skr_gui_widget_id widget) SKR_NOEXCEPT;

    // life circle
    virtual void activate() SKR_NOEXCEPT;
    virtual void deactivate() SKR_NOEXCEPT;

    // element tree modify
    virtual void mount(Element* parent, Slot* slot) SKR_NOEXCEPT;
    virtual void unmount() SKR_NOEXCEPT;

    // render object tree modify
    virtual void attach_render_object(Slot* new_slot) SKR_NOEXCEPT;
    virtual void detach_render_object() SKR_NOEXCEPT;

    // widget update
    // rebuild --> perform_rebuild --> update_child --> update_slot_for_child
    //                                              --> update
    //                                              --> inflate_widget
    void rebuild(bool force = false) SKR_NOEXCEPT; // 控件树刷新的入口，由 BuildOwner 调用，实现固定，主要做一些 assert 工作
    virtual void perform_rebuild() SKR_NOEXCEPT; // 实际走到的 rebuild 逻辑，由具体的 Element 实现
    virtual Element* update_child(Element* child, Widget* new_widget, Slot* new_slot) SKR_NOEXCEPT; // perform_rebuild 中调用，在这里做 child 的 diff 工作
    virtual void update_slot_for_child(Element* child, Slot* new_slot) SKR_NOEXCEPT; // 最低开销的更新，仅仅更新 slot
    virtual void update(Widget* new_widget) SKR_NOEXCEPT; // 更新 child 的数据，将 widget 信息透传到 render object
    virtual not_null<Element*> inflate_widget(not_null<Widget*> widget, Slot* new_slot) SKR_NOEXCEPT; // 刷新 widget，最耗的更新

    // element tree query
    virtual void visit_children(skr::function_ref<void(Element*)> visitor) SKR_NOEXCEPT;
    virtual void visit_child_elements(skr::function_ref<void(Element*)> visitor) SKR_NOEXCEPT;
    virtual void forget_child(Element* child) SKR_NOEXCEPT;
    virtual void deactivate_child(Element* child) SKR_NOEXCEPT;

    // build context interfaces
    virtual bool mounted() SKR_NOEXCEPT override;
    virtual Widget* get_widget() SKR_NOEXCEPT override;
    virtual BoxSizeType get_size() SKR_NOEXCEPT override;
    virtual RenderObject* find_render_object() SKR_NOEXCEPT override;

private:
    // help functions
    Element* _retake_inactive_element(Key& key, not_null<Widget*> widget) SKR_NOEXCEPT;
    void _active_with_parent(Element* parent, Slot* slot) SKR_NOEXCEPT;
    static void _active_recursively(Element* element) SKR_NOEXCEPT;
    virtual void _update_slot(Slot* new_slot) SKR_NOEXCEPT;
    void _update_depth(int parentDepth) SKR_NOEXCEPT;
    static int _compare_depth(Element* a, Element* b) SKR_NOEXCEPT;
    bool _debug_is_in_scope(Element* ancestor) SKR_NOEXCEPT;

private:
    uint32_t _depth = 0;
    bool _dirty = true;
    bool _in_dirty_list = false;

    skr_gui_widget_id _widget = nullptr;
    skr_gui_element_id _parent = nullptr;
    skr_gui_slot_id _slot = nullptr;
    BuildOwner* _owner = nullptr;
    ElementLifecycle _lifecycle_state = ElementLifecycle::initial;
};

} // namespace gui
} // namespace skr