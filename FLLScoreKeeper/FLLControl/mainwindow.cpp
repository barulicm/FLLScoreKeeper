#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <iostream>
#include <QtNetwork>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;

string timeToString(const QTime& time)
{
    auto minString = to_string(time.minute());
    auto secString = to_string(time.second());
    return minString + ":" + (secString.size() < 2 ? "0" : "") + secString;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scoreEntryDialog(this)
{
    ui->setupUi(this);

    // set up game timer
    defaultTimerValue.setHMS(0, 2, 30);
    timerValue.setHMS(0, 2, 30);
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, [=](){
        timerValue.setHMS(0, timerValue.addSecs(-1).minute(), timerValue.addSecs(-1).second());
        ui->timer_display_lineEdit->setText(timeToString(timerValue).c_str());
        if(timerValue.minute() == 0 && timerValue.second() == 0) {
            // game over
            ui->timer_display_lineEdit->setReadOnly(false);
            ui->timer_start_stop_button->setText("Start");
            timer.stop();
            timerValue.setHMS(0, defaultTimerValue.minute(), defaultTimerValue.second());
            ui->timer_display_lineEdit->setText(timeToString(timerValue).c_str());
            ui->schedule_match_comboBox->setCurrentIndex(ui->schedule_match_comboBox->currentIndex()+1);
            schedule.advanceMatch();
        }
    });

    // disable selections and edits to table widget
    QPalette palette = ui->tableWidget->palette();
    palette.setBrush(QPalette::Highlight,QBrush(Qt::white));
    palette.setBrush(QPalette::HighlightedText,QBrush(Qt::black));
    ui->tableWidget->setPalette(palette);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // set default column widths for table widget
    ui->tableWidget->setColumnWidth(0,260);
    ui->tableWidget->setColumnWidth(1, 69);
    ui->tableWidget->setColumnWidth(2, 69);
    ui->tableWidget->setColumnWidth(3, 69);
    ui->tableWidget->setColumnWidth(4, 69);

    connect(&networkTimer, &QTimer::timeout, [=]() {
        auto nextTeams = schedule.getNextTeams();
        vector<int> nextTeamNumbers;
        for(const auto& team : nextTeams)
            nextTeamNumbers.push_back((team.get() != nullptr ? team->getNumber() : 0));
        server.sendData(ui->timer_display_lineEdit->text().toStdString(), schedule.getCurrentMatch(), nextTeamNumbers, teams);
    });
    networkTimer.start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_schedule_load_button_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Schedule File"), QDir::homePath(), tr("Comma Separated Values (*.csv)"));
    if(path.isEmpty()) {
        return;
    }
    schedule.clear();
    QFile scheduleFile(path);
    if(!scheduleFile.open(QIODevice::ReadOnly)) {
        return;
    }
    QTextStream stream(&scheduleFile);
    if(stream.readLine().split(",")[1] != "1") {
        QMessageBox msg(this);
        msg.setText("Unable to load schedule: Expected version number 1");
        return;
    }
    // Skip block format
    stream.readLine();
    // Load teams
    teams.clear();
    auto numberOfTeams = stoi(stream.readLine().split(",")[1].toStdString());
    for(auto i = 0; i < numberOfTeams; i++) {
        auto tokens = stream.readLine().split(",");
        shared_ptr<Team> team(new Team());
        team->setNumber(stoi(tokens[0].toStdString()));
        team->setName(tokens[1].toStdString());
        teams.push_back(team);
    }
    // Skip block format
    stream.readLine();

    auto numberOfMatches = stoi(stream.readLine().split(",")[1].toStdString());
    auto numberOfTables = stoi(stream.readLine().split(",")[1].toStdString());
    auto teamsPerTable = stoi(stream.readLine().split(",")[1].toStdString());

    // skip simultaneous tables
    stream.readLine();

    // table names
    vector<string> tableNames;
    auto tableNameTokens = stream.readLine().split(",");
    for(auto i = 0; i < numberOfTables; i++) {
        tableNames.push_back(tableNameTokens[3+i].toStdString());
    }
    schedule.setTables(tableNames);

    // populate matches
    for(auto match = 0; match < numberOfMatches; match++) {
        auto tokens = stream.readLine().split(",");
        for(int table = 0; table < numberOfTables; table++) {
            for(auto t = 0; t < teamsPerTable; t++) {
                auto teamNumber = stoi(tokens[3 + (table*teamsPerTable) + t].toStdString());
                shared_ptr<Team> team = getTeamForNumber(teamNumber);
                schedule.addTeamToMatch(team, match, table);
            }
        }
    }
    ui->schedule_match_comboBox->clear();
    for(auto i = 0; i < numberOfMatches; i++) {
        ui->schedule_match_comboBox->addItem(to_string(i+1).c_str());
    }
    schedule.setMatch(1);

    scheduleFile.close();

    ui->scoring_manual_team_combobox->clear();
    for(shared_ptr<Team> team : teams) {
        ui->scoring_manual_team_combobox->addItem(tr(to_string(team->getNumber()).c_str()));
    }
    updateTableWidget();
}

