# Introduction-to-Socket-Programming
# Overview
in this project we want to familiar with `Socket Programming`. Client after connecting to server, send a number for another client. this transportation done with help of server. First server receive the number from source client and then send `number + 1` to target client.
 
  - [`Architecture`](#architecture)
  - [`Client Class`](#client-class)
  - [`Server Class`](#server-class)
  - [`Project contents`](#project-contents)
  - [`How To Run`](#how-to-run)

## Architecture
Client connect to server with **Socket** and we use from [sockent.h](https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html) library.
Because server should handle more than one client, we use `select()` to manage more than one client.
We have 2 important class :
  - **Client Class** : This class help us to first connect to server and then handle command of client.
  - **Server Class** : This class handle server and response the request of clients.


Also in Server class, we have a vector of **ClientMessage** class that save all message between clients.

## Client Class
``` c++
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
    void Send(int ServerFd, string mess);\
    void SendToServer(vector<string> commands, string command);
    bool IsDigit(string str);
    vector<string> BreakString(string str, char sep);
};
```
To use of this class, when the server run we should call the `Run()` methods. As you see, we comply **Encapsulation**.


## Server Class
``` c++
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
```
To use of this class, after creating a instance of class, should call `Run()` method to setup server and then clients can connect to servre.
Note that the number of client that can connect to server define in `headers/INCLUDE.hpp` :
``` c++
const int MAX_CLIENT = 5;
```
As you see, we comply **Encapsulation**.


## Project contents
- **`heades`** FOLDER
    - *client.hpp*
    - *server.hpp*
    - *INCLUDE.hpp*
- **`src`** FOLDEF
    - *client.cpp*
    - *server.cpp*
- **`makefile`**

In INCLUDE.hpp we include the libraries and define the const vars.

## How To Run
For creating the execution files `server.out` & `client.out` you should run `make` in terminal. After that for running the server you should write `./server.out` 
in terminal, then you can run `./client.out` in another terminals to connect new client to server.

