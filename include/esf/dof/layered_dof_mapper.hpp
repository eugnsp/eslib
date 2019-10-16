#pragma once
#include <esf/dof/dof_index.hpp>
#include <esf/dof/dof_mapper_base.hpp>
#include <esf/type_traits.hpp>
#include <esf/types.hpp>
#include <esf/util/mesh_vars_map.hpp>
#include <esf/var_list.hpp>

#include <vector>

template<class>
class TD;
template<bool>
class TB;

namespace esf
{
template<class Var_list>
class Layered_dof_mapper : public internal::Dof_mapper_base<Var_list>
{
private:
	using Base = internal::Dof_mapper_base<Var_list>;

	using My_system = System<Var_list, esf::Layered_dof_mapper>;
	using Mesh = typename My_system::Mesh;

	using Cell_view = typename Mesh::Cell_view;

	template<std::size_t var>
	using Var_t = typename Var_list::template Nth<var>;

public:
	template<std::size_t var_index>
	using Var_dofs = typename Base::template Var_dofs<var_index>;

	using typename Base::Vars_dofs;

	template<std::size_t var_index>
	using Var_vertex_dofs = typename Base::template Var_vertex_dofs<var_index>;

	template<std::size_t var_index>
	using Var_edge_dofs = typename Base::template Var_edge_dofs<var_index>;

public:
	template<std::size_t var>
	Var_dofs<var> dofs(const Cell_view& cell, Index layer) const
	{
		assert(layer < n_layers_);

		Var_dofs<var> dofs;
		var_dofs<var>(cell, layer, dofs);
		return dofs;
	}

	Vars_dofs all_dofs(const Cell_view& cell, Index layer) const
	{
		assert(layer < n_layers_);

		Vars_dofs dofs_list;
		dofs2(cell, layer, dofs_list);
		return dofs_list;
	}

	void all_dofs(const Cell_view& cell, std::vector<Vars_dofs>& dofs) const
	{
		assert(dofs.size() == n_layers_);

		for (Index layer = 0; layer < n_layers_; ++layer)
			dofs2(cell, layer, dofs[layer]);
	}

	template<std::size_t var>
	void vertex_dofs(Vertex_index vertex, Var_vertex_dofs<var>& dofs_list, Index layer) const
	{
		assert(layer < n_layers_);
		var_vertex_dofs<var>(vertex, layer, dofs_list);
	}

	template<std::size_t var>
	void edge_dofs(Edge_index edge, Var_edge_dofs<var>& dofs_list, Index layer) const
	{
		assert(layer < n_layers_);
		var_edge_dofs<var>(edge, layer, dofs_list);
	}

