#pragma once
#include <esf/types.hpp>

#include <esu/tuple.hpp>
#include <esu/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace esf::internal
{
// Base class of a variable
//
// Template parameters:
//		Element 		- finite element type,
//		Bnd_conds... 	- zero of more boundary condition types.
template<class Element_, class... Bnd_conds>
class Var_base
{
public:
	using Element = Element_;
	static constexpr auto n_bnd_conds = sizeof...(Bnd_conds);
	static constexpr bool has_bnd_cond = n_bnd_conds > 0;
	static constexpr Local_index space_dim = Element::dim;

	// TODO : remove
	static constexpr Local_index n_total_dofs = Element::n_total_face_dofs;

private:
	using Bnd_conds_tuple = std::tuple<std::unique_ptr<Bnd_conds>...>;

public:
	template<std::size_t index = 0>
	auto& bnd_cond()
	{
		assert(std::get<index>(bnd_conds_));
		return *std::get<index>(bnd_conds_);
	}

	template<std::size_t index = 0>
	const auto& bnd_cond() const
	{
		assert(std::get<index>(bnd_conds_));
		return *std::get<index>(bnd_conds_);
	}

	template<std::size_t index = 0, typename... Args>
	void set_bnd_cond(Args&&... args)
	{
		using Bnd_cond = typename std::tuple_element_t<index, Bnd_conds_tuple>::element_type;
		std::get<index>(bnd_conds_) = std::make_unique<Bnd_cond>(std::forward<Args>(args)...);
	}

	template<class Func>
	void for_each_ess_bnd_cond(Func func) const
	{
		esu::tuple_for_each(
			[&func](const auto& bnd_cond) {
				using Bnd_cond = esu::Remove_cv_ref<decltype(*bnd_cond)>;

				assert(bnd_cond);
				if constexpr (Bnd_cond::is_essential)
					func(*bnd_cond);
			},
			bnd_conds_);
	}

	template<class Func>
	void for_each_non_ess_bnd_cond(Func func) const
	{
		esu::tuple_for_each(
			[&func](const auto& bnd_cond) {
				using Bnd_cond = esu::Remove_cv_ref<decltype(*bnd_cond)>;

				assert(bnd_cond);
				if constexpr (!Bnd_cond::is_essential)
					func(*bnd_cond);
			},
			bnd_conds_);
	}

	const std::string& name() const
	{
		return name_;
	}

	void set_name(std::string name)
	{
		name_ = std::move(name);
	}

private:
	Bnd_conds_tuple bnd_conds_;
	std::string name_ = "<un-named>";
};
} // namespace esf::internal
