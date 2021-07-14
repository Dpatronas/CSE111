
#include <cmath>
#include <cstdio>
#include "Circle.h"
#include "Polygon.h"
#include "Reuleaux.h"
#include "Geom.h"

// =======================================
// Constructor ========== Getter Functions ==========
// =======================================
Polygon::Polygon(const vector<Point>vec) {
    vertices = vec;
    vertices_ = vertices.size();
}
int Polygon::Vertices() {
    return vertices_;
}
Point Polygon::Vertex(int indx) {
    return vertices.at(indx);
}
vector<Point> Polygon::GetPoints() {
  return vertices;
}

// =======================================
// Main Class Functions===============================
// =======================================
bool Polygon::ContainedBy(Circle &circle) {
  for (int i = 0; i < Vertices(); i++) {
    if ( Geom::DistanceBetween(circle.Center(), this->Vertex(i))
         > circle.Radius()) {
      return false;
    }
  }
  return true;
}

bool Polygon::ContainedBy(Polygon &polygon) {

  bool ret = true;
  int len;
  if (polygon.Vertices() <= Vertices()) {
    len = polygon.Vertices();
  }
  else {
    len = Vertices();
  }

  //checks intersections
  for (int i = 0; i < len-1; i++) {
    Polygon *inner = this;
    Point in = Geom::PolygonCenter(*inner);
    Point out = Geom::PolygonCenter(polygon);

    Point v0 = polygon.Vertex(i);
    Point v1 = polygon.Vertex(i+1);

    if (Geom::SegmentIntersect(out, in, v0, v1)) {
      ret = false;
    }
  }

  int contained = 0;
  //checks inner vertices are contained within outer polygon
  for (int i = 0; i < Vertices(); i++) {
    Point point = Vertex(i);
    if (Geom::PointInPolygon(point, polygon)) {
      contained++;
    }
  }
  
  if (contained < polygon.Vertices()) {
    ret = false;
  }
  return ret;
}

bool Polygon::ContainedBy(ReuleauxTriangle &rt) {

  //checks contained from within using vertices only not the arch radius
  for (int i = 0; i < 3; i++) {
    if (Vertex(i).x == rt.Vertex(i).x && Vertex(i).y == rt.Vertex(i).y) {
      return true;
    }
  }
  
  for (int i = 0; i < Vertices(); i++) {
    if ( Geom::DistanceBetween(rt.Center(), Vertex(i))
         > rt.RadiusFromCenter()) {
      return false;
    }
  }
  return true;
}

