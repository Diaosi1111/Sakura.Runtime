#pragma once
#include "SkrGui/fwd_config.hpp"
#include "SkrGui/framework/key.hpp"
#include "containers/not_null.hpp"

SKR_DECLARE_TYPE_ID_FWD(skr::gui, Element, skr_gui_element)
SKR_DECLARE_TYPE_ID_FWD(skr::gui, FocusManager, skr_gui_focus_manager)

namespace skr
{
namespace gui
{
struct SKR_GUI_API BuildOwner {
    BuildOwner();
    ~BuildOwner();

    // build
    void schedule_build_for(not_null<Element*> element) SKR_NOEXCEPT;
    void reassemble(Element* element) SKR_NOEXCEPT;
    void build_scope(Element* element) SKR_NOEXCEPT;
    void finalize_tree() SKR_NOEXCEPT;

    // global key
    void register_global_key(State* key, Element* element) SKR_NOEXCEPT;
    void unregister_global_key(State* key, Element* element) SKR_NOEXCEPT;

    VectorStorage<Element*> _dirty_elements;
    bool _dirty_elements_needs_resorting;
    bool _debug_is_in_build_scope;
    bool _debug_building;
    bool _scheduled_flush_dirty_elements;
    int _debug_state_lock_level;
    VectorStorage<Element*> _inactive_elements;
    FocusManager* _focus_manager;
    HashMapStorage<State*, Element*> _global_key_registry;
};
} // namespace gui
} // namespace skr