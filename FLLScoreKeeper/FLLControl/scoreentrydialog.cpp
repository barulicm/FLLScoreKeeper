#include "scoreentrydialog.h"
#include "ui_scoreentrydialog.h"
#include <QDir>
#include <QWebElement>
#include <QWebFrame>
#include <iostream>

using namespace std;

ScoreEntryDialog::ScoreEntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScoreEntryDialog)
{
    ui->setupUi(this);
}

ScoreEntryDialog::~ScoreEntryDialog()
{
    delete ui;
}

void ScoreEntryDialog::on_pushButton_close_clicked()
{
    this->close();
}

void ScoreEntryDialog::on_pushButton_save_clicked()
{

}