	template<class Active_cell_predicate>
	void init(const My_system& system, Index n_layers, Active_cell_predicate is_active_cell)
	{
		n_layers_ = n_layers;

		auto& mesh = system.mesh();
		auto& vars = system.variables();

		// Count DoFs
		std::vector<bool> is_active_vertex(n_layers_ * *mesh.n_vertices(), false);
		std::vector<bool> is_active_edge(n_layers_ * *mesh.n_edges(), false);
		std::vector<bool> is_active_face(n_layers_ * *mesh.n_faces(), false);

		for (auto& cell : mesh.cells())
			for (Index layer = 0; layer < n_layers_; ++layer)
			{
				if (!is_active_cell(cell, layer))
					continue;

				std::array<Vertex_index, 3> vertices;
				std::array<Edge_index, 3> edges;
				cell.get_indices(vertices, edges);

				for (const auto v : vertices)
					is_active_vertex[*v + layer * *mesh.n_vertices()] = true;

				for (const auto e : edges)
					is_active_edge[*e + layer * *mesh.n_edges()] = true;

				is_active_face[**cell + layer * *mesh.n_faces()] = true;
			}

		const auto active_vertex_dofs =
			std::count(is_active_vertex.begin(), is_active_vertex.end(), true);
		const auto active_edge_dofs =
			std::count(is_active_edge.begin(), is_active_edge.end(), true);
		const auto active_face_dofs =
			std::count(is_active_face.begin(), is_active_face.end(), true);

		this->n_dofs_ = 0;
		for_each_var<Var_list>(
			[this, &system, active_vertex_dofs, active_edge_dofs, active_face_dofs](auto var) {
				this->n_dofs_ += static_cast<Index>(
					system.variable(var).n_dofs(Vertex_tag{}) * active_vertex_dofs);
				this->n_dofs_ +=
					static_cast<Index>(system.variable(var).n_dofs(Edge_tag{}) * active_edge_dofs);
				this->n_dofs_ +=
					static_cast<Index>(system.variable(var).n_dofs(Face_tag{}) * active_face_dofs);
			});

		// Assign indices
		this->indices_.init_storage(mesh, Dof_index::invalid_free(), n_layers_);

		// Mark const dofs
		this->n_free_dofs_ = this->n_dofs_;
		for_each_var<Var_list>([&](auto var) {
			const auto& v = system.variable(var);
			v.for_each_ess_bnd_cond([this, &mesh, &is_active_vertex, &is_active_edge,
										&is_active_face, &v, var](auto& bc) {
				using Element = typename Var_by_var_index<Var_list, decltype(var)>::Element;

				if constexpr (Element::has_vertex_dofs)
					for (Index layer = 0; layer < n_layers_; ++layer)
						for (Vertex_index vertex : bc.vertices(layer))
						{
							if (!is_active_vertex[*vertex + layer * *mesh.n_vertices()])
								continue;

							Dof_index& dof = this->indices_.at(layer, vertex, var);
							assert(dof.is_free);

							this->n_free_dofs_ -= v.n_dofs(Vertex_tag{});
							dof.is_free = false;
						}

				if constexpr (Element::has_edge_dofs)
					for (Index layer = 0; layer < n_layers_; ++layer)
						for (Halfedge_index halfedge : bc.halfedges(layer))
						{
							if (!is_active_edge[*edge(halfedge) + layer * *mesh.n_edges()])
								continue;

							Dof_index& dof = this->indices_.at(layer, edge(halfedge), var);
							assert(dof.is_free);

							this->n_free_dofs_ -= v.n_dofs(Edge_tag{});
							dof.is_free = false;
						}
			});
		});

		Index free_index = 0;
		Index const_index = this->n_free_dofs_;

		for_each_var_element<Var_list>([&](auto var, auto tag) {
			auto& v = system.variable(var);

			for (internal::Element_index_by_tag<decltype(tag)> element{0};
				 element < system.mesh().n_elements(tag); ++element)
				for (Index layer = 0; layer < n_layers_; ++layer)
				{
					if constexpr (std::is_same_v<decltype(tag), Vertex_tag>)
						if (!is_active_vertex[*element + layer * *mesh.n_vertices()])
							continue;

					if constexpr (std::is_same_v<decltype(tag), Edge_tag>)
						if (!is_active_edge[*element + layer * *mesh.n_edges()])
							continue;

					if constexpr (std::is_same_v<decltype(tag), Face_tag>)
						if (!is_active_face[*element + layer * *mesh.n_faces()])
							continue;

					Dof_index& dof = this->indices_.at(layer, element, var);

					auto& index = dof.is_free ? free_index : const_index;
					dof.index = index;
					index += v.n_dofs(tag);
				}
		});

		// 		assert(indices_.all_of([](auto& dof_index) { return dof_index.is_valid(); }));
	}

