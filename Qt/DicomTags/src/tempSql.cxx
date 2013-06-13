#include "tempSql.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDir>
#include <QDebug>

bool DatabaseManager::getTag(QString str, TagData*& data)
    {
    bool ret = false;

    QSqlDatabase db = QSqlDatabase::database("dicom_query");

    QSqlQuery query(db);
    QString execstr("SELECT * FROM dicom_nema where TagID = ");
    execstr.append(QString("'")+str+QString("'"));

    qDebug() << execstr;

    query.exec(execstr);
    //    query.exec("SELECT * FROM dicom_nema where TagID = '0002,0000'");
    //    qDebug() << query.lastError().text();

    while (query.next())
    {
        data->TagId = query.value(0).toString();
        data->Name = query.value(1).toString();
        //        qDebug() << data->TagId << data->Name;
        ret = true;
    }

    return ret;
    }




bool DatabaseManager::openDB()
    {
    // Find QSLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","dicom_query");

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("dicom_nema");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    QString path(QDir::currentPath());
    path.append(QDir::separator()).append("dicom.sqlite");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    qDebug() << path;
    #endif


    // Open databasee
    return db.open();
    }
