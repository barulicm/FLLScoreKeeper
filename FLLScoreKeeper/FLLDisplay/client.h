#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <vector>
#include <team.h>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    void setMulticastInformation(QHostAddress address, quint16 port);

signals:

    void dataReady(std::string timerText, int currentMatch, std::vector<int> nextTeamNumbers, std::vector<Team> teams);

protected slots:

    void dataReceived();

private:
    QUdpSocket socket;
};

#endif // CLIENT_H
