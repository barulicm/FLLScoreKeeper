#ifndef SCOREENTRYDIALOG_H
#define SCOREENTRYDIALOG_H

#include <QDialog>

namespace Ui {
class ScoreEntryDialog;
}

class ScoreEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScoreEntryDialog(QWidget *parent = 0);
    ~ScoreEntryDialog();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::ScoreEntryDialog *ui;
};

#endif // SCOREENTRYDIALOG_H
