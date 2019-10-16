#pragma once
#include <esl/dense/functor/fn_expr_loop.hpp>
#include <esl/dense/type_traits/mkl_decay.hpp>

namespace esl::internal
{
// Returns a functor for the X = Y operation (the default one uses two nested loops)
template<class Expr1, class Expr2, class Mkl_expr1, class Mkl_expr2, typename = void>
struct Fn_expr_assign_trait
{
	using Type = Fn_expr_assign_loop;
};

// Returns a functor for the X += Y operation (the default one uses two nested loops)
template<class Expr1, class Expr2, class Mkl_expr1, class Mkl_expr2, typename = void>
struct Fn_expr_add_assign_trait
{
	using Type = Fn_expr_add_assign_loop;
};

// Returns a functor for the X -= Y operation (the default one uses two nested loops)
template<class Expr1, class Expr2, class Mkl_expr1, class Mkl_expr2, typename = void>
struct Fn_expr_sub_assign_trait
{
	using Type = Fn_expr_sub_assign_loop;
};

template<class Expr1, class Expr2, template<class, class, class, class, typename = void> class Trait>
using Make_fn_expr_type = typename Trait<Expr1, Expr2, Mkl_expr_decay<Expr1>, Mkl_expr_decay<Expr2>>::Type;

//////////////////////////////////////////////////////////////////////

// Returns a functor for the X = Y operation
template<class Expr1, class Expr2>
using Fn_expr_assign_type = Make_fn_expr_type<Expr1, Expr2, Fn_expr_assign_trait>;

// Returns a functor for the X += Y operation
template<class Expr1, class Expr2>
using Fn_expr_add_assign_type = Make_fn_expr_type<Expr1, Expr2, Fn_expr_add_assign_trait>;

// Returns a functor for the X -= Y operation
template<class Expr1, class Expr2>
using Fn_expr_sub_assign_type = Make_fn_expr_type<Expr1, Expr2, Fn_expr_sub_assign_trait>;
} // namespace esl::internal
