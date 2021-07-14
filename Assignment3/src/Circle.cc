
#include <cmath>
#include <cstdio>
#include "Circle.h"
#include "Polygon.h"
#include "Reuleaux.h"
#include "Geom.h"

// =======================================
// Constructor ========== Getter Functions ==========
// =======================================
Circle::Circle(const Point &center, double radius) {
  center_ = Point(center);
  radius_ = radius;
}
Point Circle::Center() {
  return Point(center_);
}
double Circle::Radius() {
  return radius_;
}

// =======================================
// Main Class Functions===============================
// =======================================
bool Circle::ContainedBy(Circle &circle) {
  return Geom::DistanceBetween(circle.Center(), Center()) 
    <= circle.Radius() - Radius();
}

bool Circle::ContainedBy(Polygon &polygon) {
  for (int i = 0; i < polygon.Vertices()-1; i++) {
    int intersections = Geom::LineIntersectsCircle(Center(), Radius(), 
      polygon.Vertex(i), polygon.Vertex(i+1));
    if (intersections == 2) {
      return false;
    }
    else if (intersections <= 1 && Geom::DistanceBetween(Center(), 
      Geom::PolygonCenter(polygon)) <= Radius()) {
        return true;
      }
  }
  Point center = Center();
  return Geom::PointInPolygon(center, polygon);
}

bool Circle::ContainedBy(ReuleauxTriangle &rt) {
  return Geom::DistanceBetween(rt.Center(), Center()) 
    <= rt.RadiusFromCenter() - Radius();
}
