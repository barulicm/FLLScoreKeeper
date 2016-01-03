#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

struct Options
{
    QString multicast_address;
    quint16 multicast_port;
};

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(Options *options, QWidget *parent = 0);
    ~OptionsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_multicast_address_lineEdit_textChanged(const QString &text);

    void on_multicast_port_lineEdit_textChanged(const QString &text);

private:
    Ui::OptionsDialog *ui;

    Options *options;
};

#endif // OPTIONSDIALOG_H
