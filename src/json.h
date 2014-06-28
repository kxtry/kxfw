/****************************************************************************
**
** Copyright (c) 2010 Girish Ramakrishnan <girish@forwardbias.in>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#ifndef JSON_H
#define JSON_H

#include <QByteArray>
#include <QVariant>

/*
\code

JsonReader reader;
if (reader.parse(QString("{ \"id\": 123, \"class\": \"JsonReader\", \"authors\": [\"Denis\",\"Ettrich\",\"Girish\"] }"))) {
QVariant result = reader.result();
QVariantMap map = result.toMap(); // the JSON object
qDebug() << map.count(); // 3
qDebug() << map["id"].toInt(); // 123
qDebug() << map["class"].toString(); // "JsonReader"
QVariantList list = map["authors"].toList();
qDebug() << list[2].toString(); // "Girish"
} else {
qDebug() << reader.errorString();
}
\endcode

For example,
\code
QVariantMap map;
map["id"] = 123;
map["class"] = "JsonWriter";
QVariantList list;
list << "Denis" << "Ettrich" << "Girish";
map["authors"] = list;

JsonWriter writer;
if (writer.stringify(map)) {
QString json = writer.result();
qDebug() << json; // {"authors": ["Denis", "Ettrich", "Girish"], "class": "JsonWriter", "id": 123 }
} else {
qDebug() << "Failed to stringify " << writer.errorString();
}
\endcode

*/


class JsonReader
{
public:
    JsonReader();
    ~JsonReader();

    bool parse(const QByteArray &ba);
    bool parse(const QString &str);

    QVariant result() const;

    QString errorString() const;

private:
    QVariant m_result;
    QString m_errorString;
    void *reserved;
};

class JsonWriter
{
public:
    JsonWriter();
    ~JsonWriter();

    bool stringify(const QVariant &variant);

    QString result() const;

    QString errorString() const;

    void setAutoFormatting(bool autoFormat);
    bool autoFormatting() const;

    void setAutoFormattingIndent(int spaceOrTabs);
    int autoFormattingIndent() const;

private:
    void stringify(const QVariant &variant, int depth);

    QString m_result;
    QString m_errorString;
    bool m_autoFormatting;
    QString m_autoFormattingIndent;
    void *reserved;
};

#endif // JSON_H

