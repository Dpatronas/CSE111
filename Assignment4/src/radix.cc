/*
 * Copyright (C) 20182-2021 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

// Citation:
// String Sort Method: https://www.geeksforgeeks.org/sort-an-array-of-strings-lexicographically-based-on-prefix/
// Compare() : https://stackoverflow.com/questions/19588809/sort-array-of-integers-lexicographically-c
// Threads: https://stackoverflow.com/questions/54551371/creating-thread-inside-a-for-loop-c

#include <iostream>
#include <string>
#include <thread>
#include <bits/stdc++.h>

#include "radix.h"

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
    strList.emplace_back(to_string(list.at(i)));
  }

  sort(strList.begin(), strList.end(), Compare);

  for (unsigned int i = 0; i < list.size(); i++) {
    sorted.emplace_back(stoull(strList.at(i)));
  }
  list = sorted;
}

void joinThreads(std::vector<std::thread> &Threads) {
  for(auto& t: Threads) {  // Sync threads
      t.join();
  }
}

void RadixSort::msd(vector<reference_wrapper<vector<unsigned int>>> &lists, 
  const unsigned int cores) {

  std::vector<std::thread> Threads;

  for (std::vector<unsigned int> &list : lists) {
    Threads.emplace_back([&](){
      LexNumbers(list);
    });

    if (Threads.size() == lists.size()) {
        break;
    }

    if (Threads.size() == cores) {
      joinThreads(Threads);
      Threads.clear();
    }
  }
  joinThreads(Threads);
}
