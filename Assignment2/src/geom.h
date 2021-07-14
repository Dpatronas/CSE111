#ifndef _GEOM_H_
#define _GEOM_H_

/// Generic Formula to find distance between two points
double dist_2points(Point *a, Point *b);

// return true is line segments intersect
bool segmentIntersect(Point *p1, Point *p2, Point *p3, Point *p4);

// Returns true if a line intersects or is tangential to a circle   // Circle in Polygon Test
bool line_intersects_circle (Circle *circle, Point *p1, Point *p2);

// A function to calculate area of triangle
double triangle_Area(int x1, int y1, int x2, int y2, int x3, int y3);
 
// A function to check whether point P(x, y) lies inside a triangle
bool point_Inside_Triangle(Point *p0, Point *p1, Point *p2, Point *p3);

// A function to find and return Point at the center of a polygon
Point* polygon_Center(Polygon *outer);

#endif
