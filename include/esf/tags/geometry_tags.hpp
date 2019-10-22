#pragma once

namespace esf
{
struct Polygon_tag {};

struct Linestring_tag {};

struct Rect_tag {};

struct Triangle_tag : Polygon_tag {};

struct Segment_tag {};
struct Segment1_tag : Segment_tag {};
struct Segment2_tag : Segment_tag {};

struct Points2_tag {};

struct Point_tag {};
struct Point1_tag : Point_tag {};
struct Point2_tag : Point_tag {};
} // namespace esf
