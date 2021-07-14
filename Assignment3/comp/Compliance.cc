/*
 * Copyright (C) 2018-2021 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

/******************  DO NOT MODIFY THIS FILE ****************************
 *
 * It is not included in the subission archive ceated by 'make submit' .
 *
 * If you modify it and your code relies on those modifications, your code
 * will not compile in the automated test harness and will be unable to
 * execute any tests.
 *
 * To put it another way, if you modify this file, you will get a big fat
 * ZERO on this assignment.
 *
 ************************************************************************/

#include "Circle.h"
#include "Polygon.h"
#include "Reuleaux.h"

int main(int argc, char *argv[])
{
  Point p1 = Point();
  Point p2 = Point(0.0, 0.0);
  Point p3 = Point(p2);
  p1 = p3;

  Circle circle = Circle(Point(0.0,0.0), 2.0);

  Polygon polygon = Polygon({Point(0,0), Point(1,1), Point(-1,-1)});

  Point v[3] = {Point(0,0), Point(0,0), Point(0,0)};
  ReuleauxTriangle reuleaux = ReuleauxTriangle(v);

  circle.ContainedBy(circle);
  circle.ContainedBy(polygon);
  circle.ContainedBy(reuleaux);

  polygon.ContainedBy(circle);
  polygon.ContainedBy(polygon);
  polygon.ContainedBy(reuleaux);

  reuleaux.ContainedBy(circle);
  reuleaux.ContainedBy(polygon);
  reuleaux.ContainedBy(reuleaux);
}