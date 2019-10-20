#pragma once

namespace esf
{
//////////////////////////////////////////////////////////////////////
/** Mesh element tags */

struct Vertex_tag
{};

struct Halfedge_tag
{};

struct Edge_tag
{};

struct Face_tag
{};

struct Cell_tag // Edge in 1D, Face in 2D
{};

//////////////////////////////////////////////////////////////////////
/** Circulation type tags */

struct Vertex_out_circ_tag
{};

struct Vertex_in_circ_tag
{};

struct Face_circ_tag
{};
} // namespace esf