	template<class Symmetry_tag, class Couplig_func>
	esl::Sparsity_pattern<Symmetry_tag> sparsity_pattern(
		const My_system& system, Couplig_func coupling) const
	{
		esl::Sparsity_pattern<Symmetry_tag> pattern(this->n_free_dofs_);

		std::vector<std::size_t> cols;
		for (auto& cell : system.mesh().faces())
		{
			std::vector<Vars_dofs> dofs(n_layers_);
			all_dofs(cell, dofs);

			for_each_var<Var_list>([&](auto var1) {
				const auto& v1 = system.variable(var1);

				for (Local_index dim1 = 0; dim1 < v1.dim(); ++dim1)
					for (Index layer1 = 0; layer1 < n_layers_; ++layer1)
					{
						const auto& dofs1 = std::get<var1>(dofs[layer1]);

						cols.clear();
						for_each_var<Var_list>([&](auto var2) {
							const auto& v2 = system.variable(var2);

							// TODO : O(n^2) -> O(n) with iterators or sparse matrices

							for (Local_index dim2 = 0; dim2 < v2.dim(); ++dim2)
								for (Index layer2 = 0; layer2 < n_layers_; ++layer2)
									if (coupling(var1, var2, dim1, dim2, layer1, layer2))
									{
										// TODO : break if is_valid() returns false (other dofs are
										// also invalid)

										const auto& dofs2 = std::get<var2>(dofs[layer2]);
										for (std::size_t i = 0; i < dofs2.size(); ++i)
											if (dofs2[i].is_valid() && dofs2[i].is_free)
												cols.push_back(dofs2[i].index + dim2);
									}
						});

						std::sort(cols.begin(), cols.end());
						for (std::size_t i = 0; i < dofs1.size(); ++i)
							if (dofs1[i].is_valid() && dofs1[i].is_free)
							{
								const auto row = dofs1[i].index + dim1;
								if constexpr (std::is_same_v<Symmetry_tag, esl::Symmetric_upper>)
								{
									const auto pos =
										std::lower_bound(cols.begin(), cols.end(), row);
									pattern.insert(row, pos, cols.end());
								}
								else
									pattern.insert(row, cols.begin(), cols.end());
							}
					}
			});

			// for_each_var<Var_list>(
			//	[&](auto var1)
			//	{
			//		const auto& v1 = system.variable(var1);

			//		for_each_var<Var_list>(
			//			[&](auto var2)
			//			{
			//				const auto& v2 = system.variable(var2);

			//				for (auto c : coupling(var1, var2))
			//				{
			//					// c.first = dimension
			//					// c.second
			//				}
			//			});
			//	});
		}

#ifndef NDEBUG
		assert(pattern.is_each_row_not_empty());
		auto err = pattern.check();
		if (err)
			throw std::runtime_error(err.message());
#endif

		return pattern;
	}

	template<class Symmetry_tag, class Couplig_func>
	esl::Sparsity_pattern<Symmetry_tag> sparsity_pattern2(
		const My_system& system, Couplig_func coupling) const
	{
		esl::Sparsity_pattern<Symmetry_tag> pattern(this->n_free_dofs_);
		std::vector<Vars_dofs> dofs(n_layers_);
		std::vector<std::size_t> cols;

		for (auto& cell : system.mesh().faces())
		{
			all_dofs(cell, dofs);

			const auto ins = [&coupling, &pattern, &dofs, &cols](auto var1, auto var2) {
				for (auto c = coupling.begin(var1, var2); c != coupling.end(var1, var2); ++c)
				{
					auto& c1 = std::get<0>(*c);
					auto& c2 = std::get<1>(*c);

					const auto& dofs1 = std::get<var1>(dofs[c1.layer]);
					const auto& dofs2 = std::get<var2>(dofs[c2.layer]);

					cols.clear();
					for (std::size_t j = 0; j < dofs2.size(); ++j)
						if (dofs2[j].is_valid() && dofs2[j].is_free)
							cols.push_back(dofs2[j].index + c2.dim);

					if (cols.empty())
						continue;

					std::sort(cols.begin(), cols.end());
					for (std::size_t i = 0; i < dofs1.size(); ++i)
						if (dofs1[i].is_valid() && dofs1[i].is_free)
						{
							const auto row = dofs1[i].index + c1.dim;
							if constexpr (std::is_same_v<Symmetry_tag, esl::Symmetric_upper>)
							{
								const auto pos = std::lower_bound(cols.begin(), cols.end(), row);
								pattern.insert(row, pos, cols.end());
							}
							else
								pattern.insert(row, cols.begin(), cols.end());
						}
				}
			};

			for_each_var<Var_list>(
				[&](auto var1) { for_each_var<Var_list>([&](auto var2) { ins(var1, var2); }); });
		}

		assert(pattern.is_each_row_not_empty());
		assert(!pattern.check());

		return pattern;
	}

private:
	void compute_n_dofs()
	{}

