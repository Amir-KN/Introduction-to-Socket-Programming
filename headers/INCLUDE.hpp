#ifndef _INCLUDE_
#define _INCLUDE_

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const string DEFULT_IP = "127.0.0.1" ;
const int PORT = 8000;
const int MAX_LEN_MESS = 256 ;
const int MAX_CLIENT = 5;
const char SEP = ' ' ;


#endif