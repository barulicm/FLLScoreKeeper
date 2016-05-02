#include "client.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;

Client::Client(QObject *parent)
    : QObject(parent),
      socket(this)
{   
    socket.bind(QHostAddress::AnyIPv4, 8008, QUdpSocket::ShareAddress);
    socket.joinMulticastGroup(QHostAddress("239.255.1.1"));

    connect(&socket, &QUdpSocket::readyRead, this, &Client::dataReceived);
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

        QJsonDocument jsonDoc = QJsonDocument::fromBinaryData(datagram);

        QJsonObject jsonMessage = jsonDoc.object();

        string timerText = jsonMessage["TimerText"].toString().toStdString();

        int currentMatch = jsonMessage["CurrentMatch"].toInt();

        vector<int> nextTeamNumbers;
        auto jsonNextTeams = jsonMessage["NextMatchTeamNumbers"].toArray();
        for(auto value : jsonNextTeams) {
            nextTeamNumbers.push_back(value.toInt());
        }

        vector<Team> teams;
        auto jsonTeams = jsonMessage["Teams"].toArray();
        for(auto value : jsonTeams) {
            Team team;
            QJsonObject valueObj = value.toObject();
            team.setName(valueObj["Name"].toString().toStdString());
            team.setNumber(valueObj["Number"].toInt());
            team.setScore(1, valueObj["Round1"].toInt());
            team.setScore(2, valueObj["Round2"].toInt());
            team.setScore(3, valueObj["Round3"].toInt());
            teams.push_back(team);
        }

        emit dataReady(timerText, currentMatch, nextTeamNumbers, teams);
    }
}