void MainWindow::on_timer_start_stop_button_clicked()
{
    // TODO play sounds
    if(timer.isActive()) {
        // timer is running
        ui->timer_display_lineEdit->setReadOnly(false);
        ui->timer_start_stop_button->setText("Start");
        timer.stop();
        timerValue.setHMS(0, defaultTimerValue.minute(), defaultTimerValue.second());
        ui->timer_display_lineEdit->setText(timeToString(timerValue).c_str());
    } else {
        // timer is not active
        ui->timer_display_lineEdit->setReadOnly(true);
        ui->timer_start_stop_button->setText("Stop");
        timer.start();
    }
}

void MainWindow::on_scoring_enter_button_clicked()
{
    scoreEntryDialog.show();
}

void MainWindow::on_scoring_manual_button_clicked()
{
    int teamNumber = stoi(ui->scoring_manual_team_combobox->currentText().toStdString());
    int score = ui->scoring_manual_score_spinbox->value();
    int round = stoi(ui->scoring_manual_round_combobox->currentText().toStdString());
    getTeamForNumber(teamNumber)->setScore(round, score);
    updateTableWidget();
}

void MainWindow::on_scoring_export_button_clicked()
{
    sortTeamsByHighestScore();
    auto path = QFileDialog::getSaveFileName(this, "Save Score File", QDir::homePath(), "Comma Separated Values (*.csv)");
    if(path.isEmpty()) {
        return;
    }
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)) {
        return;
    }
    QTextStream stream(&file);

    stream << "Rank,Team Number,Team Name,Highest Score,Match1,Match2,Match3,Match4" << endl;
    auto rank = 0;
    for(shared_ptr<Team>& team : teams) {
        rank++;
        stream << to_string(rank).c_str() << "," <<
                  to_string(team->getNumber()).c_str() << "," <<
                  team->getName().c_str() << "," <<
                  to_string(team->getHighestScore()).c_str() << "," <<
                  to_string(team->getScoreForRound(1)).c_str() << "," <<
                  to_string(team->getScoreForRound(2)).c_str() << "," <<
                  to_string(team->getScoreForRound(3)).c_str() << "," << endl;
    }
    stream.flush();
    file.close();
}

void MainWindow::on_schedule_match_comboBox_activated(const QString &arg1)
{
    schedule.setMatch(arg1.toInt());
}

std::shared_ptr<Team> MainWindow::getTeamForNumber(int number)
{
    for(shared_ptr<Team> team : teams) {
        if(team->getNumber() == number) {
            return team;
        }
    }
    return shared_ptr<Team>(nullptr);
}

void MainWindow::updateTableWidget()
{
    sortTeamsByHighestScore();
    ui->tableWidget->setRowCount((int)teams.size());
    int row = 0;
    for(shared_ptr<Team> team : teams) {
        ui->scoring_manual_team_combobox->addItem(tr(to_string(team->getNumber()).c_str()));
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(tr((to_string(team->getNumber()) + ", " + team->getName()).c_str())));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(tr(to_string(team->getHighestScore()).c_str())));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(tr(to_string(team->getScoreForRound(1)).c_str())));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(tr(to_string(team->getScoreForRound(2)).c_str())));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(tr(to_string(team->getScoreForRound(3)).c_str())));
        row++;
    }
}

struct HighestScoreComparator
{
    inline bool operator () (const shared_ptr<Team>& a, const shared_ptr<Team>& b)
    {
        return a->getHighestScore() > b->getHighestScore();
    }
};

void MainWindow::sortTeamsByHighestScore()
{
    sort(teams.begin(), teams.end(), HighestScoreComparator());
}

void MainWindow::on_timer_display_lineEdit_editingFinished()
{
    auto tokens = ui->timer_display_lineEdit->text().split(":");
    int minutes = stoi(tokens[0].toStdString());
    int seconds = stoi(tokens[1].toStdString());
    defaultTimerValue.setHMS(0,minutes,seconds);
    timerValue.setHMS(0,minutes,seconds);
}

void MainWindow::on_server_portNumber_lineEdit_editingFinished()
{
    server.setMulticastInformation(QHostAddress(ui->server_address_lineEdit->text()), ui->server_portNumber_lineEdit->text().toUShort());
}

void MainWindow::on_server_address_lineEdit_editingFinished()
{
    server.setMulticastInformation(QHostAddress(ui->server_address_lineEdit->text()), ui->server_portNumber_lineEdit->text().toUShort());
}
