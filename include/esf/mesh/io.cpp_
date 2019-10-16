// std::unique_ptr<Mesh2> NetgenFileIO::read(const std::string& fileName, double scale)
// {
// 	std::fstream file(fileName);
// 	if (!file.is_open())
// 		throw util::FileReadOpenException(fileName);
// 
// 	std::unique_ptr<Mesh2> mesh = std::make_unique<Mesh2>();
// 
// 	Index n_vertices, n_cells;
// 
// 	file >> n_vertices;
// 	mesh->reserve(n_vertices, 3 * n_vertices, 0);
// 	for (Index i = 0; i < n_vertices; ++i)
// 	{
// 		double x, y;
// 		file >> x >> y;
// 		mesh->addVertex({x * scale, y * scale});
// 	}
// 
// 	file >> n_cells;
// 	mesh->reserve(0, 0, n_cells);
// 	for (Index i = 0; i < n_cells; ++i)
// 	{
// 		Index ignore, v1, v2, v3;
// 		file >> ignore >> v1 >> v2 >> v3;
// 		mesh->add_cell({v1 - 1, v2 - 1, v3 - 1});
// 	}
// 
// 	if (file.bad())
// 		throw util::BadMeshFileStructure(fileName);
// 
// 	return mesh;
// }
