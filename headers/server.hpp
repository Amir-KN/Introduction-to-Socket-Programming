#ifndef _SERVER_
#define _SERVER_

#include "INCLUDE.hpp"

class ClientMessage{
    public :
        ClientMessage(int _ClientId, int _Number, int _TargetClient);
        string GetMessage();

    private :
        int ClientId;
        int Number;
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

    void PrintArchive();

    void SendNumber(vector<string> commands, int ClientFd);

    vector<string> BreakString(string str, char sep);

    bool IsClientExist(int ClientId);
    
};

#endif