
#include <cmath>
#include <cstdio>
#include "Geom.h"

/* Citation:
 Circle w/ Circle Code: Prof. Harrison Lecture 3/4 - July 23
 DistanceBetween:      https://technotip.com/7339/c-program-to-calculate-distance-between-two-points/
 LineIntersectsCircle: http://csharphelper.com/blog/2014/09/determine-where-a-line-intersects-a-circle-in-c/
 SegmentIntersect:     https://www.youtube.com/watch?v=A86COO8KC58
                       https://github.com/bit101/CodingMath/blob/master/episode33/main_interactive.js
 PolygonCenter:        https://stackoverflow.com/questions/19766485/how-to-calculate-centroid-of-polygon-in-c
 PointInsidePolygon:   https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
                       https://stackoverflow.com/questions/11716268/point-in-polygon-algorithm
*/

double Geom::DistanceBetween(const Point &a, const Point &b) {
  return (sqrt( pow((a.x - b.x),2) + pow((a.y - b.y),2) ));
}

int Geom::LineIntersectsCircle (const Point &center, 
    const double &radius, const Point &p1, const Point &p2) {

  double cx, cy, dx, dy, A, B, C, det;

  cx = center.x;
  cy = center.y;

  dx = p2.x - p1.x;
  dy = p2.y - p1.y;

  A = (dx * dx) + (dy * dy);
  B = 2 * ((dx * (p1.x - cx)) + (dy * (p1.y -cy)));
  C = pow((p1.x - cx),2) + pow((p1.y - cy), 2) - 
      pow((radius),2);
  det = B * B - 4 * A * C;
  
  if ((A <= 0.0000001) || (det < 0)) { // no intersections
    return 0; 
  }

  else if (det == 0) {  //touching
    return 1;
  }

  return 2; //intersects
}

// return true is line segments intersect
bool Geom::SegmentIntersect(Point &p1, Point &p2, Point &p3, 
    Point &p4) {

  double A1, B1, C1, A2, B2, C2, den, intersectX, 
    intersectY, rx0, ry0, rx1, ry1;

  A1 = p2.y - p1.y;
  B1 = p1.x - p2.x;
  C1 = A1 * p1.x + B1 * p1.y;
  A2 = p4.y - p3.y;
  B2 = p3.x - p4.x;
  C2 = A2 * p3.x + B2 * p3.y;
  den = A1 * B2 - A2 * B1;

  intersectX = (B2 * C1 - B1 * C2) / den;
  intersectY = (A1 * C2 - A2 * C1) / den;

  rx0 = (intersectX - p1.x) / (p2.x - p1.x);
  ry0 = (intersectY - p1.y) / (p2.y - p1.y);
  rx1 = (intersectX - p3.x) / (p4.x - p3.x);
  ry1 = (intersectY - p3.y) / (p4.y - p3.y);

  if (((rx0 >= 0 && rx0 <= 1) || (ry0 >= 0 && ry0 <= 1)) && 
      ((rx1 >= 0 && rx1 <= 1) || (ry1 >= 0 && ry1 <= 1))) 
  {
    return  true;
  }
  return false;
}

Point Geom::PolygonCenter(Polygon &poly) {
  double cx, cy, t;
  double area = 0.0;
  int i1 = 1;

  for (int i = 0; i < poly.Vertices(); i++) {
    area += ((poly.Vertex(i).x  * poly.Vertex(i1).y) - 
             (poly.Vertex(i1).x * poly.Vertex(i).y));
    i1 = (i1 + 1) % poly.Vertices();
  }

  area *= 0.5;
  cx = cy = 0.0;
  i1 = 1;
  for (int i = 0; i < poly.Vertices(); i++) {
    t =  ((poly.Vertex(i).x * poly.Vertex(i1).y) - 
          (poly.Vertex(i1).x* poly.Vertex(i).y));
    cx += (poly.Vertex(i).x + poly.Vertex(i1).x) * t;
    cy += (poly.Vertex(i).y + poly.Vertex(i1).y) * t;
    i1 = (i1 + 1) % poly.Vertices();
  }
  cx /= (6.0 * area);
  cy /= (6.0 * area);

  Point center = Point(cx, cy);

  return center;
}

bool Geom::PointInPolygon(Point &point, Polygon &polygon) {
  vector<Point> points = polygon.GetPoints();
  int i; 
  int j;
  int nvert = points.size();
  bool c = false;

  for(i = 0, j = nvert - 1; i < nvert; j = i++) {
    if( ( (points[i].y >= point.y ) != (points[j].y >= point.y) ) &&
        (point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
      )
      c = !c;
  }

  return c;
}
