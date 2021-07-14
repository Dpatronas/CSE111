#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "bounds.h"
#include "geom.h"

/* Citation:
 Circle w/ Circle Code: Prof. Harrison Lecture 3/4 - July 23
 Dist between Point:    https://technotip.com/7339/c-program-to-calculate-distance-between-two-points/
 line intersect circle: http://csharphelper.com/blog/2014/09/determine-where-a-line-intersects-a-circle-in-c/
 Area of Triangle:      https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
 Intersecting Lines:    https://www.youtube.com/watch?v=A86COO8KC58
    https://github.com/bit101/CodingMath/blob/master/episode33/main_interactive.js

  Center of Polygon:    https://stackoverflow.com/questions/19766485/how-to-calculate-centroid-of-polygon-in-c
  Center of Triangle:   https://www.geeksforgeeks.org/program-to-find-the-centroid-of-the-triangle/
*/

/// Generic Formula to find distance between two points
double dist_2points(Point *a, Point *b) {
  return (sqrt( pow((a->x - b->x),2) + pow((a->y - b->y),2) ));
}

// return true is line segments intersect
bool segmentIntersect(Point *p1, Point *p2, Point *p3, Point *p4) {

  double A1, B1, C1, A2, B2, C2, den, intersectX, intersectY, rx0, ry0, rx1, ry1;

  A1 = p2->y - p1->y;
  B1 = p1->x - p2->x;
  C1 = A1 * p1->x + B1 * p1->y;
  A2 = p4->y - p3->y;
  B2 = p3->x - p4->x;
  C2 = A2 * p3->x + B2 * p3->y;
  den = A1 * B2 - A2 * B1;

  intersectX = (B2 * C1 - B1 * C2) / den;
  intersectY = (A1 * C2 - A2 * C1) / den;
  rx0 = (intersectX - p1->x) / (p2->x - p1->x);
  ry0 = (intersectY - p1->y) / (p2->y - p1->y);
  rx1 = (intersectX - p3->x) / (p4->x - p3->x);
  ry1 = (intersectY - p3->y) / (p4->y - p3->y);

  if (((rx0 >= 0 && rx0 <= 1) || (ry0 >= 0 && ry0 <= 1)) && 
      ((rx1 >= 0 && rx1 <= 1) || (ry1 >= 0 && ry1 <= 1))) 
  {
    return  true;
  }
  return false;
}

// Returns true if a line intersects or is tangential to a circle   // Circle in Polygon Test
bool line_intersects_circle (Circle *circle, Point *p1, Point *p2) {
  
  double cx, cy, dx, dy, A, B, C, det;

  cx = circle->center.x;
  cy = circle->center.y;

  dx = p2->x - p1->x;
  dy = p2->y - p1->y;

  A = (dx * dx) + (dy * dy);
  B = 2 * ((dx * (p1->x - cx)) + (dy * (p1->y -cy)));
  C = pow((p1->x - cx),2) + pow((p1->y - cy), 2) - pow((circle->radius),2);  //circle equation
  
  det = B * B - 4 * A * C;
  
  // No intersections
  if ((A <= 0.0000001) || (det < 0)) {
    return false; 
  }
  // At least one intersection
  return true; 
}

// A function to calculate area of triangle
double triangle_Area(int x1, int y1, int x2, int y2, int x3, int y3) {
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}
 
// A function to check whether point P(x, y) lies inside a triangle
bool point_Inside_Triangle(Point *p0, Point *p1, Point *p2, Point *p3) {
  
  double A  = triangle_Area (p1->x, p1->y, p2->x, p2->y, p3->x, p3->y);
  double A1 = triangle_Area (p0->x, p0->y, p2->x, p2->y, p3->x, p3->y);
  double A2 = triangle_Area (p1->x, p1->y, p0->x, p0->y, p3->x, p3->y);
  double A3 = triangle_Area (p1->x, p1->y, p2->x, p2->y, p0->x, p0->y);

  return (A == A1 + A2 + A3);
}

// Find and return Point in the center of polygon
Point* polygon_Center(Polygon *poly) {
  double cx, cy, t;
  double area = 0.0;
  int i1 = 1;

  for (int i = 0; i < poly->num_vertices; i++) {
    area += ((poly->vertices[i].x  * poly->vertices[i1].y) - 
             (poly->vertices[i1].x * poly->vertices[i].y));
    i1 = (i1 + 1) % poly->num_vertices;
  }

  area *= 0.5;

  // Now calculate the centroid coordinates Cx and Cy
  cx = cy = 0.0;
  i1 = 1;
  for (int i = 0; i < poly->num_vertices; i++) {
    t = ((poly->vertices[i].x  * poly->vertices[i1].y) - 
         (poly->vertices[i1].x * poly->vertices[i].y));
    cx += (poly->vertices[i].x + poly->vertices[i1].x) * t;
    cy += (poly->vertices[i].y + poly->vertices[i1].y) * t;
    i1 = (i1 + 1) % poly->num_vertices;
  }
  cx /= (6.0 * area);
  cy /= (6.0 * area);

  Point* poly_center = (Point*)malloc( sizeof(Point));

  poly_center->x = cx;
  poly_center->y = cy;

  return poly_center;
}

