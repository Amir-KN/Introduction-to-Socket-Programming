#ifndef _CLIENT_
#define _CLIENT_

#include "INCLUDE.hpp"


class Client
{
public:
    Client(int _port, string _ip);
    void Run();

private:
    int ServerFd, Port, MaxSd;
    string Ip;
    bool IsContinue;
    fd_set MasterSet, WorkingSet;

    bool ConnectToServer();

    bool GetFromBuffer();

    string Recv(int ServerFd);

    void Send(int ServerFd, string mess);

    

    vector<string> BreakString(string str, char sep);

};

#endif