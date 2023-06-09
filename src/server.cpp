#include "../headers/server.hpp"

ClientMessage::ClientMessage(int _ClientId, int _Number, int _TargetClient){
    ClientId = _ClientId ;
    Number = _Number ;
    TargetClient = _TargetClient ;
}

string ClientMessage::GetMessage(){
    string mess = "From Client : " + to_string(ClientId) + "\n"
         + "To Client : " + to_string(TargetClient) + "\n"
         + "Number : " + to_string(Number) + "\n";
    return mess ;
}

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

                    cout << endl << "JOIN :   -- > client " << NewSocket << " <--" << endl;
                }

                else
                { // recv message from client
                    try
                    {
                        GiveMessFromClient(i);
                    }
                    catch (string Error)
                    {
                        cout << Error << endl;
                        RemoveClient(i);                        
                    }
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
    string command;
    getline(cin, command);
    if ((command == " ") || (command == "\n") || (command == "") ) return true ;

    if( command ==  "1"){
        PrintArchive();
    }

    else if( command == "2" ){
        cout << "EXIT" << endl;
        return false;
    }

    else {
        cout << "   --> Bad Command <--" << endl;
    }

    return true;

}

void Server::GiveMessFromClient(int ClientFd)
{
    string mess = Recv(ClientFd);
    vector<string> commands = BreakString(mess, SEP);
    
    if (commands[0] == "send"){
        SendNumber(commands, ClientFd);
    }
    else if (commands[0] == "get"){
        string mess = "Online Clients :";
        cout << OnlineClient.size() << endl;
        for (int i = 0 ; i < OnlineClient.size() ; i++){
            mess += (" " + to_string(OnlineClient[i]));
        }
        Send(ClientFd, mess);
    }
    if (commands[0] == "give"){
        Send(ClientFd, to_string(ClientFd));
    }
}

void Server::SendNumber(vector<string> commands, int ClientFd){
    if (!IsClientExist(stoi(commands[1]))){
        Send(ClientFd, "    --> Error : The Clinet Does not Exist! <--");
        return ;
    }
    int target_client = stoi(commands[1]);
    string mess = "New Message : " + to_string(stoi(commands[2])+1) + "\nFrom : " + to_string(ClientFd) + "\n--------------------\n" ;
                    
    Send(target_client, mess);

    ClientMessage new_mess(ClientFd, stoi(commands[2])+1, target_client);
    ArchiveMessage.push_back(new_mess);
    cout << "--> New : " << endl << new_mess.GetMessage() << endl ;
}

string Server::Recv(int ClientFd)
{
    memset(Buffer, 0, MAX_LEN_MESS);
    int Status = recv(ClientFd, Buffer, MAX_LEN_MESS, 0);
    if (Status == 0) {
        string Error = "\n   --> Client " + to_string(ClientFd) + " Closed! <--\n"  ;
        throw Error ;
    }
    return string(Buffer);
}

void Server::Send(int ClientFd, string Message)
{
    send(ClientFd, Message.c_str(), Message.length(), 0);
}

void Server::PrintArchive(){
    if (ArchiveMessage.size() == 0){
        cout << "   --> There is no any message yet! <--" << endl;
        return ;
    }
    for(int i = 0 ; i < ArchiveMessage.size() ; i++){
        cout << ArchiveMessage[i].GetMessage();
        cout << "----------------------------------" << endl;
    }
    cout << endl ;
}

void Server::RemoveClient(int ClientId){
    close(ClientId);
    FD_CLR(ClientId, &MasterSet);
    for(int i = 0 ; i < OnlineClient.size() ; i++){
        if (OnlineClient[i] == ClientId)
            OnlineClient.erase(OnlineClient.begin() + i);
    }
}

bool Server::IsClientExist(int ClientId){
    for (int i = 0 ; i < OnlineClient.size() ; i++){
        if (ClientId == OnlineClient[i])
            return true;
    }
    return false;
}

vector<string> Server::BreakString(string str, char sep)
{
    istringstream ss(str);
    string word;
    vector<string> words;

    while (getline(ss, word, sep))
        words.push_back(word);
    return words;
}


int main(int argc, char const *argv[])
{
    Server Server(PORT);
    Server.Run();

    return 0;
}