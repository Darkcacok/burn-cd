#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "writeiso.h"

#include <QMainWindow>
#include <QMdiArea>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_write_iso_clicked();

private:
    Ui::MainWindow *ui;
    WriteISO *write_iso;
};

#endif // MAINWINDOW_H
