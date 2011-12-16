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
#include <string>
#include <iostream>
#include <fstream>
#ifndef Laig3_sockets_h
#define Laig3_sockets_h
using namespace std;
class Socket {
    int fd;
    struct sockaddr_in server_addr;
public:
    Socket(string ip,int port){
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
        
        
        
    }
    string sendandreceive(const string & msg){
        string out="";
        write(fd, msg.c_str(), msg.size());
        char c=0;
        while(read(fd, &c, 1)>0 && c!='\n'){
            out+=c;
        }
        //cout<<"done: "<<out<<endl;
        return out;
    
    }
    
};

#endif
