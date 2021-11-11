#ifndef READERTHREAD_H
#define READERTHREAD_H

#include <QThread>
#include <QWidget>
#include <QtSql>

class ReaderThread : public QThread
{
    Q_OBJECT
    void run();

signals:
    void progresso(double val);
    void log(QString log);

public:
    ReaderThread(QString filepath, QString tabOrigem, QString tabDest, QString primaryKey, QString newPrimaryKey, int step, QStringList dbInfo);
    QString filepath, tabOrigem, tabDestino, primaryKey, newPrimaryKey;
    QStringList dbInfo;
    int step;
    QSqlDatabase db;
    void gerarScript();
};

#endif // READERTHREAD_H
