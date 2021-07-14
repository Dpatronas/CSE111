#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "bounds.h"
#include "geom.h"

/*
 * Determine if one shape is contained by another where "contained"
 * means entirely within the bounds of the containing shape.
 * Return TRUE if INNER is contained within OUTER, FALSE otherwise.
*/

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

// Circle in Circle Test
static bool check_bounds_c_c(Circle *inner, Circle *outer) {
  return (dist_2points( &inner->center, &outer->center) <= outer->radius - inner->radius);
}

// Circle in Triangle Test
static bool check_bounds_c_p(Circle *inner, Polygon *outer) {

  // Check for intersections
  for (int i = 0; i < outer->num_vertices-1; i++) {
    if (line_intersects_circle (inner, &outer->vertices[i], &outer->vertices[i+1]) == true ) {
      return false;
    }
  }
  // Check if circle center is contained within Triangle area
  if (!point_Inside_Triangle(&inner->center, &outer->vertices[0], &outer->vertices[1], &outer->vertices[2])) {
    return false;
  }
  return true;
}

// Triangle in Circle Test
// Returns False if any triangles vertices is further than the radius from center of circle
static bool check_bounds_p_c(Polygon *inner, Circle *outer) {
  for (int i = 0; i < 3; i++) {
    if (dist_2points( &outer->center, &inner->vertices[i]) > outer->radius) {
      return false;
    }
  }
  return true;
}

// Triangle in Polygon Test
static bool check_bounds_p_p(Polygon *inner, Polygon *outer) {

  bool ret = true;

  // return false if Triangle is Outside else assumed triangle inside : 
  // check if Centers of poly and triangle intesect edges of Polygon
  for (int i = 0; i < outer->num_vertices-1; i++) {
    Point* out = polygon_Center(outer);
    Point* in = polygon_Center(inner);

    if (segmentIntersect( out, in, &outer->vertices[i], &outer->vertices[i+1] )) {
      ret = false;  //triangle outside
    }
    free(out); free(in);
    out = NULL; in = NULL;
  }

  int contained = 0;

  // Triangle surrounds pentagon Test
  // Pass vertexes into triangle area, all must be contained to pass
  for (int i = 0; i < outer->num_vertices; i++) {
    if (point_Inside_Triangle( &outer->vertices[i], &inner->vertices[0], &inner->vertices[1], &inner->vertices[2] )) {
      contained++;
    }
  }
  if (contained == 3) {
    ret = false;
  }

  return ret;
}

// Main program
bool contained_by(Shape *inner, Shape *outer) {
  if ((inner->type == CIRCLE) && (outer->type == CIRCLE)) {
    return (check_bounds_c_c((Circle*)inner, (Circle*)outer));
  }

  else if ((inner->type == CIRCLE) && (outer->type == POLYGON)) {
    return (check_bounds_c_p((Circle*)inner, (Polygon*)outer));
  }

  else if ((inner->type == POLYGON) && (outer->type == CIRCLE)) {
    return (check_bounds_p_c((Polygon*)inner, (Circle*)outer));
  }

  else {
    return (check_bounds_p_p((Polygon*)inner, (Polygon*)outer));
  }

  exit(-1);
}
