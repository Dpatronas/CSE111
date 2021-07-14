
#include "CppUTest/TestHarness.h"

#include "Circle.h"
#include "Polygon.h"
#include "Reuleaux.h"

TEST_GROUP(PolygonCircle)
{
};
//=======================================================================

TEST(PolygonCircle, Inside)
{// Inner is contained by outer
  vector<Point> inner = {Point(5.0,-5.0), Point(-5.0,-5.0), Point(0.0,5.0)};
  Polygon in = Polygon(inner);
  Circle out = Circle(Point(0.0,0.0),20.0);
  CHECK(in.ContainedBy(out));
}

TEST(PolygonCircle, Outside) 
{// Inner is entirely outside Outer (not contained)
  vector<Point> inner = {Point(5.0,-5.0), Point(-5.0,-5.0), Point(0.0,5.0)};
  Polygon in = Polygon(inner);
  Circle out = Circle(Point(20.0,20.0),1.0);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonCircle, Intersects) 
{// Inner and Outer intersect (not contained)
  vector<Point> inner = {Point(5.0,-5.0), Point(-5.0,-5.0), Point(0.0,5.0)};
  Polygon in = Polygon(inner);
  Circle out = Circle(Point(5.0,5.0),1.0);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonCircle, Surrounds)
{// Inner surrounds Outer (not contained)
  vector<Point> inner = {Point(5.0,-5.0), Point(-5.0,-5.0), Point(0.0,5.0)};
  Polygon in = Polygon(inner);
  Circle out = Circle(Point(0.0,0.0),1.0);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonCircle, TouchingInside)
{
  vector<Point> inner = {Point(1.0,0.0), Point(0.0,0.0), Point(0.0,0.5)};
  Polygon in = Polygon(inner);
  Circle out = Circle(Point(0.0,0.0),1.0);
  CHECK(in.ContainedBy(out));
}

TEST(PolygonCircle, TouchingOutside)
{
  vector<Point> inner = {Point(1.0,1.0), Point(-1.0,1.0), Point(0.0,2.0)};
  Polygon in = Polygon(inner);
  Circle out = Circle(Point(0.0,0.0),1.0);
  CHECK(!in.ContainedBy(out));
}

//=======================================================================
TEST_GROUP(PolygonPolygon)
{
};
//=======================================================================

TEST(PolygonPolygon, Inside)
{// Inner is contained by outer
  vector<Point> inner = {Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.0)};
  vector<Point> outer = {Point(4.0,-2.0), Point(-4.0,-2.0), Point(0.0,4.0)};
  
  Polygon in = Polygon(inner);
  Polygon out = Polygon(outer);
  CHECK(in.ContainedBy(out));
}

TEST(PolygonPolygon, Outside) 
{// Inner is entirely outside Outer (not contained)
  vector<Point> inner = {Point(2.0,4.0), Point(-2.0,4.0), Point(0.0,6.0)};
  vector<Point> outer = {Point(2.0,0.0), Point(0.0,-2.0), Point(-2.0,0.0), 
    Point(-1.0,2.0), Point(1.0,2.0)};

  Polygon in = Polygon(inner);
  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonPolygon, Intersects) 
{// Inner and Outer intersect (not contained)
  vector<Point> inner = {Point(2.0,1.0), Point(-2.0,1.0), Point(0.0,4.0)};
  vector<Point> outer = {Point(2.0,0.0), Point(0.0,-2.0), Point(-2.0,0.0), 
    Point(-1.0,2.0), Point(1.0,2.0)};

  Polygon in = Polygon(inner);
  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonPolygon, Surrounds) 
{// Inner surrounds Outer (not contained)
  vector<Point> inner = {Point(10.0,-10.0), Point(-10.0,-10.0), Point(0.0,10.0)};
  vector<Point> outer = {Point(2.0,0.0), Point(0.0,-2.0), Point(-2.0,0.0), 
    Point(-1.0,2.0), Point(1.0,2.0)};

  Polygon in = Polygon(inner);
  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonPolygon, TouchingInside)
{
  vector<Point> inner = {Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.0)};
  Polygon in = Polygon(inner);
  vector<Point> outer = {Point(2.0,1.0), Point(0.0,-4.0), Point(-2.0,1.0)};
  Polygon out = Polygon(outer);
  CHECK(in.ContainedBy(out));
}

TEST(PolygonPolygon, TouchingOutside)
{
  vector<Point> inner = {Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.0)};
  Polygon in = Polygon(inner);
  vector<Point> outer = {Point(2.0,1.0), Point(-2.0,1.0), Point(0.0,4.0)};
  Polygon out = Polygon(outer);
  CHECK(!in.ContainedBy(out));
}

//=======================================================================
TEST_GROUP(PolygonReuleaux)
{
};
//=======================================================================

TEST(PolygonReuleaux, Inside)
{// Inner is contained by outer
  vector<Point> inner = {Point(1.0,-1.0), Point(-1.0,-1.0), Point(0.0,1.0)};
  Polygon in = Polygon(inner);

  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle out = ReuleauxTriangle(vert);
  CHECK(in.ContainedBy(out));
}

TEST(PolygonReuleaux, Outside) 
{//  Inner is entirely outside Outer (not contained)
  vector<Point> inner = {Point(6.0,3.0), Point(4.0,3.0), Point(5.0,5.0)};
  Polygon in = Polygon(inner);

  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle out = ReuleauxTriangle(vert);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonReuleaux, Intersects) 
{// Inner and Outer intersect (not contained)
  vector<Point> inner = {Point(2.0,0.0), Point(0.0,-2.0), Point(-2.0,0.0)};
  Polygon in = Polygon(inner);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle out = ReuleauxTriangle(vert);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonReuleaux, Surrounds)
{// Inner surrounds Outer (not contained)
  vector<Point> inner = {Point(4.0,-4.0), Point(-4.0,-4.0), Point(0.0,4.0)};
  Polygon in = Polygon(inner);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle out = ReuleauxTriangle(vert);
  CHECK(!in.ContainedBy(out));
}

TEST(PolygonReuleaux, TouchingInside) 
{
  vector<Point> inner = {Point(1.0,-1.0), Point(-1.0,-1.0), Point(0.0,2.0)};
  Polygon in = Polygon(inner);

  Point vertices[3] = {Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle out = ReuleauxTriangle(vert);
  CHECK(in.ContainedBy(out));
}

TEST(PolygonReuleaux, TouchingOutside)
{
  vector<Point> inner = {Point(2.0,2.0), Point(-2.0,2.0), Point(0.0,4.0)};
  Polygon in = Polygon(inner);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle out = ReuleauxTriangle(vert);
  CHECK(!in.ContainedBy(out));
}
