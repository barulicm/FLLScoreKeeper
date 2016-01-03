#include "server.h"
#include <iostream>

using namespace std;

Server::Server()
    : socket(this)
{
}

void Server::sendData(string timerText, int currentMatch, std::vector<int> nextMatchTeamNumbers, vector<shared_ptr<Team>>& teams)
{
    string data;
    data += timerText + "|";
    data += to_string(currentMatch) + "|";
    for(const int& teamNumber : nextMatchTeamNumbers) {
        data += to_string(teamNumber) + ",";
    }
    data += "|";
    for(shared_ptr<Team>& team : teams) {
        data += to_string(team->getNumber()) + "," +
                team->getName() + "," +
                to_string(team->getScoreForRound(1)) + "," +
                to_string(team->getScoreForRound(2)) + "," +
                to_string(team->getScoreForRound(3)) + "|";
    }
    socket.writeDatagram(QByteArray::fromStdString(data), address, port);
}

void Server::setMulticastInformation(QHostAddress address, quint16 port)
{
    this->address = address;
    this->port = port;
}
