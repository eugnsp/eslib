// #include "matfile_output.hpp"
// #include <es/fe/mesh/base.hpp>
// #include <cassert>
//
// namespace fe
// {
// void MATFileOutput::writeDataOnCells(const std::string& name, const LA::VectorX<double>& data,
// double scale)
// {
// 	assert(data.size() == mesh_.n_cells());
//
// 	LA::VectorX<double> tmp(mesh_.n_vertices(), 0);
// 	LA::VectorX<mesh::Local_index> norm(mesh_.n_vertices(), 0);
//
// 	for (auto& cell : mesh_.faces())
// 	{
// 		//std::array<mesh::Index, 3> vertices;
// 		std::vector<mesh::Index> vertices;
// 		cell.vertex_indices(vertices);
//
// 		for (mesh::Local_index r = 0; r < 3; ++r)
// 		{
// 			tmp[vertices[r]] += data[*cell];
// 			++norm[vertices[r]];
// 		}
// 	}
//
// 	for (std::size_t i = 0; i < mesh_.n_vertices(); ++i)
// 		tmp[i] /= (norm[i] * scale);
//
// 	file_.write(name, tmp);
// }
//
// void MATFileOutput::_writeDataOnEdges(const std::string& name, const LA::VectorX<double>& data,
// double scale)
// {
// 	assert(data.size() == mesh_.n_edges());
//
// 	file_.write(name, data);
//
// 	// Cells
// 	math::LA::MatrixX<int> edges(2, mesh_.n_edges());
//
// 	for (auto edge = mesh_.begin_edge(); edge != mesh_.end_edge(); ++edge)
// 		for (mesh::Index r = 0; r < 2; ++r)
// 			edges(r, **edge) = edge->vertex_index(r) + 1;
//
// 	file_.write("edges", edges);
// }
// }

// template<class MESH>
// FeMATFileOutput<MESH>::FeMATFileOutput(const std::string& _name, const MESH& _mesh, double
// _meshLengthScale /* = 1 */) 	: FeFileOutput(_mesh, _meshLengthScale), file(_name)
// {
// }
//
// 	file.write("nodes", nodes);
//
// 	// Elements
// 	Math::LA::MatrixX<int> elements(MESH::NumberOfNodesPerElement, mesh.n_cells());
//
// 	for (auto element = mesh.GetCellView(); element.isValid(); ++element)
// 		for (unsigned int i = 0; i < MESH::NumberOfNodesPerElement; ++i)
// 			elements(i, *element) = element[i] + 1;
//
// 	file.write("elements", elements);
// }
//
// template<class MESH>
// void FeMATFileOutput<MESH>::PutScalarDataOnNodes(const std::string& name, const
// Math::LA::VectorX<double>& data)
// {
// 	assert(data.size() == mesh.n_vertices());
//
// 	if (!IsInitialized())
// 		Init();
//
// 	file.write(name, data);
// }
//
// template<class MESH>
// void FeMATFileOutput<MESH>::PutScalarDataOnNodes(const std::string& name,
// Math::LA::VectorX<double> data, double scale)
// {
// 	assert(data.size() == mesh.n_vertices());
//
// 	data *= (1 / scale);
// 	PutScalarDataOnNodes(name, data);
// }
//
// template<class MESH>
// void FeMATFileOutput<MESH>::PutScalarDataOnElements(const std::string& name, const
// Math::LA::VectorX<double>& data, double scale /* = 1 */)
// {
// 	assert(data.size() == mesh.n_cells());
//
// 	if (!IsInitialized())
// 		Init();
//
// 	Math::LA::VectorX<double> tmp(mesh.n_vertices(), 0);
//
// 	for (auto cell = mesh.GetCellView(); cell.isValid(); ++cell)
// 		for (unsigned int r = 0; r < 3; ++r)
// 			tmp[cell[r]] += data[cell.GetIndex()] / mesh.GetNumberOfAdjacentCells(cell[r]) / scale;
//
// 	file.write(name, tmp);
// }
//
// //template<class MESH>
// //void FeMATFileOutput<MESH>::PutScalarDataOnNodes(const std::string& _name, const
// FeLinearSolutionView<MESH, double>& _data, double _scale /* = 1 */)
// //{
// //	assert(_data.GetSize() == mesh.n_vertices());
// //
// //	if (!IsInitialized())
// //		Init();
// //
// //	VectorX<double> data(mesh.n_vertices());
// //	for (unsigned int i = 0; i < mesh.n_vertices(); ++i)
// //		data[i] = _data[i] / _scale;
// //
// //	file.putVar(_name, data);
// //}
//
// //template<class MESH>
// //void FeMATFileOutput<MESH>::PutScalarDataOnNodes(const std::string& _name, const
// DataOnNodesViewType<double>& _data, double _scale /* = 1 */)
// //{
// //	if (!IsInitialized())
// //		Init();
// //
// //	VectorX<double> data(mesh.n_vertices());
// //	for (unsigned int i = 0; i < mesh.n_vertices(); ++i)
// //		data[i] = _data(i) / _scale;
// //
// //	file.putVar(_name, data);
// //}
//
// //void FeMATFileOutput::PutScalarDataOnNodes(const std::string& _name, const
// FeEigenSolutionView<double>& _data, double _scale /* = 1 */)
// //{
// //	assert(IsInitialized());
// //	//assert(_data.GetSize() == mesh.n_vertices());
// //
// //	VectorX<double> eigenValues(_data.GetNumberOfEigenValues());
// //	VectorX<double> eigenVector(mesh.n_vertices());
// //
// //	for (unsigned int eigenPairIndex = 0; eigenPairIndex < _data.GetNumberOfEigenValues();
// ++eigenPairIndex)
// //	{
// //		eigenValues[eigenPairIndex] = _data.GetEigenvalue(eigenPairIndex) / _scale;
// //
// //		for (unsigned int i = 0; i < mesh.n_vertices(); ++i)
// //			eigenVector[i] = _data.GetEigenvector(eigenPairIndex, i);
// //
// //		file.putVar(_name + std::to_string(eigenPairIndex), eigenVector);
// //	}
// //
// //	file.putVar(_name, eigenValues);
// //}
// //
// //void FeMATFileOutput::_PutScalarDataOnNodes(const std::string& _name, const
// FeEigenSolutionView<double>& _data, double _scale /* = 1 */)
// //{
// //	assert(IsInitialized());
// //	//assert(_data.GetSize() == mesh.n_vertices());
// //
// //	VectorX<double> dens(mesh.n_vertices(), 0);
// //
// //	for (unsigned int eigenPairIndex = 0; eigenPairIndex < _data.GetNumberOfEigenValues();
// ++eigenPairIndex)
// //	{
// //		for (unsigned int i = 0; i < mesh.n_vertices(); ++i)
// //			dens[i] += _data.GetEigenvector(eigenPairIndex, i) *
// _data.GetEigenvector(eigenPairIndex, i)
// *
// //			SpecialFunctions::FermiDiracMinusHalf(-_data.GetEigenvalue(eigenPairIndex) /
// .00095043);
// //	}
// //
// //	file.putVar(_name, dens);
// //}
//
// // Explicit instantiation
// //template class FeMATFileOutput<LinMesh>;
// template class FeMATFileOutput<Math::mesh::Mesh2>;
