//
//  sockets.h
//  Laig3
//
//  Created by Pedro Borges on 16/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <sstream>

#ifndef Laig3_sockets_h
#define Laig3_sockets_h
using namespace std;
class Socket {
#ifndef _WIN32
	int fd;
    struct sockaddr_in server_addr;
#else
    WORD wVersionRequested; // request Winsock 1.1
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    struct hostent *hp;
    //        char buf[BUFS];
    
#endif
public:
    Socket(string ip,int port){
#ifndef _WIN32
		bzero((char*)&server_addr,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());	
        server_addr.sin_port = htons(port);	
        fd= socket(AF_INET, SOCK_STREAM, 0);
        if(fd<0){
            printf("ERROR Creating socket\n");
            exit(-1);
        }
        if(connect(fd, 
                   (struct sockaddr *)&server_addr, 
                   sizeof(server_addr)) < 0){
            
            printf("Connection error\n");
            exit(-1);
        }
#else
		wVersionRequested = MAKEWORD(1,1);
        
        // Initialize Winsockets
        if (WSAStartup(wVersionRequested, &wsaData) == 0)       // allways needed
            printf("\nVersion allowed: %d.%d\n",
                   LOBYTE(wsaData.wHighVersion),HIBYTE(wsaData.wHighVersion));
        else
        {
            fprintf(stderr,"\n Wrong version\n");
            WSACleanup();
            exit(1);
        }
        
        /* Create socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET)
        {
            perror("Opening stream socket");
            exit(1);
        }
        setbuf(stdout, NULL);   // do not buffer output - DEBUGING!!
        
        /* Connect socket using server name indicated in the command line */
        server.sin_family = AF_INET;
        hp = gethostbyname(ip.c_str());
        if (hp == NULL)
        {
            perror("Trying to get host by name");
            exit(2);
        }
        memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);
        server.sin_port = htons((short)port);
        
        if (connect(sock, (struct sockaddr *)&server, sizeof server) == SOCKET_ERROR)
        {
            perror("Connecting stream socket");
            exit(1);
        }
#endif
        
        
    }
    string sendandreceive(const string & msg){
        string out="";
        cout<<"send: "<<msg<<endl;
#ifndef _WIN32
		write(fd, msg.c_str(), msg.size());
#else
		if (send (sock, msg.c_str(), msg.size(), 0) == SOCKET_ERROR)
            perror("Writing on stream socket");
#endif
        char c=0;
#ifdef _WIN32
		while(recv(sock,&c, 1, 0) &&c!='\n' ){
#else
            while(read(fd, &c, 1)>0 && c!='\n'){
#endif
                out+=c;
            }
            //cout<<"done: "<<out<<endl;
            return out;
            
        }
        
        
        void send2(const string & msg){
            string out="";
            cout<<"send: "<<msg<<endl;
#ifndef _WIN32
            write(fd, msg.c_str(), msg.size());
#else
            if (send (sock, msg.c_str(), msg.size(), 0) == SOCKET_ERROR)
                perror("Writing on stream socket");
#endif
           
                
            }
        string innerfunc(string s){
            stringstream st(s);
            
            char line[3000];
            st.getline(line, 3000, '(');
            st.getline(line,3000,')');
            string inner(line);
            return inner;
        }
        
        string cleanline(string s){
            string a="";
            stringstream sst(s);
            char c=0;
            bool cont=true;
            while(cont){
                cont=!(sst>> c).eof();
                if(cont){
                    if((c>='A' && c<='Z') || (c>='a' && c<='z')){
                        a+=c;
                    }
                }
            }
            return a;
            
        }
        
        vector<string> slitarray(string ss){
            vector<string> a;
            stringstream st(ss);
            string inner;
            char line[500];
            bool cont=true;
            while(cont){
                cont=!(st.getline(line, 500, ']')).eof();
                
                string nova(line);
                //cout<<nova<<endl;
                string cleaned=cleanline(nova);
               // cout<<cleaned<<endl;
                if(cleaned.size()!=0){
                    a.push_back(cleaned);
                }
            };
            
            
            return a;
            
        }
        void closes(){
#ifndef _WIN32
            close(fd);
#else
            #endif

        }
        
    };
    
#endif
