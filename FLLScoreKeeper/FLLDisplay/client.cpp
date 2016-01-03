#include "client.h"
#include <iostream>

using namespace std;

Client::Client(QObject *parent)
    : QObject(parent),
      socket(this)
{   
    socket.bind(QHostAddress::AnyIPv4, 8008, QUdpSocket::ShareAddress);
    socket.joinMulticastGroup(QHostAddress("239.255.1.1"));

    connect(&socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

void Client::setMulticastInformation(QHostAddress address, quint16 port)
{
    socket.close();
    socket.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    socket.joinMulticastGroup(address);
}

void Client::dataReceived()
{
    while (socket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket.pendingDatagramSize());
        socket.readDatagram(datagram.data(), datagram.size());
        if(datagram.isEmpty()) {
            continue;
        }

        QStringList dataTokens = QString(datagram.toStdString().c_str()).split("|");

        std::string timerText = dataTokens[0].toStdString();
        dataTokens.erase(dataTokens.begin());

        int currentMatch = stoi(dataTokens[0].toStdString());
        dataTokens.erase(dataTokens.begin());

        vector<int> nextTeamNumbers;
        if(!dataTokens[0].isEmpty()) {
            QStringList nextTeamTokens = dataTokens[0].split(",");
            for(QString& token : nextTeamTokens) {
                nextTeamNumbers.push_back(token.toInt());
            }
        } else {
            nextTeamNumbers = {0,0,0,0,0,0};
        }
        dataTokens.erase(dataTokens.begin());

        vector<Team> teams;
        for(QString& teamToken : dataTokens) {
            if(teamToken.isEmpty())
                continue;
            auto teamTokens = teamToken.split(",");
            Team team;
            team.setNumber(stoi(teamTokens[0].toStdString()));
            team.setName(teamTokens[1].toStdString());
            team.setScore(1,stoi(teamTokens[2].toStdString()));
            team.setScore(2,stoi(teamTokens[3].toStdString()));
            team.setScore(3,stoi(teamTokens[4].toStdString()));
            teams.push_back(team);
        }
        emit dataReady(timerText, currentMatch, nextTeamNumbers, teams);
    }
}
