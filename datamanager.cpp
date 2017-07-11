#include "datamanager.h"
#include <QXmlStreamWriter>
#include <QFile>
#include <QDomDocument>

DataManager dm;
BlackList bl;

DataManager::DataManager()
{
    Load();
}

void DataManager::Save()
{
    QFile file("data.xml");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QXmlStreamWriter xsw(&file);
    xsw.setAutoFormatting(true);
    xsw.writeStartDocument();
    xsw.writeStartElement("entries");

    for(QVector<Entry>::iterator i = entries.begin(); i != entries.end(); ++i)
    {
        xsw.writeStartElement("entry");
        xsw.writeAttribute("date",i->date.toString());
        xsw.writeAttribute("hour",QString::number(i->hour));
        xsw.writeAttribute("lane",QString::number(i->lane));
        xsw.writeAttribute("payment",QString::number(i->payment));
        xsw.writeTextElement("firstname",i->firstname);
        xsw.writeTextElement("lastname",i->lastname);
        xsw.writeTextElement("phone",i->phone);
        xsw.writeTextElement("notes",i->notes);
        xsw.writeTextElement("price",QString::number(i->price));
        xsw.writeTextElement("who",i->recipient);
        xsw.writeTextElement("when",i->receptiontime.toString());
        xsw.writeEndElement();
    }

    xsw.writeEndElement();
    xsw.writeEndDocument();

    file.close();
}

void DataManager::Load()
{
    QFile file("data.xml");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QDomDocument doc("data");
    doc.setContent(&file);
    QDomElement root = doc.documentElement();
    QDomNodeList nodelist = root.elementsByTagName("entry");
    this->entries.clear();
    for(int i = 0; i < nodelist.count(); ++i)
    {
        QDomElement elem = nodelist.at(i).toElement();
        Entry entry;
        entry.date = QDate::fromString(elem.attribute("date"));
        entry.hour = elem.attribute("hour").toInt();
        entry.lane = elem.attribute("lane").toInt();
        entry.payment = elem.attribute("payment").toInt();
        entry.firstname = elem.firstChildElement("firstname").text();
        entry.lastname = elem.firstChildElement("lastname").text();
        entry.phone = elem.firstChildElement("phone").text();
        entry.notes = elem.firstChildElement("notes").text();
        entry.price = elem.firstChildElement("price").text().toDouble();
        entry.recipient = elem.firstChildElement("who").text();
        entry.receptiontime = QDateTime::fromString(elem.firstChildElement("when").text());
        entries.append(entry);
    }

    file.close();
}

void DataManager::AddEntry(Entry &entry)
{
    this->RemoveEntry(entry.date, entry.hour, entry.lane);
    entries.append(entry);
}

void DataManager::RemoveEntry(QDate &date, int hour, int lane)
{
    for(QVector<Entry>::iterator i = entries.begin(); i != entries.end(); ++i)
    {
        if( (i->date == date) && (i->lane == lane) && (i->hour == hour) )
        {
            i = entries.erase(i);
            return;
        }
    }
}

Entry* DataManager::GetEntry(QDate &date, int hour, int lane)
{
    for(QVector<Entry>::iterator i = entries.begin(); i != entries.end(); ++i)
    {
        if( (i->date == date) && (i->lane == lane) && (i->hour == hour) )
        {
            return &(*i);
        }
    }
    return NULL;
}

int DataManager::HoursUnpaid(QDate &date)
{
    int count = 0;
    Entry* entry;
    for(int i = 0; i < 17; ++i)
        for(int j = 0; j < 5; ++j)
        {
            entry = this->GetEntry(date,i,j);
            if(entry)
                if(!entry->payment)
                    ++count;
        }
    return count;
}

int DataManager::HoursReserved(QDate &date)
{
    int count = 0;
    Entry* entry;
    for(int i = 0; i < 17; ++i)
        for(int j = 0; j < 5; ++j)
        {
            entry = this->GetEntry(date,i,j);
            if(entry)
                ++count;
        }
    return count;
}

BlackList::BlackList()
{
    Load();
}

void BlackList::Save()
{
    QFile file("blacklist.xml");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QXmlStreamWriter xsw(&file);
    xsw.setAutoFormatting(true);
    xsw.writeStartDocument();
    xsw.writeStartElement("blacklist");

    for(QVector<Person>::iterator i = people.begin(); i != people.end(); ++i)
    {
        xsw.writeStartElement("person");
        xsw.writeTextElement("firstname",i->firstname);
        xsw.writeTextElement("lastname",i->lastname);
        xsw.writeTextElement("phone",i->phone);
        xsw.writeEndElement();
    }

    xsw.writeEndElement();
    xsw.writeEndDocument();

    file.close();
}

void BlackList::Load()
{
    QFile file("blacklist.xml");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QDomDocument doc("blacklist");
    doc.setContent(&file);
    QDomElement root = doc.documentElement();
    QDomNodeList nodelist = root.elementsByTagName("person");
    this->people.clear();
    for(int i = 0; i < nodelist.count(); ++i)
    {
        QDomElement elem = nodelist.at(i).toElement();
        Person person;
        person.firstname = elem.firstChildElement("firstname").text();
        person.lastname = elem.firstChildElement("lastname").text();
        person.phone = elem.firstChildElement("phone").text();
        people.append(person);
    }

    file.close();
}

void BlackList::AddPerson(Person &person)
{
    this->RemovePerson(person.firstname, person.lastname, person.phone);
    people.append(person);
}

void BlackList::RemovePerson(QString &firstname, QString &lastname, QString &phone)
{
    for(QVector<Person>::iterator i = people.begin(); i != people.end(); ++i)
    {
        if( (i->firstname == firstname) && (i->lastname == lastname) && (i->phone == phone) )
        {
            i = people.erase(i);
            return;
        }
    }
}

bool BlackList::IsBlacklisted(QString &firstname, QString &lastname)
{
    for(QVector<Person>::iterator i = people.begin(); i != people.end(); ++i)
    {
        if( (i->firstname == firstname) && (i->lastname == lastname) )
        {
            return true;
        }
    }
    return false;
}

bool BlackList::IsBlacklisted(QString &phone)
{
    for(QVector<Person>::iterator i = people.begin(); i != people.end(); ++i)
    {
        if( i->phone == phone )
        {
            return true;
        }
    }
    return false;
}
