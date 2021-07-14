
#include <cmath>
#include <cstdio>
#include "Circle.h"
#include "Polygon.h"
#include "Reuleaux.h"
#include "Geom.h"
#include "Containable.h"

// =======================================
// Constructor ========== Getter Functions ==========
// =======================================
ReuleauxTriangle::ReuleauxTriangle(const Point vertices[3]) {
    for (int i = 0; i < 3; i++) {
        vertices_[i] = vertices[i];
    }
}

Point ReuleauxTriangle::Vertex(int indx) {
    return vertices_[indx];
}

Point ReuleauxTriangle::Center() {
  vector<Point>v = {Vertex(0),Vertex(1),Vertex(2)};
  Polygon rt = Polygon(v);
  Point center = Geom::PolygonCenter(rt);
  return center;
}

double ReuleauxTriangle::RadiusFromCenter() {
  return Geom::DistanceBetween(vertices_[1], vertices_[2])/2;
}

// =======================================
// Main Class Functions===============================
// =======================================

bool ReuleauxTriangle::ContainedBy(Circle &circle) {
  return Geom::DistanceBetween(circle.Center(), Center()) 
    <= circle.Radius() - RadiusFromCenter();
}

bool ReuleauxTriangle::ContainedBy(Polygon &polygon) {
  for (int i = 0; i < polygon.Vertices()-1; i++) {
    int intersections = Geom::LineIntersectsCircle(Center(), RadiusFromCenter(), 
      polygon.Vertex(i), polygon.Vertex(i+1));
    if (intersections == 2) {
      return false;
    }
    else if (intersections <= 1 && Geom::DistanceBetween(Center(), 
      Geom::PolygonCenter(polygon)) <= RadiusFromCenter()) {
        return true;
      }
  }
  Point center = Center();
  return Geom::PointInPolygon(center, polygon);
}

bool ReuleauxTriangle::ContainedBy(ReuleauxTriangle &rt) {
  return Geom::DistanceBetween(rt.Center(), Center()) 
    <= rt.RadiusFromCenter() - RadiusFromCenter();
}

