#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QRegExp>
#include <QPushButton>
#include <iostream>

OptionsDialog::OptionsDialog(Options *options, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog),
    options(options)
{
    ui->setupUi(this);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_buttonBox_accepted()
{
    options->multicast_address = ui->multicast_address_lineEdit->text();
    options->multicast_port = ui->multicast_port_lineEdit->text().toUShort();
}

void OptionsDialog::on_multicast_address_lineEdit_textChanged(const QString &text)
{
    QRegExp regex("^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$");
    if(regex.exactMatch(text)) {
        ui->multicast_address_lineEdit->setStyleSheet("color:black");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        ui->multicast_address_lineEdit->setStyleSheet("color:red");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void OptionsDialog::on_multicast_port_lineEdit_textChanged(const QString &text)
{
    QRegExp regex("^[0-9]+$");
    if(regex.exactMatch(text)) {
        auto number = text.toInt();
        if(number >= 0 && number <= 65535) {
            ui->multicast_port_lineEdit->setStyleSheet("color:black");
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        } else {
            ui->multicast_port_lineEdit->setStyleSheet("color:red");
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    } else {
        ui->multicast_port_lineEdit->setStyleSheet("color:red");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
