//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_INFO_H
#define SHAPEREALITY_TYPE_INFO_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <any>
#include <cassert>

#include "type_id.h"

/**
 * @namespace reflection
 * @brief simple reflection of plain-old datastructures (POD) or compositions of PODs
 */
namespace reflection
{
    template<typename>
    struct is_list : std::false_type
    {
    };

    template<typename T>
    struct is_list<std::vector<T>> : std::true_type
    {
    };

    template<typename>
    struct is_dictionary : std::false_type
    {
    };

    template<typename Key, typename Value>
    struct is_dictionary<std::unordered_map<Key, Value>> : std::true_type
    {
    };

    enum class PropertyType
    {
        Object = 0,
        List, // std::vector<T>
        Dictionary, // std::unordered_map<Key, Value>
    };

    // returns a pointer to the member
    // instance should be a pointer
    template<typename Type, auto Data>
    std::any getter(std::any instance)
    {
        auto castInstance = std::any_cast<Type*>(instance);
        return &(std::invoke(Data, castInstance));
    }

    // instance should be a pointer
    template<typename Type, auto Data>
    void setter(std::any instance, std::any value)
    {
        using value_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;

        auto castInstance = std::any_cast<Type*>(instance);
        std::invoke(Data, castInstance) = std::any_cast<value_type>(value);
    }

    struct ObjectPropertyInfo
    {
        type_id typeId;

        std::any (* getter)(std::any);

        void (* setter)(std::any, std::any);

        // instance is pointer
        template<typename ValueType>
        ValueType* get(std::any const& instance)
        {
            return std::any_cast<ValueType*>(getter(instance));
        }

        // instance is pointer
        template<typename ValueType>
        void set(std::any const& instance, ValueType value)
        {
            setter(instance, value);
        }
    };

    struct ListPropertyInfo
    {

    };

    struct DictionaryPropertyInfo
    {

    };

    struct PropertyInfo
    {
        std::string name;
        PropertyType type = PropertyType::Object;

        union
        {
            ObjectPropertyInfo object;
            ListPropertyInfo list;
            DictionaryPropertyInfo dictionary;
        };
    };

    struct TypeInfo
    {
        std::string name;
        std::vector<PropertyInfo> properties;
    };

    struct TypeInfoRegistry;

    template<typename Type>
    class TypeInfoBuilder
    {
    public:
        explicit TypeInfoBuilder(std::string name)
        {
            typeInfo.name = std::move(name);
        }

        template<auto Data>
        TypeInfoBuilder& addProperty(std::string const& name)
        {
            // we assume the property is non-const and non-volatile
            using value_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;

            // a property needs to be handled differently depending on whether it is a regular object, list or dictionary
            if constexpr (is_list<value_type>::value)
            {
                // list
                typeInfo.properties.emplace_back(PropertyInfo{
                    .name = name,
                    .type = PropertyType::List
                });
            }
            else if constexpr (is_dictionary<value_type>::value)
            {
                // dictionary
                typeInfo.properties.emplace_back(PropertyInfo{
                    .name = name,
                    .type = PropertyType::Dictionary
                });
            }
            else
            {
                // object
                type_id id = TypeIndex<value_type>().value();
                typeInfo.properties.emplace_back(PropertyInfo{
                    .name = name,
                    .type = PropertyType::Object,
                    .object = {
                        .typeId = id,
                        .getter = getter<Type, Data>,
                        .setter = setter<Type, Data>
                    }
                });
            }

            return *this;
        }

        [[nodiscard]] TypeInfo build() const
        {
            return typeInfo;
        }

        // registers the type to the provided `TypeInfoRegistry`
        void registerType(TypeInfoRegistry& r);

    private:
        TypeInfo typeInfo;
    };

    // contains a map of type ids to info's
    class TypeInfoRegistry
    {
    public:
        template<typename Type>
        void registerType(TypeInfo&& info)
        {
            type_id id = TypeIndex<Type>().value();
            assert(!types.contains(id) && "error: type was already registered");
            types[id] = std::move(info);
        }

        // returns nullptr if type was not registered (i.e. unknown type)
        [[nodiscard]] TypeInfo* getTypeInfo(type_id id)
        {
            if (types.contains(id))
            {
                return &types[id];
            }
            else
            {
                return nullptr;
            }
        }

        template<typename Type>
        [[nodiscard]] TypeInfo* getTypeInfo()
        {
            type_id id = TypeIndex<Type>().value();
            return getTypeInfo(id);
        }

    private:
        std::unordered_map<type_id, TypeInfo> types;
    };

    template<typename Type>
    void TypeInfoBuilder<Type>::registerType(TypeInfoRegistry& r)
    {
        r.registerType<Type>(std::move(typeInfo));
    }

    // ------------------------------------
    // Iterate using stack
    // ------------------------------------

    // data for iterating over an object
    struct ObjectStackFrame
    {
        type_id typeId;
        TypeInfo* typeInfo = nullptr; // cached value to avoid calling TypeInfoRegistry::get(type_id) multiple times
        size_t index = 0; // for iterating over the properties of the type
    };

    // data for iterating over a list
    struct ListStackFrame
    {

    };

    // data for iterating over a dictionary
    struct DictionaryStackFrame
    {

    };

    struct StackFrame
    {
        std::string name;
        std::any value = nullptr; // pointer to current instance

        PropertyType type = PropertyType::Object;

        union
        {
            ObjectStackFrame object;
            ListStackFrame list;
            DictionaryStackFrame dictionary;
        };
    };

    using iterate_callback = std::function<bool(StackFrame const&)>;
    using iterate_on_pop = std::function<void(StackFrame const&)>;

    /**
     * @tparam Type
     * @param r
     * @param name name of the root instance to use
     * @param instance
     * @param callback called on each property inside the reflected type.
     * should return whether to continue iterating. this is irrelevant for primitive types
     * @param onStackPop called when the type is popped from the stack. useful for example for calling ImGui::TreePop()
     */
    template<typename Type>
    void iterateUsingStack(TypeInfoRegistry& r,
                           std::string const& name,
                           Type* instance,
                           iterate_callback const& callback,
                           iterate_on_pop const& onPop)
    {
        iterateUsingStack(r,
                          TypeIndex<Type>::value(),
                          name,
                          std::make_any<Type*>(instance),
                          callback,
                          onPop);
    }

    /**
     * non-generic implementation, so that the stack based iteration doesn't get copied for each type
     * we want to iterate.
     */
    void iterateUsingStack(TypeInfoRegistry& r,
                           type_id typeId,
                           std::string const& name,
                           std::any const& instance,
                           iterate_callback const& callback,
                           iterate_on_pop const& onPop);
}

#endif //SHAPEREALITY_TYPE_INFO_H
