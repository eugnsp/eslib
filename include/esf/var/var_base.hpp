#pragma once
#include <esf/index.hpp>

#include <esu/tuple.hpp>
#include <esu/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace esf::internal
{
template<class Element_, class... Bnd_conds>
class Var_base
{
public:
	using Element   = Element_;
	using Space_dim = typename Element::Space_dim;
	static constexpr auto n_bnd_conds  = sizeof...(Bnd_conds);
	static constexpr bool has_bnd_cond = n_bnd_conds > 0;

private:
	using Bnd_conds_tuple = std::tuple<std::optional<Bnd_conds>...>;

public:
	template<std::size_t i = 0>
	auto& bnd_cond()
	{
		auto& bnd_cond = std::get<i>(bnd_conds_);
		assert(bnd_cond.has_value());
		return *bnd_cond;
	}

	template<std::size_t i = 0>
	const auto& bnd_cond() const
	{
		auto& bnd_cond = std::get<i>(bnd_conds_);
		assert(bnd_cond.has_value());
		return *bnd_cond;
	}

	template<std::size_t i = 0,
			 typename... Args>
	void set_bnd_cond(Args&&... args)
	{
		std::get<i>(bnd_conds_).emplace(std::forward<Args>(args)...);
	}

	const std::string& name() const
	{
		return name_;
	}

	void set_name(std::string name)
	{
		name_ = std::move(name);
	}

	template<class Fn>
	void for_each_bnd_cond(Fn fn) const
	{
		esu::tuple_for_each([&fn](auto& bnd_cond) { fn(*bnd_cond); }, bnd_conds_);
	}

	template<class Fn>
	void for_each_strong_bnd_cond(Fn fn) const
	{
		for_each_bnd_cond([&fn]<class Bnd_cond>(const Bnd_cond& bnd_cond)
		{
			if constexpr (Bnd_cond::is_strong)
				fn(bnd_cond);
		});
	}

	template<class Fn>
	void for_each_weak_bnd_cond(Fn fn) const
	{
		for_each_bnd_cond([&fn]<class Bnd_cond>(const Bnd_cond& bnd_cond)
		{
			if constexpr (!Bnd_cond::is_strong)
				fn(bnd_cond);
		});
	}

private:
	Bnd_conds_tuple bnd_conds_;
	std::string name_{"Unnamed variable"};
};
} // namespace esf::internal
