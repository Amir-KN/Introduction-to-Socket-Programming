#include "../headers/server.hpp"

Server::Server(int _port)
{
    IsContinue = true ;
    Port = _port ;    
}

void Server::Run()
{
    if (!SetupServer())
    {
        cout << "*** Server Run Before ***" << endl;
        return;
    }

    FD_ZERO(&MasterSet);
    MaxSd = ServerFd;
    FD_SET(ServerFd, &MasterSet);
    FD_SET(fileno(stdin), &MasterSet);

    cout << "*** Welcome to Program ***" << endl;

    while (IsContinue)
    {
        WorkingSet = MasterSet;
        cout << "> Enter Number Command :" << endl
             << "    --> 1. Get Archive Message" << endl
             << "    --> 2. Close Server" << endl;

        select(MaxSd + 1, &WorkingSet, NULL, NULL, NULL);
        
        for (int i = 0; i <= MaxSd; i++)
        {
            if (FD_ISSET(i, &WorkingSet))
            {
                if (i == 0)
                { // get input from user with command
                    IsContinue = GetFromBuffer();
                }

                else if (i == ServerFd)
                { // new clinet
                    NewSocket = AcceptClient(ServerFd);
                    FD_SET(NewSocket, &MasterSet);
                    if (NewSocket > MaxSd)
                        MaxSd = NewSocket;
                    OnlineClient.push_back(NewSocket);

                    cout << "New Client Connected. fd = " << NewSocket << endl;
                }

                else
                { // recv message from client
                    GiveMessFromClient(i);
                }
            }
        }
    }

}

bool Server::SetupServer()
{
    struct sockaddr_in address;
    ServerFd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(ServerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(Port);

    int temp = bind(ServerFd, (struct sockaddr *)&address, sizeof(address));
    if (temp == -1)
    {
        close(ServerFd);
        return false;
    }

    listen(ServerFd, MAX_CLIENT);
    return true;
}

int Server::AcceptClient(int ServerFd)
{
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(ServerFd, (struct sockaddr *)&client_address, (socklen_t *)&address_len);
    return client_fd;
}

bool Server::GetFromBuffer()
{


}

void Server::GiveMessFromClient(int ClientFd)
{

}


string Server::Recv(int ClientFd)
{
    memset(Buffer, 0, MAX_LEN_MESS);
    recv(ClientFd, Buffer, MAX_LEN_MESS, 0);
    return string(Buffer);
}

void Server::Send(int ClientFd, string Message)
{
    send(ClientFd, Message.c_str(), Message.length(), 0);
}

void RemoveClient(int ClientId){

}

int main(int argc, char const *argv[])
{
    Server Server(PORT);
    Server.Run();

    return 0;
}