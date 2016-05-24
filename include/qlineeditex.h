#pragma once

#include <QLineEdit>
class KXFW_API QLineEditEx : public QLineEdit
{
	Q_OBJECT;
	Q_PROPERTY(bool chineseContextMenu READ chineseContextMenu WRITE setChineseContextMenu)

public:
	explicit QLineEditEx(QWidget *parent = NULL);
	~QLineEditEx(void);

	QSize contentSize();
	void showContextMenu();

	bool chineseContextMenu() const;
	void setChineseContextMenu(const bool on);
signals:
	void focusOut();
	void buttonClicked(Qt::MouseButton button);
	void focusIn();
	void capsLockChanged();
	void updownKeyClicked(bool bUp);
	void clicked(QKeyEvent *event);

protected:
	virtual void contextMenuEvent(QContextMenuEvent *event );
	virtual void keyPressEvent( QKeyEvent *event );
	virtual void mousePressEvent( QMouseEvent *event );
	virtual void focusOutEvent( QFocusEvent *event );
	virtual void focusInEvent( QFocusEvent *event );

private:
	void showEnContextMenu();
	void showCnContextMenu();
};
