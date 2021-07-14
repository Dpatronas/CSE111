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

#ifndef _POINT_H_
#define _POINT_H_

class Point {
  public:
    double x;
    double y;

    Point() { x = 0.0, y = 0.0; }
    Point(double dx, double dy) { x = dx; y = dy; }
    Point(const Point &p) { x = p.x; y = p.y; }
};

#endif
