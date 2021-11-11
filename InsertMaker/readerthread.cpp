#include "readerthread.h"
#include <QFile>
#include <QTextStream>

void ReaderThread::run()
{
    db.setDatabaseName(dbInfo[0]);
    db.setHostName(dbInfo[1]);
    db.setPort(dbInfo[2].toInt());
    db.setUserName(dbInfo[3]);
    db.setPassword(dbInfo[4]);
    if(db.open()){
        emit log("The database connection was successfully established!");
    }else{
        emit log("The database connection failed:\n" + db.lastError().text());
        return;
    }
    gerarScript();
}

ReaderThread::ReaderThread(QString filepath, QString tabOrigem, QString tabDest, QString primaryKey, QString newPrimaryKey, int step, QStringList dbInfo)
{
    this->filepath = filepath;
    this->tabOrigem = tabOrigem;
    this->tabDestino = tabDest;
    this->step = step;
    this->primaryKey = primaryKey;
    this->newPrimaryKey = newPrimaryKey;
    this->dbInfo = dbInfo;
    db = QSqlDatabase::addDatabase("QPSQL", "ThreadDB");
}

void ReaderThread::gerarScript()
{

    QFile file(filepath);
    if(!file.open(QIODevice::Append)){
        emit log("COULD NOT OPEN THE FILE");
        return;
    }
    QTextStream stream(&file);

    int regCount;
    emit log("Checking the row count...");
    QSqlQuery q(db);
    q.exec("SELECT COUNT(*) FROM "+tabOrigem+";");
    if(q.lastError().isValid()){
        emit log("QUERY ERROR: " + q.lastError().text());
        emit log(q.executedQuery());
        return;
    }
    q.next();
    regCount = q.value(0).toInt();

    emit log("Row count: " + QString::number(regCount));

    int progCount = 0;
    int offset = 0;
    QString insert;
    emit log("Processing...");
    do{
        q.exec("SELECT * FROM "+tabOrigem+" ORDER BY "+primaryKey+" ASC LIMIT "+QString::number(step)+" OFFSET "+QString::number(offset)+";");
        //emit log(q.executedQuery());
        if(q.lastError().isValid()){
            emit log("QUERY ERROR: " + q.lastError().text());
            return;
        }

        while(q.next()){
            insert.clear();
            insert.append("INSERT INTO "+tabDestino+" (");
            QSqlRecord rec = q.record();

            bool isFirst = true;
            for(int j = 0; j < rec.count(); j++){
                if(isFirst){
                    isFirst = false;
                }else{
                    insert.append(",");
                }

                insert.append(rec.field(j).name());
            }

            insert.append(") VALUES (");

            isFirst = true;
            for(int j = 0; j < rec.count(); j++){
                if(isFirst){
                    isFirst = false;
                }else{
                    insert.append(",");
                }
                if(rec.field(j).type() == QVariant::String){
                    insert.append("'"+rec.field(j).value().toString()+"'");
                }else if(rec.field(j).type() == QVariant::Date){
                    insert.append("'"+rec.field(j).value().toDate().toString("yyyy-MM-dd")+"'");
                }else{
                    insert.append(rec.field(j).value().toString());
                }
            }
            insert.append(");\n");
            insert.replace(primaryKey, newPrimaryKey);

            stream << insert;
            progCount++;
            if(progCount>=regCount/100){
                progCount=0;
                emit progresso(1);
            }
        }

        offset+=step;

    }while(q.size() == step);

}
