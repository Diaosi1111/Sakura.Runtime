#pragma once
#include "SkrRT/rttr/type/type.hpp"
#include "SkrRT/containers_new/multi_umap.hpp"
#include "SkrRT/containers_new/umap.hpp"
#include "SkrRT/containers_new/string.hpp"
#include "SkrRT/containers_new/span.hpp"
#include "SkrRT/containers_new/array.hpp"

namespace skr::rttr
{
struct BaseInfo {
    Type*  type   = nullptr;
    size_t offset = 0;
};
template <typename FROM, typename TO>
constexpr inline size_t get_cast_offset()
{
    return reinterpret_cast<size_t>(static_cast<TO*>(reinterpret_cast<FROM*>(0)));
}
struct Field {
    string name   = {};
    Type*  type   = nullptr;
    size_t offset = 0;
};
struct ParameterInfo {
    string name = {};
    Type*  type = nullptr;
};
struct Method {
    using ExecutableType = void (*)(void* self, void* parameters, void* return_value);

    string               name            = {};
    Type*                return_info     = nullptr;
    Array<ParameterInfo> parameters_info = {};
    ExecutableType       executable      = {};
};

struct RecordBasicMethodTable {
    void (*ctor)(void* self)                      = nullptr;
    void (*dtor)(void* self)                      = nullptr;
    void (*copy)(void* dst, const void* src)      = nullptr;
    void (*move)(void* dst, void* src)            = nullptr;
    void (*assign)(void* dst, const void* src)    = nullptr;
    void (*move_assign)(void* dst, void* src)     = nullptr;
    void (*hash)(const void* ptr, size_t& result) = nullptr;
};
template <typename T>
SKR_INLINE RecordBasicMethodTable make_record_basic_method_table()
{
    RecordBasicMethodTable table = {};
    if constexpr (std::is_default_constructible_v<T>)
    {
        table.ctor = +[](void* self) { new (self) T(); };
    }
    if constexpr (std::is_destructible_v<T>)
    {
        table.dtor = +[](void* self) { ((T*)self)->~T(); };
    }
    if constexpr (std::is_copy_constructible_v<T>)
    {
        table.copy = +[](void* dst, const void* src) { new (dst) T(*(T*)src); };
    }
    if constexpr (std::is_move_constructible_v<T>)
    {
        table.move = +[](void* dst, void* src) { new (dst) T(std::move(*(T*)src)); };
    }
    if constexpr (std::is_copy_assignable_v<T>)
    {
        table.assign = +[](void* dst, const void* src) { *(T*)dst = *(T*)src; };
    }
    if constexpr (std::is_move_assignable_v<T>)
    {
        table.move_assign = +[](void* dst, void* src) { *(T*)dst = std::move(*(T*)src); };
    }
    if constexpr (skr_hashable_v<T>)
    {
        table.hash = +[](const void* ptr, size_t& result) { result = Hash<T>()(*(const T*)ptr); };
    }

    return table;
}

struct SKR_RUNTIME_API RecordType : public Type {
    RecordType(string name, GUID type_id, size_t size, size_t alignment, RecordBasicMethodTable basic_methods);

    bool call_ctor(void* ptr) const override;
    bool call_dtor(void* ptr) const override;
    bool call_copy(void* dst, const void* src) const override;
    bool call_move(void* dst, void* src) const override;
    bool call_assign(void* dst, const void* src) const override;
    bool call_move_assign(void* dst, void* src) const override;
    bool call_hash(const void* ptr, size_t& result) const override;

    // setup
    SKR_INLINE void set_base_types(UMap<GUID, BaseInfo> base_types) { _base_types_map = std::move(base_types); }
    SKR_INLINE void set_fields(MultiUMap<string, Field> fields) { _fields_map = std::move(fields); }
    SKR_INLINE void set_methods(MultiUMap<string, Method> methods) { _methods_map = std::move(methods); }

    // getter
    SKR_INLINE const UMap<GUID, BaseInfo>& base_types() const { return _base_types_map; }
    SKR_INLINE const MultiUMap<string, Field>& fields() const { return _fields_map; }
    SKR_INLINE const MultiUMap<string, Method>& methods() const { return _methods_map; }

    // find base
    bool find_base(const Type* type, BaseInfo& result) const;

    // find methods
    // find fields

private:
    UMap<GUID, BaseInfo>      _base_types_map = {};
    MultiUMap<string, Field>  _fields_map     = {};
    MultiUMap<string, Method> _methods_map    = {};
    RecordBasicMethodTable    _basic_methods  = {};
};
} // namespace skr::rttr