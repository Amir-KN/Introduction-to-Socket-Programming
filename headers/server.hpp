#ifndef _SERVER_
#define _SERVER_

#include "INCLUDE.hpp"

class ClientMessage{
    public :

    private :
        int ClientId;
        string Message;
        int TargetClient;
};

class Server
{
public:
    Server(int _port);
    void Run();

private:
    int ServerFd, NewSocket, MaxSd, Port;
    char Buffer[MAX_LEN_MESS] = {0};
    bool IsContinue;
    fd_set MasterSet, WorkingSet;
    vector<ClientMessage> ArchiveMessage;
    vector<int> OnlineClient;

    bool SetupServer();

    int AcceptClient(int ServerFd);

    bool GetFromBuffer();

    void GiveMessFromClient(int client_fd);

    void RemoveClient(int ClientId);

    string Recv(int ClientFd);

    void Send(int ClientFd, string Message);
};

#endif