	template<std::size_t var, class Dofs>
	void var_dofs(const Cell_view& cell, Index layer, Dofs& dofs) const
	{
		typename Cell_view::Vertex_indices vertices;
		typename Cell_view::Edge_with_dir_indices edges;
		cell.get_indices(vertices, edges);

		var_dofs_impl<var>(vertices, edges, *cell, layer, dofs);
	}

	template<class Dofs>
	void dofs2(const Cell_view& cell, Index layer, Dofs& dofs) const
	{
		dofs_impl(cell, layer, dofs, std::make_index_sequence<Var_list::size>{});
	}

	template<class Dofs, std::size_t... vars>
	void dofs_impl(const typename Mesh::Cell_view& cell, Index layer, Dofs& dofs,
		std::index_sequence<vars...>) const
	{
		typename Cell_view::Vertex_indices vertices;
		typename Cell_view::Edge_with_dir_indices edges;
		cell.get_indices(vertices, edges);

		(var_dofs_impl<vars>(vertices, edges, *cell, layer, std::get<vars>(dofs)), ...);
	}

	template<std::size_t var, class Dofs>
	void var_dofs_impl(const typename Cell_view::Vertex_indices& vertices,
		const typename Cell_view::Edge_with_dir_indices& edges, Face_index cell, Index layer,
		Dofs& dofs) const // TODO Cell_index
	{
		using Var = Var_t<var>;

		// TODO : replace indexing with iterator (when iterators are ready in esl::)
		std::size_t i = 0;
		auto first_degree_dofs = dofs.template col<0>();
		if constexpr (Var::Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				var_dofs_impl2<Vertex_tag, var>(layer, vertex, first_degree_dofs, i);

		if constexpr (Var::Element::has_edge_dofs)
			for (const auto& edge : edges)
				var_dofs_impl2<Edge_tag, var>(layer, edge.first, first_degree_dofs, i, edge.second);

		if constexpr (Var::Element::has_face_dofs)
			var_dofs_impl2<Face_tag, var>(layer, cell, first_degree_dofs, i);

		assert(i == dofs.size());
	}

	template<class T_Tag, std::size_t var, class Dofs>
	void var_dofs_impl2(Index layer, internal::Element_index_by_tag<T_Tag> element, Dofs& dofs,
		std::size_t& i, bool reversed = false) const
	{
		using Element = typename Var_t<var>::Element;

		const Dof_index& first_dof = this->indices_.at(layer, element, Var_index<var>{});

		// constexpr auto n = _System::template VarType<_index>::Element::dofs(_Tag{ });
		// for (LocalIndexType k = 0; k < n; ++k)
		//	*it++ = firstDofIndex + (!reversed ? k : static_cast<LocalIndexType>(n - k - 1));

		constexpr auto n = Element::n_dofs(T_Tag{});
		for (Index k = 0; k < n; ++k)
			dofs[i++] = first_dof + (reversed ? n - k - 1 : k);
	}

	template<std::size_t var, class Dofs>
	void var_vertex_dofs(Vertex_index vertex, Index layer, Dofs& dofs) const
	{
		static_assert(Var_t<var>::Element::has_dofs(Vertex_tag{}));

		const Dof_index& first_dof = this->indices_.at(layer, vertex, Var_index<var>{});

		// resize?
		for (std::size_t j = 0; j < dofs.cols(); ++j)
			for (std::size_t i = 0; i < dofs.rows(); ++i)
				dofs(i, j) = first_dof + static_cast<Index>(i + j * dofs.rows());
	}

	template<std::size_t var, class Dofs>
	void var_edge_dofs(Edge_index edge, Index layer, Dofs& dofs) const
	{
		static_assert(Var_t<var>::Element::has_dofs(Edge_tag{}));

		const Dof_index& first_dof = this->indices_.at(layer, edge, Var_index<var>{});

		// resize?
		for (std::size_t j = 0; j < dofs.cols(); ++j)
			for (std::size_t i = 0; i < dofs.rows(); ++i)
				dofs(i, j) = first_dof + static_cast<Index>(i + j * dofs.rows());
	}

private:
	Index n_layers_ = 1;
};
} // namespace esf
