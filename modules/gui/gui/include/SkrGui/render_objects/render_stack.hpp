#include "SkrGui/framework/render_object/render_box.hpp"

namespace skr
{
namespace gui
{
struct PositionalValue
{
    LiteOptional<float> value;
    bool is_percent = false;
    explicit operator bool() const { return value; }
    float get_value(float parent_value) const
    {
        if (is_percent)
        {
            return parent_value * value.get();
        }
        else
        {
            return value.get();
        }
    }
};

struct Positional
{
    PositionalValue left;
    PositionalValue top;
    PositionalValue right;
    PositionalValue bottom;
    PositionalValue minWidth;
    PositionalValue minHeight;
    PositionalValue maxWidth;
    PositionalValue maxHeight;

    skr_float2_t pivot;
    Positional& set_left(float left)
    {
        this->left.value = left;
        this->left.is_percent = false;
        return *this;
    }
    Positional& set_left_percent(float left)
    {
        this->left.value = left;
        this->left.is_percent = true;
        return *this;
    }
    Positional& set_top(float top)
    {
        this->top.value = top;
        this->top.is_percent = false;
        return *this;
    }
    Positional& set_top_percent(float top)
    {
        this->top.value = top;
        this->top.is_percent = true;
        return *this;
    }
    Positional& set_right(float right)
    {
        this->right.value = right;
        this->right.is_percent = false;
        return *this;
    }
    Positional& set_right_percent(float right)
    {
        this->right.value = right;
        this->right.is_percent = true;
        return *this;
    }
    Positional& set_bottom(float bottom)
    {
        this->bottom.value = bottom;
        this->bottom.is_percent = false;
        return *this;
    }
    Positional& set_bottom_percent(float bottom)
    {
        this->bottom.value = bottom;
        this->bottom.is_percent = true;
        return *this;
    }
    Positional& set_min_width(float width)
    {
        this->minWidth.value = width;
        this->minWidth.is_percent = false;
        return *this;
    }
    Positional& set_min_width_percent(float width)
    {
        this->minWidth.value = width;
        this->minWidth.is_percent = true;
        return *this;
    }
    Positional& set_min_height(float height)
    {
        this->minHeight.value = height;
        this->minHeight.is_percent = false;
        return *this;
    }
    Positional& set_min_height_percent(float height)
    {
        this->minHeight.value = height;
        this->minHeight.is_percent = true;
        return *this;
    }
    Positional& set_max_width(float width)
    {
        this->maxWidth.value = width;
        this->maxWidth.is_percent = false;
        return *this;
    }
    Positional& set_max_width_percent(float width)
    {
        this->maxWidth.value = width;
        this->maxWidth.is_percent = true;
        return *this;
    }
    Positional& set_max_height(float height)
    {
        this->maxHeight.value = height;
        this->maxHeight.is_percent = false;
        return *this;
    }
    Positional& set_max_height_percent(float height)
    {
        this->maxHeight.value = height;
        this->maxHeight.is_percent = true;
        return *this;
    }
    Positional& set_width(float width)
    {
        this->minWidth.value = width;
        this->minWidth.is_percent = false;
        this->maxWidth.value = width;
        this->maxWidth.is_percent = false;
        return *this;
    }
    Positional& set_width_percent(float width)
    {
        this->minWidth.value = width;
        this->minWidth.is_percent = true;
        this->maxWidth.value = width;
        this->maxWidth.is_percent = true;
        return *this;
    }
    Positional& set_height(float height)
    {
        this->minHeight.value = height;
        this->minHeight.is_percent = false;
        this->maxHeight.value = height;
        this->maxHeight.is_percent = false;
        return *this;
    }
    Positional& set_height_percent(float height)
    {
        this->minHeight.value = height;
        this->minHeight.is_percent = true;
        this->maxHeight.value = height;
        this->maxHeight.is_percent = true;
        return *this;
    }
    Positional& set_pivot(float x, float y)
    {
        this->pivot.x = x;
        this->pivot.y = y;
        return *this;
    }
};

class SKR_GUI_API RenderStack : public RenderBox
{
public:
    SKR_GUI_TYPE(RenderStack, RenderBox, u8"0c1ac8b5-d3aa-4560-a011-4b655231c8ac");
    RenderStack(skr_gdi_device_id gdi_device);

    virtual void layout(BoxConstraint constraints, bool needSize = false) override;
    Positional get_position(int index); //each child's corresponding positional property
    virtual void add_child(RenderObject* child) override;
    virtual void insert_child(RenderObject* child, int index) override;
    virtual void remove_child(RenderObject* child) override;
    void set_positional(int index, Positional positional);

private:
    VectorStorage<Positional> positionals;
};
}
} // namespace skr