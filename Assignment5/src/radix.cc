
/*
 * Copyright (C) 2018-2021 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */
#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include <bits/stdc++.h>

#include "radix.h"

// Citations:
// Get First Digit of Number: https://www.codevscolor.com/c-plus-plus-find-last-digit-number
// Bucket Sort Example:       https://www.geeksforgeeks.org/bucket-sort-2/
// Clearing Array of Vector:  https://stackoverflow.com/questions/33290077/most-efficient-way-to-clear-array-of-vectors
using namespace std;

bool Compare(string a, string b) {
  unsigned int i = 0, j = 0;
  while(i < a.size() && j < b.size()) {
    if(a[i] - '0' != b[j] - '0')
      return (a[i] - '0' < b[j] - '0');
    i++, j++;
  }
  return (a.size() < b.size());
}

// Lexicographically sort the unsigned ints in list vector
void LexNumbers(vector<unsigned int> &list) {
  vector<string> strList;
  vector<unsigned int> sorted;

  // Convert to string
  for (unsigned int i = 0; i < list.size(); i++) {
    strList.push_back(to_string(list.at(i)));
  }

  sort(strList.begin(), strList.end(), Compare);

  for (unsigned int i = 0; i < list.size(); i++) {
    sorted.push_back(stoull(strList.at(i)));
  }
  list = sorted;
}

void JoinThreads(std::vector<std::thread> &Threads) {
  for(auto& t: Threads) {  // Sync threads
      t.join();
  }
}

// Note: bucket[0] == 1 digits | bucket[8] == 9 digits
void BucketSort(std::vector<unsigned int> &list, unsigned int cores) {
  
  std::vector<std::thread> SubListThread;
  vector<unsigned int> bucket[9];
  vector<unsigned int> smallBucket[10];
  vector<vector<unsigned int>> toSort;
  vector<unsigned int> sorted;

  // Put elements in different buckets
  for (auto elem : list) {
    unsigned int digit = elem;
    while(digit >= 10) {
      digit = digit/10;
    }
    bucket[digit-1].push_back(elem);
  }
  
  // Calulate mean size of buckets
  unsigned int buckMed = 0;
  for (auto buck: bucket) {
    buckMed += buck.size();
  }
  buckMed /= 9;

  // Divide and Conquer (Uniform Distribution)
  for (auto buck: bucket) {
    if (buck.size() > buckMed) {
      for (auto elem : buck) {
        // Get the Second digit of the element
        unsigned int d = elem;
        while(d >= 100) {
          d = d/10;
        }
        while (d >= 10) {
          d -= 10;
        }
        smallBucket[d].push_back(elem);
      }
      for (smallB : smallBucket) {
        toSort.push_back(smallB);
      }
      //clear for next iteration
      for (auto& clr : smallBucket) {
        clr.clear();
      }
    }
    else {
      toSort.push_back(buck);
    }
  }

  // Sort buckets in threads
  for (std::vector<unsigned int> &digit : toSort) {

    SubListThread.emplace_back([&] {
      LexNumbers(digit);
    });

    if (SubListThread.size() == cores) {
      JoinThreads(SubListThread);
      SubListThread.clear();
    }
  }

  JoinThreads(SubListThread);

  for (unsigned int i = 0; i < toSort.size(); i++) {
    for (unsigned int j = 0; j < toSort[i].size(); j++) {
      sorted.push_back(toSort[i][j]);
    }
  }
  list = sorted;
}

void ParallelRadixSort::msd(std::vector<std::reference_wrapper
    <std::vector<unsigned int>>> &lists, unsigned int cores) { 

  for (std::vector<unsigned int> &list : lists) {
    BucketSort(list, cores);
  }
}

// unsigned int getDigit(unsigned int num, unsigned int digitPlace) {
//   while(num >= pow(10, digitPlace)) {
//     num = num/10;
//   }
//   // Get single digit
//   while (num >= 10) {
//     num -= 10;
//   }
//   return num;
// }