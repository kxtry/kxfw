#pragma once

#include "kwidget.h"

class KHotkeyPrivate;

class KXFW_API KHotkey : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KHotkey)
	Q_DECLARE_PRIVATE(KHotkey)

	Q_PROPERTY( QString backgroundImage READ backgroundImage WRITE setBackgroundImage )
public:
	explicit KHotkey(QGraphicsItem *parent = NULL);
	~KHotkey();

	void setEnableHotKey( bool isEnable );
	void setHotkey(quint32 modifierKey, quint32 virtualKey);
	void getHotkey(quint32 &modifierKey, quint32 &virtualKey);
	void setHotKey(const QString& key );
	QString getHotkey() const;

	void setBackgroundImage( const QString &image );
	QString backgroundImage( ) const;

	void setFocus();

	static QString getVirtualKeyName(quint32 keyCode);
	static QString getModifierKeyName(quint32 keyCode);

signals:
	void hotkeyChanged(quint32 modifierKey, quint32 virtualKey);
	void focusOut();
protected:
	QString _getModifierKeyName( quint32 keyCode );

	bool eventFilter(QObject *object, QEvent *event);
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
private:
	 quint32 modifierToVirtualKey( quint32 modifierKey );
	 quint32 virtualKeyToModifier( quint32 virtualKey );
};