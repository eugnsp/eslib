#pragma once

namespace esf
{
//////////////////////////////////////////////////////////////////////
/** Geometry tags */
struct Polygon_tag
{};
struct Linestring_tag
{};
struct Rect_tag
{};
struct Triangle_tag : Polygon_tag
{};
struct Segment_tag
{};
struct Segment1_tag : Segment_tag
{};
struct Segment2_tag : Segment_tag
{};
struct Points2_tag
{};
struct Point_tag
{};
struct Point1_tag : Point_tag
{};
struct Point2_tag : Point_tag
{};

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
