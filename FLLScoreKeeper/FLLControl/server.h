#ifndef SERVER_H
#define SERVER_H

#include <QUdpSocket>
#include <vector>
#include <memory>
#include "team.h"

class Server : public QObject
{
    Q_OBJECT

public:
    Server();

    void sendData(std::string timerText, int currentMatch, std::vector<int> nextMatchTeamNumbers, std::vector<std::shared_ptr<Team>>& teams);

    void setMulticastInformation(QHostAddress address, quint16 port);

private:
    QUdpSocket socket;

    QHostAddress address;

    quint16 port;
};

#endif // SERVER_H
