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

    connect(&client, &Client::dataReady, this, &MainWindow::clientDataReady);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::showContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clientDataReady(std::string timerText, int currentMatch, std::vector<int> nextTeamNumbers, std::vector<Team> teams)
{
    ui->timer_label->setText(timerText.c_str());
    ui->currentMatch_label->setText( ("Current Match: " + to_string(currentMatch)).c_str());

    vector<QLabel*> labels = { ui->next_A_label,
                               ui->next_B_label,
                               ui->next_C_label,
                               ui->next_D_label,
                               ui->next_E_label,
                               ui->next_F_label };
    vector<string> letters = { "A", "B", "C", "D", "E", "F" };

    for(size_t i = 0; i < nextTeamNumbers.size(); i++) {
        labels[i]->setText( (letters[i] + ": " + to_string(nextTeamNumbers[i])).c_str());
    }
    for(size_t i = nextTeamNumbers.size(); i < 6; i++) {
        labels[i]->hide();
    }

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
