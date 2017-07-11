#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QDate>
#include <QVector>
#include <QList>
#include <QTableWidgetItem>

class Entry
{
public:
    QString firstname;
    QString lastname;
    QString phone;
    QString notes;
    QDate date;
    int hour;
    int lane;
    bool payment;
    double price;
    QDateTime receptiontime;
    QString recipient;
};

class DataManager
{
private:
    QVector<Entry> entries;
public:
    DataManager();
    void Save();
    void Load();
    void AddEntry(Entry &entry);
    void RemoveEntry(QDate &date, int hour, int lane);
    Entry* GetEntry(QDate &date, int hour, int lane);
    int HoursUnpaid(QDate &date);
    int HoursReserved(QDate &date);
};

class Person
{
public:
    QString firstname;
    QString lastname;
    QString phone;
};

class BlackList
{
public:
    QVector<Person> people;

    BlackList();
    void Save();
    void Load();
    void AddPerson(Person &person);
    void RemovePerson(QString &firstname, QString &lastname, QString &phone);
    bool IsBlacklisted(QString &firstname, QString &lastname);
    bool IsBlacklisted(QString &phone);
};

extern DataManager dm;
extern BlackList bl;

#endif // DATAMANAGER_H
