#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QMenu>
#include <iostream>
#include "optionsdialog.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->grabKeyboard();

    connect(&client, &Client::dataReady, this, &MainWindow::on_client_dataReady);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::showContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_client_dataReady(string timerText, int currentMatch, std::vector<int> nextTeamNumbers, vector<Team> teams)
{
    ui->timer_label->setText(timerText.c_str());
    ui->currentMatch_label->setText( ("Current Match: " + to_string(currentMatch)).c_str());

    ui->next_A_label->setText( ("A: " + to_string(nextTeamNumbers[0])).c_str() );
    ui->next_B_label->setText( ("B: " + to_string(nextTeamNumbers[1])).c_str() );
    ui->next_C_label->setText( ("C: " + to_string(nextTeamNumbers[2])).c_str() );
    ui->next_D_label->setText( ("D: " + to_string(nextTeamNumbers[3])).c_str() );
    ui->next_E_label->setText( ("E: " + to_string(nextTeamNumbers[4])).c_str() );
    ui->next_F_label->setText( ("F: " + to_string(nextTeamNumbers[5])).c_str() );

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(teams.size());
    auto row = 0;
    for(Team& team : teams)
    {
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem( (to_string(team.getNumber()) + ", " + team.getName()).c_str() ));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(to_string(team.getHighestScore()).c_str()));\
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(to_string(team.getScoreForRound(1)).c_str()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(to_string(team.getScoreForRound(2)).c_str()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(to_string(team.getScoreForRound(3)).c_str()));
        row++;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F || event->key() == Qt::Key_Escape) {
        if(this->isFullScreen()) {
            this->showNormal();
        } else {
            this->showFullScreen();
        }
    } else {
        event->ignore();
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Options");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem) {
        if(selectedItem->text() == "Options") {
            Options *options = new Options;
            OptionsDialog dialog(options, this);
            this->releaseKeyboard();
            if(dialog.exec() == QDialog::Accepted) {
                client.setMulticastInformation(QHostAddress(options->multicast_address),options->multicast_port);
            }
            delete options;
            this->grabKeyboard();
        }
    }
}
