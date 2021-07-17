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
static int n;

// Citations:
// Professor Harrison:
// https://drive.google.com/drive/u/0/folders/1j38fDIem-sRltX_sjQl5jPeeRSinvxsk

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
  for (unsigned int i = 0; i < list.size(); i++)
    strList.push_back(to_string(list.at(i)));

  sort(strList.begin(), strList.end(), Compare);
  for (unsigned int i = 0; i < list.size(); i++)
    sorted.push_back(stoull(strList.at(i)));

  list = sorted;
}

// Sort by buckets
void BucketSort(std::vector<unsigned int> &list) {
  
  std::vector<std::thread> SubListThread;
  vector<unsigned int> bucket[9];
  vector<unsigned int> sorted;

  // Put elements in different buckets
  for (auto elem : list) {
    unsigned int digit = elem;
    while(digit >= 10) 
      digit = digit/10;

    bucket[digit-1].push_back(elem);
  }

  for (std::vector<unsigned int> &digit : bucket) 
    LexNumbers(digit);
  
  for (unsigned int i = 0; i < 9; i++)
    for (unsigned int j = 0; j < bucket[i].size(); j++)
      sorted.push_back(bucket[i][j]);

  list = sorted;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


// UDP protocol : Packets can go missing & can arrive out of order 
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

  // =======================================================================

  vector<unsigned int> list;
  message_t DG;
  unsigned int checkSeq = 0;
  int resnd = 0;

  for (;;) {
    n = recvfrom(sockid, (void*) &DG, sizeof(message_t), 0, 
      (struct sockaddr *)&remote_addr, &len);
      if (n < 0) exit (-1);

    unsigned int size;
    size = htonl(DG.num_values);

    for (unsigned int i = 0; i < size; i++) {
      unsigned element = htonl(DG.values[i]);
      list.push_back(element);
    }

    if (htonl(DG.sequence) != checkSeq) {
      resnd = checkSeq;
      checkSeq++;
    }
    checkSeq++;

    // SORT AND SEND NUMBERS BACK TO CLIENT
    if (htonl(DG.flag) == LAST) { // Last DG

      // Request Resends from Client
      if (resnd > 0) {
        DG.values[0]  = ntohl(resnd); // Sequence to request
        DG.num_values = ntohl(1);
        DG.sequence   = ntohl(0);
        DG.flag       = ntohl(RESEND);

        // Request resend to Client
        n = sendto(sockid, (void *) &DG, sizeof(message_t), 0, 
          (struct sockaddr *)&remote_addr, len);

        for (;;) {
        n = recvfrom(sockid, (void*) &DG, sizeof(message_t), 0, 
          (struct sockaddr *)&remote_addr, &len);
          if (htonl(DG.flag) == LAST) {  // Grab the fake last
            break;
          }
          for (unsigned int i = 0; i < htonl(DG.num_values); i++) {
            unsigned element = htonl(DG.values[i]);
            list.push_back(element);
          }
        }
      }
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
      checkSeq = 0;
      list.clear();
    }
  }
  close(sockid);
}

void SetList(vector<unsigned int> &list, vector<unsigned int> &sorted){
  list = sorted;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

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

  // Timeout set up?
  fd_set readfds;
  FD_SET(sockfd, &readfds);

//==================================================
  vector<unsigned int> sorted;
  vector<message_t> DG_send;

  //////////////////////////// SEND DATAGRAMS TO SERVER  ///////////////////////////

  for (std::vector<unsigned int> &list : lists) {
    unsigned int seq = 0; // Sequence for current list if Server requests Resends
    unsigned int num_DG = ceil((float)list.size() / (float)MAX_VALUES);

    // Make the DataGram(s) for this list
    for (unsigned int i = 0; i < num_DG; i++) {
      message_t DG;
      DG.sequence = htonl(seq++);
      unsigned int DG_start, DG_end;

      if (i == (num_DG - 1)) {// Last DG in List
        DG.flag = htonl(LAST);
        DG.num_values = htonl(list.size() - (MAX_VALUES * i));
        DG_end = list.size();
      }

      else {// Regular DG in List
        DG.flag = htonl(NONE);
        DG.num_values = htonl(MAX_VALUES);
        DG_end = (i+1) * MAX_VALUES;
      }

      // Fill current DG
      DG_start = i * MAX_VALUES;
      unsigned int DG_index = 0;
      for (unsigned int j = DG_start; j < DG_end; j++) {
        unsigned int element = htonl(list.at(j));
        DG.values[DG_index++] = element;
      }

      // Save DG for resends : NOTE element DG == seq#
      DG_send.push_back(DG);

      // Send DG
      n = sendto(sockfd, (void *)& DG, sizeof(message_t), 0, 
        (struct sockaddr *)&remote_addr, len);
      if (n < 0) exit(-1);
    }

  ///////////////////////// RECEIVE DATAGRAMS FROM SERVER  ////////////////////////////////
  
    vector <message_t> messages(seq);       //Predefined a Vector Size
    struct timeval tv;

    for (;;) {
      FD_SET(sockfd, &readfds);
      tv.tv_sec = 1;
      int tryReceive = select(sockfd + 1, &readfds, NULL, NULL, &tv);
      message_t on_wire;

      // Last goes missing Resend to Server..
      if (tryReceive == 0) {
        on_wire.values[0] = htonl(seq-1); // Resend the last
        on_wire.num_values = htonl(1);
        on_wire.sequence = htonl(0);
        on_wire.flag = htonl(RESEND);

        n = sendto(sockfd, (void *) &on_wire, sizeof(message_t), 0, 
          (struct sockaddr *)&remote_addr, len);
        continue;
      }

      n = recvfrom(sockfd, (void*)&on_wire, sizeof(message_t), 0, 
        (struct sockaddr *)&remote_addr, &len);
      if (n < 0) exit (-1);

      // Insert onwire into the vector at the position of the sequence - ignore fake lasts
      if (ntohl(on_wire.num_values) != 0) {
        messages[ntohl(on_wire.sequence)] = on_wire;
      }

      // Received the Last 
      if (ntohl(on_wire.flag) == LAST) {
        for (unsigned int i = 0; i < messages.size(); i++) {
          // If missing a sequence, send the resend message to server
          if (ntohl(messages[i].sequence) != i) {
            on_wire.values[0] = htonl(i); // Sequence to request

            on_wire.num_values = htonl(1);
            on_wire.sequence = htonl(0);
            on_wire.flag = htonl(RESEND);
            n = sendto(sockfd, (void *) &on_wire, sizeof(message_t), 0, 
              (struct sockaddr *)&remote_addr, len);

            for (;;) {
              n = recvfrom(sockfd, (void*)&on_wire, sizeof(message_t), 0, 
                (struct sockaddr *)&remote_addr, &len);
              if (ntohl(on_wire.flag) == LAST) {
                break;
              }
              messages[ntohl(on_wire.sequence)] = on_wire;
            }
          }
        }    
        break;
      }
    }

    // loop the vector of messages putting their contents into the original list
    for (auto msg : messages) {
      for (unsigned int i = 0; i < ntohl(msg.num_values); i++) {
        unsigned element = ntohl(msg.values[i]);
        sorted.push_back(element);
      }
    }

    // END OF SEND AND RECEIVE ON CURRENT LIST
    SetList(list, sorted);
    sorted.clear();     // Clear current sorted list
    DG_send.clear();    // Clear current list DG Sequence
  }
  close(sockfd);
}
