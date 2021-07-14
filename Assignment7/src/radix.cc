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

// UDP:
// Packets can go missing
// Packetss can arrive out of order 
void RadixServer::start(const int port) {

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

//==================================================

  socklen_t len = sizeof(remote_addr);
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

    for (unsigned int i = 0; i < num_DG; i++) {
      // int seq_rec = i;
      message_t on_wire; 
      check = recvfrom(sockfd, (void*)&on_wire, sizeof(message_t), 0, 
        (struct sockaddr *)&remote_addr, &len);
      if (check < 0) exit (-1);

      message_t received = on_wire;
      unsigned int size, flag, sequence;
      size = ntohl(received.num_values);
      flag = ntohl(received.flag);
      sequence = ntohl(received.sequence);

      // if (seq_rec != sequence) {  //Dropped packet, ask for the packet from the server
      //   check = recvfrom(sockfd, (void*)&on_wire, sizeof(message_t), 0, 
      //   (struct sockaddr *)&remote_addr, &len);
      //   if (check < 0) exit (-1);
      //   i--;
      // }

      for (unsigned int i = 0; i < size; i++) {
        unsigned element = ntohl(received.values[i]);
        sorted.push_back(element);
      }
      if (flag == 1)    // End of list
        break;

      if (flag == 2) {  // Server Resends Datagram at the sequence requested in current list
        check = sendto(sockfd, (void *)& datagrams.at(sequence), sizeof(message_t), 0, 
          (struct sockaddr *)&remote_addr, len);
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
