#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <readerthread.h>
#include <QtSql>

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
    void on_pushButton_clicked();

    void on_btnOpen_clicked();

    void progresso(double val);
    void log(QString log);
    void threadFinished();

private:
    Ui::MainWindow *ui;
    ReaderThread *thread = nullptr;
};
#endif // MAINWINDOW_H
