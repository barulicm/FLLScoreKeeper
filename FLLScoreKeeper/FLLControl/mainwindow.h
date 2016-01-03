#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "schedule.h"
#include "server.h"
#include "scoreentrydialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_schedule_load_button_clicked();

    void on_timer_start_stop_button_clicked();

    void on_server_portNumber_lineEdit_editingFinished();

    void on_scoring_enter_button_clicked();

    void on_scoring_manual_button_clicked();

    void on_scoring_export_button_clicked();

    void on_schedule_match_comboBox_activated(const QString &arg1);

    void on_timer_display_lineEdit_editingFinished();

    void on_server_address_lineEdit_editingFinished();

private:
    Ui::MainWindow *ui;

    QTimer timer;
    QTime timerValue;
    QTime defaultTimerValue;

    QTimer networkTimer;

    Schedule schedule;

    std::vector<std::shared_ptr<Team>> teams;

    Server server;

    ScoreEntryDialog scoreEntryDialog;

    std::shared_ptr<Team> getTeamForNumber(int number);

    void updateTableWidget();

    void sortTeamsByHighestScore();
};

#endif // MAINWINDOW_H
