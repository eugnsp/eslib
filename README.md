# ES libraries

:construction: Under construction.

## Synopsis

This is a collection of libraries that is being developed mostly for educational purposes.

### `esu`: utilities library

### `esl`: linear algebra library

- Dense matrices and vectors of strandard and user-defined types with static and dynamic extents
- Matrices with static extents can be used in `constexpr` context
- Sub-matrix (block) and transposed views
- Expression templates
- Sparse matrices with CSR storage scheme
- Some operations like `X = Y` or `X += Y` with `float`, `double`, `std::complex<float>`, and `std::complex<double>` matrix elements are mapped into MKL routines
- Export in MATLAB `.mat` and Gnuplot binary matrix files
- MKL inspector-executor sparse matrix wrapper
- MKL Pardiso linear solver and MKL Feast eigensolver wrappers

### `esf`: finite elements library

- 1D meshes
- 2D meshed based on half-edge datastructure
- Vertex, edge and face iterators and views
- 1D and 2D Lagrange elements of variable degree that is fixed at compile-time
- 2D&otimes;1D Lagrange elements
- Guass and Dunavant rules for itegration
- Matrix-based linear, non-linear and eigensolvers
- Mesh faces colouring and multi-threaded matrix assembly
- Import from Gmsh mesh files, export into VTK `.vtk` and Matlab `.mat` files

## Dependencies

- Intel MKL

Requires C++17 compiler. Tested with GCC 8.3.0 and Clang 7.0.0.

<!--
## Documentation

See [here](doc/README.md).
-->