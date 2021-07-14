#include "CppUTest/TestHarness.h"

#include <cmath>

#include "Circle.h"
#include "Polygon.h"
#include "Reuleaux.h"

TEST_GROUP(ReuleauxCircle)
{
};
//=======================================================================

TEST(ReuleauxCircle, Inside)
{// Inner is contained by outer
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  
  Point *vert = vertices;
  ReuleauxTriangle inner = ReuleauxTriangle(vert);

  Circle outer = Circle(Point(0.0,0.0), 5.0);
  CHECK(inner.ContainedBy(outer));
}

TEST(ReuleauxCircle, Outside)
{// Inner is entirely outside Outer (not contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle inner = ReuleauxTriangle(vert);
  Circle outer = Circle(Point(5.0,5.0), 1.0);
  CHECK(!inner.ContainedBy(outer));
}

TEST(ReuleauxCircle, Intersects) 
{// Inner and Outer intersect (not contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle inner = ReuleauxTriangle(vert);
  Circle outer = Circle(Point(3.0,3.0), 3.0);
  CHECK(!inner.ContainedBy(outer));
}

TEST(ReuleauxCircle, Surrounds) 
{// Inner surrounds Outer (not contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle inner = ReuleauxTriangle(vert);
  Circle outer = Circle(Point(0.0,0.0), 0.5);
  CHECK(!inner.ContainedBy(outer));
}

TEST(ReuleauxCircle, TouchingInside) 
{// Inner perimeter touches Outer perimeter (contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), 
    Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle inner = ReuleauxTriangle(vert);
  Circle outer = Circle(Point(0.0,0.0), 4.0);
  CHECK(inner.ContainedBy(outer));
}

TEST(ReuleauxCircle, TouchingOutside) 
{// Inner perimeter touches Outer perimeter (not contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle inner = ReuleauxTriangle(vert);
  Circle outer = Circle(Point(0.0,4.0), 2);
  CHECK(!inner.ContainedBy(outer));
}


//=======================================================================
TEST_GROUP(ReuleauxPolygon)
{
};
//=======================================================================

TEST(ReuleauxPolygon, Inside)
{// Inner is contained by outer
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle in = ReuleauxTriangle(vert);

  vector<Point> outer = {Point(4.0,-4.0), Point(-4.0,-4.0), Point(0.0,4.0)};
  Polygon out = Polygon(outer);
  CHECK(in.ContainedBy(out));
}

TEST(ReuleauxPolygon, Outside) 
{// Inner is entirely outside Outer (not contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle in = ReuleauxTriangle(vert);

  vector<Point> outer = {Point(6.0,3.0), Point(4.0,3.0), Point(5.0,5.0)};
  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

TEST(ReuleauxPolygon, Intersects) 
{// Inner and Outer intersect (not contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle in = ReuleauxTriangle(vert);

  vector<Point> outer = {Point(2.0,0.0), Point(0.0,-2.0), Point(-2.0,0.0)};

  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

TEST(ReuleauxPolygon, Surrounds) 
{// Inner surrounds Outer (not contained)
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle in = ReuleauxTriangle(vert);

  vector<Point> outer = {Point(1.0,0.0), Point(0.0,-1.0), Point(-1.0,0.0)};

  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

TEST(ReuleauxPolygon, TouchingInside) 
{
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle in = ReuleauxTriangle(vert);

  vector<Point> outer = {Point(0.0,2.0), Point(3.0,0.0), Point(-3.0,0.0)};

  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

TEST(ReuleauxPolygon, TouchingOutside) 
{
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle in = ReuleauxTriangle(vert);

  vector<Point> outer = {Point(2.0,2.0), Point(-2.0,2.0), Point(0.0,4.0)};

  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

//=======================================================================
TEST_GROUP(ReuleauxReuleaux)
{
};
//=======================================================================

TEST(ReuleauxReuleaux, Inside)
{// Inner is contained by outer
  Point vIn[3] = { Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.732)};
  Point *vertIn = vIn;
  ReuleauxTriangle in = ReuleauxTriangle(vertIn);

  Point vOut[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vertOut = vOut;
  ReuleauxTriangle out = ReuleauxTriangle(vertOut);

  CHECK(in.ContainedBy(out));
}

TEST(ReuleauxReuleaux, Outside)
{// Inner is entirely outside Outer (not contained)
  Point vIn[3] = {Point(2.0,4.0), Point(0.0,4.0), Point(1.0,5.732)};
  Point *vertIn = vIn;
  ReuleauxTriangle in = ReuleauxTriangle(vertIn);

  Point vOut[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vertOut = vOut;
  ReuleauxTriangle out = ReuleauxTriangle(vertOut);

  CHECK(!in.ContainedBy(out));
}

TEST(ReuleauxReuleaux, Intersects) 
{// Inner and Outer intersect (not contained)
  Point vIn[3] = {Point(0.0,0.0), Point(-2.0,0.0), Point(1.0,1.732)};
  Point *vertIn = vIn;
  ReuleauxTriangle in = ReuleauxTriangle(vertIn);

  Point vOut[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vertOut = vOut;
  ReuleauxTriangle out = ReuleauxTriangle(vertOut);

  CHECK(!in.ContainedBy(out));
}

TEST(ReuleauxReuleaux, Surrounds) 
{// Inner surrounds Outer (not contained)
  Point vIn[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vertIn = vIn;
  ReuleauxTriangle in = ReuleauxTriangle(vertIn);

  Point vOut[3] = { Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.732)};
  Point *vertOut = vOut;
  ReuleauxTriangle out = ReuleauxTriangle(vertOut);
  CHECK(!in.ContainedBy(out));
}

TEST(ReuleauxReuleaux, TouchingInside) 
{
  Point vIn[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vertIn = vIn;
  ReuleauxTriangle in = ReuleauxTriangle(vertIn);

  Point vOut[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vertOut = vOut;
  ReuleauxTriangle out = ReuleauxTriangle(vertOut);
  CHECK(in.ContainedBy(out));
}

TEST(ReuleauxReuleaux, TouchingOutside) 
{
  Point vIn[3] = { Point(0.0,2.0), Point(1.732,5.0), Point(3.464,2.0)};
  Point *vertIn = vIn;
  ReuleauxTriangle in = ReuleauxTriangle(vertIn);

  Point vOut[3] = { Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.732)};
  Point *vertOut = vOut;
  ReuleauxTriangle out = ReuleauxTriangle(vertOut);
  CHECK(!in.ContainedBy(out));
}
