/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "Containable.h"
#include "Point.h"

using namespace std;

class Circle : public Containable {

  private:
    Point center_;
    double radius_;

  public:
    // do not modify or remove this constructor
    Circle(const Point &center, double radius);

    // do not modify or remove these functions
    bool ContainedBy(Circle &circle);
    bool ContainedBy(Polygon &polygon);
    bool ContainedBy(ReuleauxTriangle &rt);

    Point Center();
    double Radius();
};

#endif
