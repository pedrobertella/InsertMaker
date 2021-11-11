#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->statusbar->showMessage("Generating inserts...");
    QStringList dbInfo;
    dbInfo << ui->nomeEdit->text();
    dbInfo << ui->ipEdit->text();
    dbInfo << ui->portaEdit->text();
    dbInfo << ui->userEdit->text();
    dbInfo << ui->senhaEdit->text();
    thread = new ReaderThread(ui->lineURL->text(), ui->lineOrigem->text(), ui->lineDestino->text(), ui->lineChave->text(), ui->lineChaveDest->text(), ui->spinBox->value(), dbInfo);
    connect(thread, &ReaderThread::progresso, this, &MainWindow::progresso);
    connect(thread, &ReaderThread::log, this, &MainWindow::log);
    connect(thread, &ReaderThread::finished, this, &MainWindow::threadFinished);

    ui->groupParam->setDisabled(true);
    ui->textEdit->clear();
    thread->start();
}

void MainWindow::on_btnOpen_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save...", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "SQL Script (*.sql)");
    ui->lineURL->setText(path);
}

void MainWindow::progresso(double val)
{
    ui->progressBar->setValue(ui->progressBar->value()+val);
}

void MainWindow::log(QString log)
{
    ui->textEdit->append(log + "\n");
}

void MainWindow::threadFinished()
{
    delete thread;
    QSqlDatabase::removeDatabase("ThreadDB");
    ui->groupParam->setDisabled(false);
    ui->statusbar->clearMessage();
    QMessageBox::information(this, "All Done!", "Finished Processing all Rows.");
    ui->progressBar->setValue(0);
}
