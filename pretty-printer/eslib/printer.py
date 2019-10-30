import gdb
import itertools
import re
import sys
from enum import Enum

row_major = 0
col_major = 1

if sys.version_info[0] > 2:
	Iterator = object
else:
	class Iterator:
		def next(self):
			return self.__next__()

def get_array_element(arr, index):
	pointer = arr.cast(arr.type.target().pointer()) + index
	return pointer.dereference()

def get_vector_element(vec, index):
    type = gdb.types.get_basic_type(vec.type)
    return gdb.parse_and_eval('(*(%s*)(%s))[%d]' % (str(type), str(vec.address), index))

class esl_matrix_printer(object):
	class _iterator(Iterator):
		def __init__(self, mat):
			self.mat = mat
			self.index = 0

		def __iter__(self):
			return self

		def __next__(self):
			if self.index == self.mat.rows * self.mat.cols:
				raise StopIteration

			index = self.index
			self.index += 1

			if self.mat.layout == col_major:
				col = index / self.mat.rows
				row = index % self.mat.rows
			else:
				row = index / self.mat.cols
				col = index % self.mat.cols

			val = get_array_element(self.mat.val['data_']['data_'], index)
			if self.mat.is_vector:
				return ('[%d]' % row, val)
			else:
				return ('(%d, %d)' % (row, col), val)

	def __init__(self, val):
		self.val = val

		basic_type = gdb.types.get_basic_type(val.type)
		self.rows = int(basic_type.template_argument(1))
		self.cols = int(basic_type.template_argument(2))
		self.is_vector = self.cols == 1
		if not val['rows_'].is_optimized_out:
			self.rows = int(val['rows_'])
		if not val['cols_'].is_optimized_out:
			self.cols = int(val['cols_'])
		if 'Col' in str(basic_type.template_argument(3)):
			self.layout = col_major
		else:
			self.layout = row_major

	def children(self):
		return self._iterator(self)

	def to_string(self):
		return '%d x %d (%s)' % (self.rows, self.cols, "col-major" if self.layout == col_major else "row-major")

	def display_hint(self):
		return 'array'

class esf_point1_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		return '(%s)' % str(self.val['x_'])

class esf_point2_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		x = str(get_array_element(self.val['data_']['data_'], 0))
		y = str(get_array_element(self.val['data_']['data_'], 1))
		return '(%s, %s)' % (x, y)

class esf_mesh_elt_idx_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		idx_type = gdb.types.get_basic_type(self.val.type)
		idx_type_name = str(idx_type)
		name = re.sub('.*::(.*)_.*', '\\1', idx_type_name)
		value = self.val.cast(gdb.lookup_type('esf::Index'))

		if value != idx_type['%s::invalid' % idx_type_name].enumval:
			return '%s {%s}' % (name, str(value))
		else:
			return '%s {invalid}' % name

class esf_mesh2_vertex_view_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		index = self.val['index_']
		return '%s' % index

class esf_mesh2_halfedge_view_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		index = self.val['index_']
		halfedges = self.val['mesh_']['halfedges_']
		vertex_from = get_vector_element(halfedges, int(index))['vertex']
		vertex_to = get_vector_element(halfedges, int(index) ^ 1)['vertex']
		return '%s: %s -> %s' % (index, vertex_from, vertex_to)

# class esf_test_printer(object):
# 	def __init__(self, val):
# 		self.val = val

# 	def to_string(self):
# 		type = gdb.types.get_basic_type()
# 		return str(self.val.type)

def build_pretty_printer():
	printer = gdb.printing.RegexpCollectionPrettyPrinter('eslib')

	# esl
	printer.add_printer('esl::Matrix', '^esl::Matrix', esl_matrix_printer)

	# esf
	printer.add_printer('esf::Point1', '^esf::Point1$', esf_point1_printer)
	printer.add_printer('esf::Point2', '^esf::Point2$', esf_point2_printer)
	printer.add_printer('esf::Mesh_element_index', '^esf::(Vertex|Halfedge|Edge|Face)_index_wrapper::(\\1)_index$', esf_mesh_elt_idx_printer)
	printer.add_printer('esf::Mesh2::Vertex_view', '^esf::Element_view<esf::Vertex_tag,\s*esf::Mesh<esf::Dim2>\s*>$', esf_mesh2_vertex_view_printer)
	printer.add_printer('esf::Mesh2::Halfedge_view', '^esf::Element_view<esf::Halfedge_tag,\s*esf::Mesh<esf::Dim2>\s*>$', esf_mesh2_halfedge_view_printer)

	return printer

def register_printers():
	gdb.printing.register_pretty_printer(gdb.current_objfile(), build_pretty_printer())
