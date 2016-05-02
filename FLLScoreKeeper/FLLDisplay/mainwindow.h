#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "client.h"
#include "team.h"

// TODO add context menu for options access

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event);

protected slots:

    void clientDataReady(std::string timerText, int currentMatch, std::vector<int> nextTeamNumbers, std::vector<Team> teams);

    void showContextMenu(const QPoint& pos);

private:
    Ui::MainWindow *ui;

    Client client;

};

#endif // MAINWINDOW_H
