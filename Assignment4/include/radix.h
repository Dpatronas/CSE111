/*
 * Copyright (C) 2018-2021 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <vector>
#include <functional>

/*
 * Simple multi-threaded Radix Sort with support for Most Significant Digit 
 * sorting only.
 */
class RadixSort {
public:
  /*
    * Perform an in-place Most Significant Digit Radix Sort on each list of
    * unsigned integers in LISTS restricted to using no more than CORES processor cores.
    */
  void msd(
      std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, 
      const unsigned int cores);
};
