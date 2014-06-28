#include "stdafx.h"
#include "kjson.h"
#include "json.h"

class KJsonReaderPrivate
{
public:
	KJsonReader *q;
	JsonReader json;
public:
	KJsonReaderPrivate()
	{

	}

	~KJsonReaderPrivate()
	{

	}

	bool read(const QByteArray &ba)
	{
		return json.parse(ba);
	}

	bool read(const QString &str)
	{
		return json.parse(str);
	}

	QVariant result() const
	{
		return json.result();
	}

	QString error() const
	{
		return json.errorString();
	}

};

KJsonReader::KJsonReader()
: d(new KJsonReaderPrivate())
{
	d->q = this;
}

KJsonReader::~KJsonReader()
{
	delete d;
}

bool KJsonReader::read( const QByteArray &ba )
{
	return d->read(ba);
}

bool KJsonReader::read( const QString &str )
{
	return d->read(str);
}

QVariant KJsonReader::result() const
{
	return d->result();
}

QString KJsonReader::error() const
{
	return d->error();
}


class KJsonWriterPrivate
{
public:
	KJsonWriter *q;
	JsonWriter json;
public:
	KJsonWriterPrivate()
	{

	}

	~KJsonWriterPrivate()
	{

	}

	bool write(const QVariant &variant)
	{
		return json.stringify(variant);
	}


	QString result() const
	{
		return json.result();
	}


	QString error() const
	{
		return json.errorString();
	}

	void setAutoFormatting(bool autoFormat)
	{
		json.setAutoFormatting(autoFormat);
	}

	bool autoFormatting() const
	{
		return json.autoFormatting();
	}

	void setAutoFormattingIndent(int spaceOrTabs)
	{
		json.setAutoFormattingIndent(spaceOrTabs);
	}

	int autoFormattingIndent() const
	{
		return json.autoFormattingIndent();
	}
};



KJsonWriter::KJsonWriter()
: d(new KJsonWriterPrivate())
{
	d->q = this;
}

KJsonWriter::~KJsonWriter()
{
	delete d;
}

bool KJsonWriter::write( const QVariant &variant )
{
	return d->write(variant);
}

QString KJsonWriter::result() const
{
	return d->result();
}

QString KJsonWriter::error() const
{
	return d->error();
}

void KJsonWriter::setAutoFormatting( bool autoFormat )
{
	d->setAutoFormatting(autoFormat);
}

bool KJsonWriter::autoFormatting() const
{
	return d->autoFormatting();
}

void KJsonWriter::setAutoFormattingIndent( int spaceOrTabs )
{
	d->setAutoFormattingIndent(spaceOrTabs);
}

int KJsonWriter::autoFormattingIndent() const
{
	return d->autoFormattingIndent();
}