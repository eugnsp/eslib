#pragma once
#include <esf/index.hpp>
#include <esf/io/text_file_writer.hpp>
#include <esf/mesh/mesh2.hpp>

#include <cassert>
#include <fstream>
#include <string>

namespace esf
{
class Gnuplot_writer2
{
public:
	Gnuplot_writer2(const std::string& file_name, const Mesh2& mesh) :
		writer_(file_name), mesh_(mesh)
	{}

	template<class Vector>
	void write_vertex_field(const Vector& values)
	{
		assert(values.size() == *mesh_.n_vertices());

		for (auto& face : mesh_.faces())
		{
			auto circ = face.vertex_circ();
			[[maybe_unused]] const auto first = circ;

			write(circ->vertex(), values[***circ]);
			++circ;
			write(circ->vertex(), values[***circ]);
			//file_ << '\n';
			++circ;
			write(circ->vertex(), values[***circ]);
			//write(circ->vertex(), values[***circ]);

			assert(++circ == first);

			writer_.write_ln();
			writer_.write_ln();
		}
	}

	template<class Vector>
	void write_face_field(const Vector& values)
	{
		assert(values.size() == *mesh_.n_faces());

		for (auto& face : mesh_.faces())
		{
			auto circ = face.vertex_circ();
			[[maybe_unused]] const auto first = circ;

			write(circ++->vertex(), values[**face]);
			write(circ++->vertex(), values[**face]);
			writer_.write_ln();
			write(circ->vertex(), values[**face]);
			write(circ->vertex(), values[**face]);

			assert(++circ == first);

			writer_.write_ln();
			writer_.write_ln();
		}
	}

private:
	template<typename T>
	void write(Point2 pt, T value)
	{
		pt *= mesh_.output_scale();
		writer_.write_ln(pt.x(), '\t', pt.y(), '\t', value);
	}

private:
	internal::Text_file_writer writer_;
	const Mesh2& mesh_;
};
} // namespace esf
