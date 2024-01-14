//
// Created by Arjo Nagelhout on 06/01/2024.
//

#ifndef SHAPEREALITY_VIEW_H
#define SHAPEREALITY_VIEW_H

#include <tuple>
#include <iostream>

namespace entity
{
    /*
     * ViewIterator wraps an iterator of the view property of View
     */
    template<typename... Types>
    class ViewIterator final
    {
    public:
        using iterator = SparseSetBase::base_iterator;

        explicit ViewIterator(iterator current, iterator last, std::tuple<Types*...> _components)
            : current(current), last(last), components(_components)
        {
        }

        ViewIterator& operator++()
        {
            while (++current != last && !valid(*current))
            {}
            return *this;
        }

        ViewIterator operator++(int)
        {
            ViewIterator orig = *this;
            ++(*this);
            return orig;
        }

//        [[nodiscard]] pointer operator->() const {
//            return &*current;
//        }

//        [[nodiscard]] std::tuple<Types& ...> operator*() const
//        {
//            return std::make_tuple(*components);
//
//            //return *operator->();
//        }

        ~ViewIterator() = default;

    private:
        iterator current;
        iterator last;
        std::tuple<Types* ...> components;

        // returns whether the given index is valid
        [[nodiscard]] bool valid(entity_type entityId) const
        {
            if (entityId == TOMBSTONE)
            {
                return false;
            }

            bool invalid = false;
            // iterate over all components to check if they contain the provided entityId
            std::apply([&entityId, &invalid](auto* ...component) {
                ((invalid = !component->contains(entityId) ? true : invalid), ...);
            }, components);

            return !invalid;
        }

        template<typename... TypesLhs, typename... TypesRhs>
        friend constexpr bool operator==(ViewIterator<TypesLhs...> const&, ViewIterator<TypesRhs...> const&);
    };

    template<typename... TypesLhs, typename... TypesRhs>
    [[nodiscard]] constexpr bool operator==(ViewIterator<TypesLhs...> const& lhs, ViewIterator<TypesRhs...> const& rhs)
    {
        return lhs.current == rhs.current;
    }

    template<typename... TypesLhs, typename... TypesRhs>
    [[nodiscard]] constexpr bool operator!=(ViewIterator<TypesLhs...> const& lhs, ViewIterator<TypesRhs...> const& rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * A view enables iterating over entities that contain a set of types,
     * e.g. TransformComponent, MeshRendererComponent
     *
     * @tparam Types which component types to get
     */
    template<typename... Types>
    requires (std::is_base_of_v<SparseSetBase, Types> && ...) // fold expression
    && (sizeof...(Types) > 1u) // why would you ever make a view of a single type, just directly iterate over the sparse set

    class View final
    {
    public:
        using iterator = ViewIterator<Types...>;

        explicit View(Types* ..._components) : components(_components...)
        {
            updateView();
            std::cout << "dense size: " << view->denseSize() << std::endl;
        }

        ~View() = default;

        [[nodiscard]] iterator begin()
        {
            return iterator{view->beginBase(), view->endBase(), components};
        }

        [[nodiscard]] iterator end()
        {
            return iterator{view->endBase(), view->endBase(), components};
        }

    private:
        std::tuple<Types* ...> components;
        SparseSetBase* view; // the sparse set to use

        void updateView()
        {
            // select the component type with the smallest denseSize
            view = std::get<0>(components);
            std::apply([this](auto* ...other) {
                ((this->view = other->denseSize() < this->view->denseSize() ? other : this->view), ...);
            }, components);
        }
    };
}

#endif //SHAPEREALITY_VIEW_H
