#include "../headers/client.hpp"

Client::Client(int _port, string _ip)
{
    Port = _port;
    Ip = _ip;
    IsContinue = true;
}

void Client::Run()
{
    if( !ConnectToServer() ){
        cout << "   --> Fail to Connet Server <--" << endl;
        return ;
    }


    FD_ZERO(&MasterSet);
    MaxSd = ServerFd;
    FD_SET(ServerFd, &MasterSet);
    FD_SET(fileno(stdin), &MasterSet);

    cout << "> Write Command : " << endl
        << "--> send <Target Client Id> <Number>" << endl
        << "--> exit"
        << endl;

    while (IsContinue)
    {

        WorkingSet = MasterSet;
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
                { // Message From Server
                    try
                    {
                        string temp = Recv(ServerFd);
                        cout << temp << endl;
                    }
                    catch(string Error)
                    {
                        cout << Error << endl ;

                    }
                }

                else
                { // Problem
                    cout << "   --> Problem in Client <--" << endl;
                }
            }
        }

    }
}

bool Client::ConnectToServer()
{
    struct sockaddr_in server_address;

    ServerFd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Port);
    server_address.sin_addr.s_addr = inet_addr(Ip.c_str());

    if (connect(ServerFd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    { // checking for errors
        return false;
    }
    return true;
}


bool Client::GetFromBuffer(){
    string command;
    getline(cin, command);
    if ((command == " ") || (command == "\n") || (command == "") ) return true ;
    vector<string> commands = BreakString(command, ' ');

    if( commands[0] ==  "send"){
        cout << "SEND" << endl;
    }
    else if( commands[0] == "exit" ){
        cout << "EXIT" << endl;
        return false;
    }
    else {
        cout << "   --> Bad Command <--" << endl;
    }
    return true;

}

vector<string> Client::BreakString(string str, char sep)
{
    istringstream ss(str);
    string word;
    vector<string> words;

    while (getline(ss, word, sep))
        words.push_back(word);
    return words;
}

string Client::Recv(int ServerFd)
{
    char buffer[MAX_LEN_MESS] = {0};
    memset(buffer, 0, MAX_LEN_MESS);
    int Status = recv(ServerFd, buffer, MAX_LEN_MESS, 0);
    if (Status == 0) {
        string Error = "    --> Server is Closed <--";
        throw Error;
    }
    return string(buffer);
}

void Client::Send(int ServerFd, string mess)
{
    send(ServerFd, mess.c_str(), mess.length(), 0);
}

int main(){
    Client client(PORT, DEFULT_IP);
    client.Run();

    return 0;
}