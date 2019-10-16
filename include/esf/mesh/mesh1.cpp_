// #include <es/fe/mesh/mesh1.hpp>
// #include <es_util/function.hpp>
// #include <ostream>
// #include <cassert>
// 
// namespace fe
// {
// Mesh1::Mesh1(double x, const geom::Linear_grid& y_grid)
// {
// 	for (const double y : y_grid.grid())
// 		vertices_.push_back({x, y});
// }
// 
// Mesh1::Mesh1(const geom::Linear_grid& x_grid, double y)
// {
// 	for (const double x : x_grid.grid())
// 		vertices_.push_back({x, y});
// }
// 
// Index Mesh1::n_vertices() const
// {
// 	return (Index)vertices_.size();
// }
// 
// Index Mesh1::n_edges() const
// {
// 	return n_vertices() - 1;
// }
// 
// Index Mesh1::n_cells() const
// {
// 	return n_edges();
// }
// 
// Index Mesh1::n_elements(Vertex_tag) const
// {
// 	return n_vertices();
// }
// 
// Index Mesh1::n_elements(Edge_tag) const
// {
// 	return n_cells();
// }
// 
// void Mesh1::reserve(Index n_vertices)
// {
// 	vertices_.reserve(n_vertices);
// }
// 
// std::size_t Mesh1::memory_size() const
// {
// 	return vertices_.capacity() * sizeof(vertices_[0]);
// }
// 
// const geom::Point& Mesh1::vertex(Index index) const
// {
// 	return vertices_[index];
// }
// 
// void Mesh1::rescale(double x)
// {
// 	for (auto& vertex : vertices_)
// 		vertex *= x;
// }
// 
// bool Mesh1::is_empty() const
// {
// 	return vertices_.empty();
// }
// 
// bool Mesh1::is_bnd_vertex(Index index) const
// {
// 	return index == 0 || index == n_vertices() - 1;
// }
// 
// bool Mesh1::is_bnd_cell(Index index) const
// {
// 	return index == 0 || index == n_cells() - 1;
// }
// 
// es::util::Error Mesh1::is_ok() const
// {
// 	es::util::Error err("Mesh check");
// 
// 	if (n_vertices() == 0)
// 		err << es::util::error_flag << "The mesh contains no vertices";
// 
// 	return err;
// }
// 
// //////////////////////////////////////////////////////////////////////////
// /** Views */
// 
// auto Mesh1::vertex_view(Index index) const -> Vertex_view
// {
// 	assert(index < n_vertices());
// 	return Vertex_view{*this, index};
// }
// 
// auto Mesh1::edge_view(Index index) const -> Edge_view
// {
// 	assert(index < n_edges());
// 	return Edge_view{*this, index};
// }
// 
// //////////////////////////////////////////////////////////////////////////
// /** Iterators */
// 
// auto Mesh1::begin_vertex() const -> Vertex_iter
// {
// 	return {*this, 0};
// }
// 
// auto Mesh1::end_vertex() const -> Vertex_iter
// {
// 	return {*this, n_vertices()};
// }
// 
// auto Mesh1::vertices() const -> es::util::Iterable<Vertex_iter>
// {
// 	return {begin_vertex(), end_vertex()};
// }
// 
// auto Mesh1::begin_edge() const -> Edge_iter
// {
// 	return {*this, 0};
// }
// 
// auto Mesh1::end_edge() const -> Edge_iter
// {
// 	return {*this, n_cells()};
// }
// 
// auto Mesh1::edges() const -> es::util::Iterable<Edge_iter>
// {
// 	return {begin_edge(), end_edge()};
// }
// 
// auto Mesh1::cells() const -> es::util::Iterable<Cell_iter>
// {
// 	return edges();
// }
// 
// std::string Mesh1::type_string() const
// {
// 	return "1D mesh";
// }
// 
// void Mesh1::clear()
// {
// 	vertices_.clear();
// }
// 
// // 	Index Mesh1::add_vertex(double vertex)
// // 	{
// // 		vertices_.push_back(vertex);
// // 		return n_vertices() - 1;
// // 	}
// 
// std::ostream& operator<<(std::ostream& ost, const Mesh1& mesh)
// {
// 	ost << "Mesh information" << std::endl
// 		<< "-----------------------------------------------------------" << std::endl
// 		<< "Type: " << mesh.type_string() << std::endl
// 		<< "Vertices: " << mesh.n_vertices() << ", edges: " << mesh.n_cells() << std::endl
// 		<< "Memory: " << es::util::size_string(mesh.memory_size()) << std::endl
// 		<< "-----------------------------------------------------------" << std::endl;
// 
// 	return ost;
// }
// }