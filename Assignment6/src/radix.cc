
/*
 * Copyright (C) 2018-2021 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <functional>
#include <thread>

#include "radix.h"

using namespace std;

// Citations:
// Professor Harrison:
// https://drive.google.com/drive/u/0/folders/1j38fDIem-sRltX_sjQl5jPeeRSinvxsk
// String Sort Method: https://www.geeksforgeeks.org/sort-an-array-of-strings-lexicographically-based-on-prefix/
// Compare() : https://stackoverflow.com/questions/19588809/sort-array-of-integers-lexicographically-c

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
  for(auto& t: Threads) {
      t.join();
  }
}

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
      // Clear for next iteration
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

RadixServer::RadixServer(const int port, const unsigned int cores) {
    int sockid = socket(AF_INET, SOCK_STREAM, 0);   
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sockid, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        exit(-1);
    listen(sockid, 1);

    // int check;
    for (;;) {  
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int newsockid = accept(sockid, (struct sockaddr *) &client_addr, &len);
        if (newsockid < 0)
            exit (-1);
    //==================================================================================

        vector<unsigned int> list;

        for (;;) {

            // RECEIVE NUMBERS FROM CLIENT
            unsigned int element = 0;
            int n = recv(newsockid, (void*)&element, sizeof(unsigned int), 0);
            if (n < 0) exit (-1);
            if (n == 0) {
                close(newsockid);
                break;
            }
            unsigned int received = htonl(element);

            if (received != 0) {
                list.push_back(received);
            }

            // SORT AND SEND NUMBERS BACK TO CLIENT
            else {
                BucketSort(list, cores);
                list.push_back(0);

                for (auto &elem : list) {
                    unsigned int on_wire = ntohl(elem);
                    int n = send(newsockid, (void *)&on_wire, sizeof(unsigned int), 0);
                    if (n < 0)
                        exit(-1);
                }
                list.clear();
            }
        }
        close(newsockid);
    }
    close(sockid);
}

void SetList(vector<unsigned int> &list, vector<unsigned int> &sorted){
    list = sorted;
}

void RadixClient::msd(const char *hostname, const int port, 
    std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) { 

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) exit(-1);

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) exit (-1);

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(port);

    if(connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) exit(-1);

    //==================================================================================

    vector<unsigned int> sorted;

    for (std::vector<unsigned int> &list : lists) {

        list.push_back(0);
        
        // SEND NUMBERS TO SERVER
        for (auto &elem : list) {

            unsigned int local = htonl(elem);
            int n = send(sockfd, (void *)&local, sizeof(unsigned int), 0);
            if (n < 0)
                exit(-1);
        }

        // RECEIVE NUMBERS FROM SERVER
        for (unsigned int i = 0; i < list.size(); i++) {

            unsigned int on_wire = 0;    
            int n = recv(sockfd, (void*)&on_wire, sizeof(unsigned int), 0);
            if (n < 0) exit (-1);
            unsigned int received = ntohl(on_wire);

            if (received == 0) {
                SetList(list, sorted);
                sorted.clear();
                break;
            }
            sorted.push_back(received);
        }
    }
    close(sockfd);
}