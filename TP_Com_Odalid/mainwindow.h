#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma comment(lib, "ODALID.lib")
#include <QMainWindow>
#include "ODALID.h"
#include <QtGui>
#define MI_OK                           0


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_Connect_clicked();

    void on_button_Saisie_clicked();

    void on_button_Quitter_clicked();


private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
