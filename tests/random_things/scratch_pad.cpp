//
// Created by Arjo Nagelhout on 09/12/2023.
//

#include "gtest/gtest.h"

#include <memory>

#ifdef SCRATCHPAD

// performance testing for casting

struct Base
{
	[[nodiscard]] virtual bool isBase() const
	{
		return true;
	}
};

struct Inherited final : public Base
{
	[[nodiscard]] bool isBase() const override
	{
		return false;
	}
};
//
//template <typename T>
//bool staticIsBase(std::shared_ptr<T> const& b)
//{
//	Inherited const* inherited;
//	if constexpr (std::is_same_v<T, Inherited>) {
//		inherited = &*b;
//	} else {
//		inherited = dynamic_cast<Inherited const*>(&*b);
//	}
//	return inherited->isBase();
//}

bool staticIsBase(std::shared_ptr<Base> const& b)
{
	auto const& staticCast = static_cast<Inherited const&>(*b);
	return staticCast.isBase();
}

bool dynamicIsBase(std::shared_ptr<Base> const& b)
{
	auto const& dynamicCast = dynamic_cast<Inherited const&>(*b);
	return dynamicCast.isBase();
}

TEST(ScratchPad, Casting)
{
	std::shared_ptr<Base> inherited = std::make_unique<Inherited>();
	std::shared_ptr<Base> base = std::make_unique<Base>();

	std::cout << "is base (dynamic): " << dynamicIsBase(inherited) << std::endl;
	std::cout << "is base (static): " << staticIsBase(inherited) << std::endl;

	// staticIsBase does not throw an error, dynamicIsBase does.
	// dangerous :)
	//std::cout << "is base (dynamic): " << dynamicIsBase(base) << std::endl;
	std::cout << "is base (static): " << staticIsBase(base) << std::endl;
}

int ba()
{
//		constexpr int weee;

	constexpr int test = 5;
	return test;
}

struct Va
{
	explicit Va(int aa) : _aa(aa)
	{

	}

	int _aa;

	static Va boe;
};

Va Va::boe = Va(8);

#endif