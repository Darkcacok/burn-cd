#ifndef WRITEISO_H
#define WRITEISO_H

#include "burn.h"

#include <QDialog>
#include <QFileDialog>
#include <QString>


namespace Ui {
class WriteISO;
}

class WriteISO : public QDialog
{
    Q_OBJECT

public:
    explicit WriteISO(QWidget *parent = nullptr);
    ~WriteISO();

private slots:
    void on_choose_clicked();

    void on_burn_clicked();

private:
    Ui::WriteISO *ui;
    QString path_to_file;
    Burn burn;
};

#endif // WRITEISO_H
