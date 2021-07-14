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

#ifndef _CONTAINABLE_H_
#define _CONTAINABLE_H_

// C++ requires forward declaration of any cyclically dependent classes
class Circle;
class Polygon;
class ReuleauxTriangle;

class Containable {
  public:
    virtual bool ContainedBy(Circle &circle) = 0;
    virtual bool ContainedBy(Polygon &polygon) = 0;
    virtual bool ContainedBy(ReuleauxTriangle &rt) = 0;
};

#endif
