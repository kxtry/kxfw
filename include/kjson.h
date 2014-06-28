#pragma once;



/*
\code

KJsonReader reader;
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

KJsonWriter writer;
if (writer.stringify(map)) {
QString json = writer.result();
qDebug() << json; // {"authors": ["Denis", "Ettrich", "Girish"], "class": "JsonWriter", "id": 123 }
} else {
qDebug() << "Failed to stringify " << writer.errorString();
}
\endcode

*/

class KJsonReaderPrivate;

class KXFW_API KJsonReader
{
public:
	explicit KJsonReader();
	~KJsonReader();

	bool read(const QByteArray &ba);
	bool read(const QString &str);

	QVariant result() const;

	QString error() const;

private:
	KJsonReaderPrivate *d;
};


class KJsonWriterPrivate;

class KXFW_API KJsonWriter
{
public:
	explicit KJsonWriter();
	~KJsonWriter();

	bool write(const QVariant &variant);

	QString result() const;

	QString error() const;

	void setAutoFormatting(bool autoFormat);
	bool autoFormatting() const;

	void setAutoFormattingIndent(int spaceOrTabs);
	int autoFormattingIndent() const;

private:
	KJsonWriterPrivate *d;
};