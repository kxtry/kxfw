#pragma once;

#include "kwidget.h"

class KLineEditPrivate;


class KXFW_API KLineEdit : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KLineEdit)

	Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly )
	Q_PROPERTY(bool focus READ focus WRITE setFocus )
	Q_PROPERTY(QString styleSheet READ styleSheet WRITE setStyleSheet )
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
	Q_PROPERTY(QString adjustText READ text WRITE setAdjustText)
	Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
	Q_PROPERTY(QLineEdit::EchoMode mode READ echoMode WRITE setEchoMode)
	Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment)
	Q_PROPERTY(QMargins textMargins READ textMargins WRITE setTextMargins)
	Q_PROPERTY(bool hasFrame READ hasFrame WRITE setFrame)
	Q_PROPERTY(QString frameImage READ frameImage WRITE setFrameImage)
	Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask)
	Q_PROPERTY(QSize intValidator READ intValidator WRITE setIntValidator)
	Q_PROPERTY(QSizeF doubleValidator READ doubleValidator WRITE setDoubleValidator)
	Q_PROPERTY(QString regExpValidator READ regExpValidator WRITE setRegExpValidator)
	Q_PROPERTY(QString validatorTip READ validatorTip WRITE setValidatorTip)
	Q_PROPERTY(bool hasContextMenu READ hasContextMenu WRITE setHasContextMenu)
	Q_PROPERTY(QObject* edit READ realLineEdit)
	Q_PROPERTY(QObject* frame READ frame)
public:
	explicit KLineEdit(QGraphicsItem *parent = NULL);
	~KLineEdit();
	
	QObject* realLineEdit();
	QObject* frame();

	bool readOnly() const;
	void setReadOnly(bool on);

	QSize intValidator() const;
	void setIntValidator(const QSize& r);

	QSizeF doubleValidator() const;
	void setDoubleValidator(const QSizeF& r);

	QString regExpValidator() const;
	void setRegExpValidator(const QString& s);

	QString validatorTip() const;
	void setValidatorTip(const QString& tip);

	QString inputMask() const;
	void setInputMask(const QString& m);

	void setLabel();
	void setEdit();

	QColor textColor();
	void setTextColor(const QColor &clr);

	int fontSize() const;
	void setFontSize(int fs);

	bool focus();
	void setFocus(bool on=true);

	bool hasFrame();
	void setFrame(bool on);

	QString frameImage() const; 
	void setFrameImage(const QString& path);

	QString styleSheet() const;
	void setStyleSheet(const QString& s);

	QString text() const;
	void setText(const QString& s);

	int maxLength() const;
	void setMaxLength(int l);

	QLineEdit::EchoMode echoMode() const;
	void setEchoMode(QLineEdit::EchoMode em);

	Qt::Alignment textAlignment() const;
	void setTextAlignment(Qt::Alignment a);

	QMargins textMargins() const;
	void setTextMargins(const QMargins& m);

	bool hasContextMenu() const;
	void setHasContextMenu(bool on);

	void insert(const QString & t);
	bool modified() const;

	QSize contentSize();
	void setAdjustText(const QString& txt);

	QFont font() const;
	void setFont(const QFont &font);
signals:
	void clicked(QKeyEvent *event);
	void textChanged(const QString &txt);
	void textEdited(const QString &txt);
	void cursorPositionChanged(int old, int now);
	void returnPressed();
	void editingFinished();
	void selectionChanged();

protected:
	virtual bool event(QEvent *event);
	virtual void resizeEvent(QGraphicsSceneResizeEvent * event);
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
	void updateLayout();

private slots:
	void on_edit_focusOut();
};