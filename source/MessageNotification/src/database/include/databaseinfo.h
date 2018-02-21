#pragma once
#ifndef __DATABASE_INFO_H__
#define __DATABASE_INFO_H__
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseInfo : public QObject {
  Q_OBJECT
public:
  DatabaseInfo(QString _databaseType,
               QString _hostName,
               QString _databaseName,
               QString _userName,
               QString _password) :
    databaseType(_databaseType),
    hostName(_hostName),
    databaseName(_databaseName),
    userName(_userName),
    password(_password) {}

public:
  QString databaseType;
  QString hostName;
  QString databaseName;
  QString userName;
  QString password;
};

#endif //__DATABASE_INFO_H__
