#ifndef TEMPSQL_H
#define TEMPSQL_H


#include <QObject>
#include <QSqlDatabase>


class TagData
    {
    public:
        QString TagId;
        QString Name;
    };


class DatabaseManager : public QObject
    {
    public:
        DatabaseManager(QObject *parent = 0);
        ~DatabaseManager();

    public:
        bool openDB();
        bool getTag(QString str, TagData *& data);

    private:
//        QSqlDatabase db;
    };











#endif // TEMPSQL_H
