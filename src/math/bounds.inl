//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef BORED_ENGINE_BOUNDS_INL
#define BORED_ENGINE_BOUNDS_INL

#include "bounds.h"
#include "vector.inl"

namespace math
{
	constexpr Bounds::Bounds(vec3 const& center, vec3 const& extents)
		: _center(center), _extents(extents)
	{
	}

	constexpr Bounds::~Bounds() = default;

	constexpr vec3 Bounds::min() const
	{
		return _center - _extents;
	}

	constexpr vec3 Bounds::max() const
	{
		return _center + _extents;
	}

	constexpr vec3 Bounds::center() const
	{
		return _center;
	}

	constexpr vec3 Bounds::extents() const
	{
		return _extents;
	}

	constexpr void Bounds::setMin(vec3 const& min)
	{

	}

	constexpr void Bounds::setMax(vec3 const& max)
	{

	}

	constexpr void Bounds::setMinMax(vec3 const& min, vec3 const& max)
	{
		_center = (min + max) / 2.f;
		// todo: _extents =
	}

	constexpr vec3 Bounds::size() const
	{
		return _extents * 2;
	}

	constexpr vec3 Bounds::closestPoint(vec3 const& point) const
	{
		return point;
	}

	constexpr bool Bounds::contains(vec3 const& point) const
	{
		return false;
	}

	constexpr void Bounds::encapsulate(vec3 const& point)
	{

	}

	constexpr bool Bounds::intersects(Bounds const& lhs, Bounds const& rhs)
	{
		return false;
	}

	constexpr bool Bounds::intersects(Bounds const& lhs, Ray const& rhs)
	{
		return false;
	}
}

#endif //BORED_ENGINE_BOUNDS_INL