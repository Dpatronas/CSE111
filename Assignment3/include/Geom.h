
#ifndef _GEOM_H_
#define _GEOM_H_

#include <vector>
#include "Point.h"
#include "Circle.h"
#include "Polygon.h"

#include "Containable.h"

using namespace std;

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

class Geom {
  public:
    static double DistanceBetween (const Point &a, const Point &b);

    static int LineIntersectsCircle (const Point &center, 
        const double &radius, const Point &p1, const Point &p2);

    static bool SegmentIntersect (Point &p1, Point &p2, Point &p3, Point &p4);

    static Point PolygonCenter (Polygon &poly);

    static bool PointInPolygon(Point &point, Polygon &polygon);
};

#endif