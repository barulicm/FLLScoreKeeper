#include "server.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;

Server::Server()
    : socket(this),
      address("239.255.1.1"),
      port(8008)
{
}

void Server::sendData(string timerText, int currentMatch, std::vector<int> nextMatchTeamNumbers, vector<shared_ptr<Team>>& teams)
{
    QJsonObject jsonMessage;

    jsonMessage.insert("TimerText", timerText.c_str());
    jsonMessage.insert("CurrentMatch", currentMatch);
    QJsonArray jsonNextMatch;
    for(auto &number : nextMatchTeamNumbers) {
        jsonNextMatch.push_back(number);
    }
    jsonMessage.insert("NextMatchTeamNumbers", jsonNextMatch);

    QJsonArray jsonTeams;
    for(shared_ptr<Team> &team : teams) {
        QJsonObject jsonTeam;
        jsonTeam.insert("Number", team->getNumber());
        jsonTeam.insert("Name", team->getName().c_str());
        jsonTeam.insert("Round1", team->getScoreForRound(1));
        jsonTeam.insert("Round2", team->getScoreForRound(2));
        jsonTeam.insert("Round3", team->getScoreForRound(3));
        jsonTeams.append(jsonTeam);
    }
    jsonMessage.insert("Teams", jsonTeams);

    socket.writeDatagram(QJsonDocument(jsonMessage).toBinaryData(), address, port);
}

void Server::setMulticastInformation(QHostAddress address, quint16 port)
{
    this->address = address;
    this->port = port;
}
