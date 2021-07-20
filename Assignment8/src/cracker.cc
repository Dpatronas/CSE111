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

#include "cracker.h"
using namespace std;

//NOTES:
  // Port to communicate between crackers
  //   unsigned int betweenServers = get_unicast_port();

// Citations:
// Professor Harrison: https://drive.google.com/drive/u/0/folders/1j38fDIem-sRltX_sjQl5jPeeRSinvxsk

// Single server embarassingly parallel implementation

// Multicast Receiver to grab encrypted password message
static void Receive(Message *msg) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { cout << "Socket failed to create"; exit(-1); }

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(get_multicast_port());
    bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)); 

    struct ip_mreq multicastRequest;  
    multicastRequest.imr_multiaddr.s_addr = get_multicast_address();
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest, sizeof(multicastRequest)); 

    socklen_t len = sizeof(server_addr);
    recvfrom(sockfd, (void*) msg, sizeof(message_t), 0, (struct sockaddr *)&server_addr, &len);
    // cout << "Message for: " << msg->cruzid << endl;

    close(sockfd);
}

// TCP Sender to send decrypted password message
static void Send(Message *msg) {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *server = gethostbyname(msg->hostname);
    if (server == NULL) exit (-1);

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = msg->port;

    if (connect(sockfd,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
        { cout << "Connect Unsuccessful" << endl; exit(-1); }

    int n = send(sockfd, (void *)msg, sizeof(message_t), 0);
    if (n < 0) { cout << "Send Unsuccessful"; exit(-1); } exit(-1);

    close(sockfd);
}

void JoinThreads(std::vector<std::thread *> &Threads) {
  for(auto& t: Threads) {
      t->join();
  }
}

// Iterate through passwrd hashes - replaces encrypted passwords with decrypted ones
static void Decrypt(Message *msg) {
    
    vector <std::thread *> Threads;
    vector <char *> hashes;

    for ( unsigned int i = 0; i < htonl(msg->num_passwds); i++ ) {
        char* hash = (char *)malloc(HASH_LENGTH+1);
        if (!hash) 
            { cout << "Bad malloc" << endl; exit(-1); }
            
        hashes.push_back(hash);
        memcpy(hash, msg->passwds[i], HASH_LENGTH+1);

        std::thread * t = new std::thread(crack, msg->alphabet, hashes[i], msg->passwds[i]);
        Threads.push_back(t);
    }
    JoinThreads(Threads);

    for ( unsigned int i = 0; i < hashes.size(); i++ ) {
        free(hashes[i]);
        delete Threads[i];
    }
}

// Cracker
int main(int argc, char *argv[]) {
  Message msg;
  Receive(&msg);
  Decrypt(&msg);
  Send(&msg);
}
