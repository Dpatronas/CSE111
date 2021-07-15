/*
 * Copyright (C) 2018-2021 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include <string>
#include <math.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>

#include "radix.h"

using namespace std;

// Helper for LexNumbers
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

// Sort by buckets
void BucketSort(std::vector<unsigned int> &list) {
  
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
    LexNumbers(digit);
  }

  for (unsigned int i = 0; i < toSort.size(); i++) {
    for (unsigned int j = 0; j < toSort[i].size(); j++) {
      sorted.push_back(toSort[i][j]);
    }
  }

  list = sorted;
}

// UDP protocol
// Packets can go missing
// Packets can arrive out of order 
void RadixServer::start(const int port) {

  int sockid = socket(AF_INET, SOCK_DGRAM, 0);   
  struct sockaddr_in server_addr;
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(sockid, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    exit(-1);

  struct sockaddr_in remote_addr;
  socklen_t len = sizeof(remote_addr);

  // ==================================================================================
  vector<unsigned int> list;
  // vector<message_t> datagrams;
  message_t DG;
  int n;

  for (;;) {
    n = recvfrom(sockid, (void*) &DG, sizeof(message_t), 0, (struct sockaddr *)&remote_addr, &len);
    if (n < 0) exit (-1);

      unsigned int size;
      size = htonl(DG.num_values);

      for (unsigned int i = 0; i < size; i++) {
        unsigned element = htonl(DG.values[i]);
        list.push_back(element);
      }

    // SORT AND SEND NUMBERS BACK TO CLIENT
    if (htonl(DG.flag) == LAST) { // Last DG
      BucketSort(list);

      DG.num_values = htonl(0); 
      DG.flag = htonl(NONE);
      unsigned int sequence = 0;

      for (auto &elem : list) {
        DG.values[DG.num_values++] = ntohl(elem);

        // Send a full DG
        if (DG.num_values == MAX_VALUES) {
          DG.num_values = ntohl(DG.num_values);
          DG.sequence = ntohl(sequence);
          DG.flag = ntohl(NONE);

          n = sendto(sockid, (void *) &DG, sizeof(message_t), 0, 
            (struct sockaddr *)&remote_addr, len);
          if (n < 0) exit(-1);

          sequence++;
          DG.num_values = htonl(0);
        }
      }

      DG.flag = ntohl(LAST);
      DG.num_values = ntohl(DG.num_values);
      DG.sequence = ntohl(sequence);

      n = sendto(sockid, (void *) &DG, sizeof(message_t), 0, 
        (struct sockaddr *)&remote_addr, len);
      
      if (n < 0) exit(-1);

      DG.num_values = ntohl(0); 
      DG.sequence = ntohl(0); 
      DG.flag = ntohl(0);    

      list.clear();
    }
  }
  close(sockid);
}

void SetList(vector<unsigned int> &list, vector<unsigned int> &sorted){
  list = sorted;
}

void RadixClient::msd(const char *hostname, const int port, 
  std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) { 

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) exit(-1);

  struct hostent *server = gethostbyname(hostname);
  if (server == NULL) exit (-1);

  struct sockaddr_in remote_addr;
  bzero((char *) &remote_addr, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  bcopy((char *) server->h_addr, (char *)
    &remote_addr.sin_addr.s_addr, server->h_length);
  remote_addr.sin_port = htons(port);
  socklen_t len = sizeof(remote_addr);

//==================================================

  vector<unsigned int> sorted;
  vector<message_t> datagrams;
  int check;

  //////////////////////////////////////////////////////////////////////////////////
  // SEND DATAGRAMS TO SERVER
  //////////////////////////////////////////////////////////////////////////////////

  for (std::vector<unsigned int> &list : lists) {
    int seq = 0;  // Sequence for the current list if Server Requests Resends
    unsigned int num_DG = ceil((float)list.size() / 128.0);

    // Make the DataGram(s) for this list
    for (unsigned int i = 0; i < num_DG; i++) {

      message_t DG;
      DG.sequence = htonl(seq++);
      unsigned int DG_start, DG_end;

      // Last DG in List
      if (i == (num_DG - 1)) {
        DG.flag = htonl(1);
        DG.num_values = htonl(list.size() - (128 * i));
        DG_end = list.size();
      }

      // Regular DG in List
      else {
        DG.flag = htonl(0);
        DG.num_values = htonl(128);
        DG_end = (i+1) * 128;
      }

      // Fill current DG
      DG_start = i * 128;
      unsigned int DG_index = 0;
      for (unsigned int j = DG_start; j < DG_end; j++) {
        unsigned int element = htonl(list.at(j));
        DG.values[DG_index++] = element;
      }
      // Save datagram for resends : NOTE element DG == seq#
      datagrams.push_back(DG);

      // Send DG
      check = sendto(sockfd, (void *)& DG, sizeof(message_t), 0, 
        (struct sockaddr *)&remote_addr, len);
      if (check < 0) exit(-1);
    }
    //////////////////////////////////////////////////////////////////////////////////
    // RECEIVE DATAGRAMS FROM SERVER
    //////////////////////////////////////////////////////////////////////////////////

    for (;;) {
      message_t on_wire; 
      check = recvfrom(sockfd, (void*)&on_wire, sizeof(message_t), 0, 
        (struct sockaddr *)&remote_addr, &len);
      if (check < 0) exit (-1);

      message_t received = on_wire;
      unsigned int size, flag, sequence;
      size = ntohl(received.num_values);
      flag = ntohl(received.flag);
      sequence = ntohl(received.sequence);

      for (unsigned int i = 0; i < size; i++) {
        unsigned element = ntohl(received.values[i]);
        sorted.push_back(element);
      }
      if (flag == 1)    // End of list
        break;

      if (flag == 2) {  // Server Resends Datagram at the sequence requested in current list
        check = sendto(sockfd, (void *)& datagrams.at(sequence), 
          sizeof(message_t), 0, (struct sockaddr *)&remote_addr, len);
        if (check < 0) exit(-1);
      }
    }
    // END OF SEND AND RECEIVE FOR THE CURRENT LIST
    SetList(list, sorted);
    sorted.clear();       // Clear current sorted list
    datagrams.clear();    // Clear current list DG Sequence
  }

  close(sockfd);
}
