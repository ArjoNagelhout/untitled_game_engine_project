//
// Created by Arjo Nagelhout on 22/04/2024.
//

#include "unique_any_pointer.h"

#include <cassert>

namespace reflection
{
    AnyDeleter::AnyDeleter() = default;

    AnyDeleter::~AnyDeleter()
    {
        reset();
    }

    AnyDeleter::AnyDeleter(AnyDeleter&& other) noexcept
    {
        handle = other.handle;
        deleter = other.deleter;
        other.handle = nullptr;
        other.deleter = nullptr;
    }

    AnyDeleter& AnyDeleter::operator=(AnyDeleter&& other) noexcept
    {
        handle = other.handle;
        deleter = other.deleter;
        other.handle = nullptr;
        other.deleter = nullptr;
        return *this;
    }

    void AnyDeleter::operator()(void* data)
    {
        assert(handle);
        handle(Action::Delete, data, deleter);
    }

    bool AnyDeleter::valid() const
    {
        return deleter && handle;
    }

    void AnyDeleter::reset()
    {
        // don't need to reset if contains no deleter
        if (deleter)
        {
            assert(handle && "if deleter is not nullptr, handle should not be nullptr");
            handle(Action::DestroyDeleter, nullptr, deleter);
        }
    }

    UniqueAnyPointer::UniqueAnyPointer() = default;

    UniqueAnyPointer::UniqueAnyPointer(nullptr_t) {}

    UniqueAnyPointer::UniqueAnyPointer(UniqueAnyPointer&& other) noexcept
    {
        if (other.handle)
        {
            other.handle(Action::Move, &other, this);
        }
    }

    UniqueAnyPointer::~UniqueAnyPointer()
    {
        reset();
    }

    bool UniqueAnyPointer::isType(TypeId typeId_) const
    {
        return typeId() == typeId_;
    }

    TypeId UniqueAnyPointer::typeId() const
    {
        if (handle)
        {
            return *static_cast<TypeId*>(call(Action::TypeId));
        }
        return nullTypeId;
    }

    void UniqueAnyPointer::reset()
    {
        if (handle)
        {
            call(Action::Destroy, this);
        }
    }

    void UniqueAnyPointer::swap(UniqueAnyPointer& other)
    {
        if (this == &other)
        {
            return; // don't need to do anything
        }

        if (handle && other.handle)
        {
            UniqueAnyPointer temporary;
            other.call(Action::Move, &temporary);
            call(Action::Move, &other);
            temporary.call(Action::Move, this);
        }
        else if (handle)
        {
            call(Action::Move, &other);
        }
        else if (other.handle)
        {
            other.call(Action::Move, this);
        }
    }

    bool UniqueAnyPointer::empty() const
    {
        return data;
    }

    void* UniqueAnyPointer::release()
    {
        assert(data && "data should not be nullptr");
        assert(deleter.valid() && "deleter should be valid");
        return data;
    }

    AnyDeleter UniqueAnyPointer::getDeleter()
    {
        assert(deleter.valid() && "deleter should be valid");
        return std::move(deleter);
    }

    void* UniqueAnyPointer::call(Action action, UniqueAnyPointer* other) const
    {
        if (handle)
        {
            return handle(action, this, other);
        }
        return nullptr;
    }
}