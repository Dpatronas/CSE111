
#include "CppUTest/TestHarness.h"

#include <cmath>
#include "Circle.h"
#include "Polygon.h"
#include "Reuleaux.h"

TEST_GROUP(CircleCircle)
{
};
//=======================================================================

TEST(CircleCircle, Inside)
{// 1. Inner is contained by outer
  Circle inner = Circle(Point(0.0,0.0), 2.0);
  Circle outer = Circle(Point(0.0,0.0), 4.0);
  CHECK(inner.ContainedBy(outer));
}

TEST(CircleCircle, Outside) 
{// 2. Inner is entirely outside Outer (not contained)
  Circle inner = Circle(Point(0.0,0.0), 2.0);
  Circle outer = Circle(Point(10.0,10.0), 4.0);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CircleCircle, Intersects) 
{// 3. Inner and Outer intersect (not contained)
  Circle inner = Circle(Point(-1.0,-1.0), 5.0);
  Circle outer = Circle(Point(1.0,1.0), 5.0);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CircleCircle, Surrounds) 
{// 4. Inner surrounds Outer (not contained)
  Circle inner = Circle(Point(0.0,0.0), 4.0);
  Circle outer = Circle(Point(0.0,0.0), 2.0);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CircleCircle, TouchingInside) 
{// 5. Inner perimeter touches Outer perimeter (contained)
  Circle inner = Circle(Point(0.0,0.0), 4.0);
  Circle outer = Circle(Point(0.0,0.0), 4.0);
  CHECK(inner.ContainedBy(outer));
}

TEST(CircleCircle, TouchingOutside) 
{// 6. Inner perimeter touches Outer perimeter (not contained)
  Circle inner = Circle(Point(1.0,1.0), 1.0);
  Circle outer = Circle(Point(1.0,-1.0), 1.0);
  CHECK(!inner.ContainedBy(outer));
}

//=======================================================================
TEST_GROUP(CirclePolygon)
{
};
//=======================================================================

TEST(CirclePolygon, Inside)
{// 7. Inner is contained by outer
  Circle inner = Circle(Point(1.0,1.0), 1.0);
  vector<Point> vec = {Point(15.0,-5.0), Point(-15.0,-5.0), Point(0.0,15.0)};
  Polygon outer = Polygon(vec);
  CHECK(inner.ContainedBy(outer));
}

TEST(CirclePolygon, Outside)
{// 8. Inner is entirely outside Outer (not contained)
  Circle inner = Circle(Point(15.0,15.0), 1.0);
  vector<Point> vec = {Point(5.0,0.0), Point(-5.0,0.0), Point(0.0,-5.0)};
  Polygon outer = Polygon(vec);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CirclePolygon, Intersects) 
{// 9. Inner and Outer intersect (not contained)
  Circle inner = Circle(Point(1.0,1.0), 1.0);
  vector<Point> vec = {Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.0)};
  Polygon outer = Polygon(vec);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CirclePolygon, Surrounds) 
{// 10. Inner surrounds Outer (not contained)
  Circle inner = Circle(Point(10.0,10.0), 1.0);
  vector<Point> vec = {Point(1.0,0.0), Point(-1.0,0.0), Point(0.0,1.0)};
  Polygon outer = Polygon(vec);  
  CHECK(!inner.ContainedBy(outer));
}

TEST(CirclePolygon, TouchingInside) 
{
  Circle inner = Circle(Point(0.0,2.0), 2.0);
  vector<Point> vec = {Point(4.0,0.0), Point(-4.0,0.0), Point(0.0,4.0)};
  Polygon outer = Polygon(vec);  
  CHECK(inner.ContainedBy(outer));
}

TEST(CirclePolygon, TouchingOutside) 
{
  Circle inner = Circle(Point(2.0,4.0), 3.0);
  vector<Point> vec = {Point(4.0,0.0), Point(-4.0,0.0), Point(0.0,4.0)};
  Polygon outer = Polygon(vec);  
  CHECK(!inner.ContainedBy(outer));
}

//=======================================================================
TEST_GROUP(CircleReuleaux)
{
};
//=======================================================================

TEST(CircleReuleaux, Inside)
{// 11. Inner is contained by outer
  Circle inner = Circle(Point(0.0,0.0), 1.0);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle outer = ReuleauxTriangle(vert);
  CHECK(inner.ContainedBy(outer));
}

TEST(CircleReuleaux, Outside)
{// 12. Inner is entirely outside Outer (not contained)
  Circle inner = Circle(Point(5.0,5.0), 1.0);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle outer = ReuleauxTriangle(vert);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CircleReuleaux, Intersects) 
{// 13. Inner and Outer intersect (not contained)
  Circle inner = Circle(Point(0.0,0.0), 3.0);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle outer = ReuleauxTriangle(vert);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CircleReuleaux, Surrounds) 
{// 14. Inner surrounds Outer (not contained)
  Circle inner = Circle(Point(0.0,0.0), 5);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle outer = ReuleauxTriangle(vert);
  CHECK(!inner.ContainedBy(outer));
}

TEST(CircleReuleaux, TouchingInside) 
{// 15. Inner perimeter touches Outer perimeter (contained)
  Circle inner = Circle(Point(0.0,0.0), 1.0);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle outer = ReuleauxTriangle(vert);
  CHECK(inner.ContainedBy(outer));
}

TEST(CircleReuleaux, TouchingOutside) 
{// 16. Inner perimeter touches Outer perimeter (not contained)
  Circle inner = Circle(Point(0.0,3.0), 1);
  Point vertices[3] = { Point(1.732,-1.0), Point(-1.732,-1.0), Point(0.0,2.0)};
  Point *vert = vertices;
  ReuleauxTriangle outer = ReuleauxTriangle(vert);
  CHECK(!inner.ContainedBy(outer));
}
