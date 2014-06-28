#include "stdafx.h"
#include "klineedit.h"
#include "qlineeditex.h"
#include "kframeitem.h"
#include "kxmlui.h"
#include "kwidget_p.h"
#include "kmessagebox.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KLineEdit)
class KLineEditPrivate: public KWidgetPrivate
{
public:
	KLineEditPrivate()
	{
		editProxy = NULL;
		lineEdit = NULL;
		frame = NULL;
		hasContextMenu = true;
	}
	QGraphicsProxyWidget *editProxy;
	QLineEditEx *lineEdit;
	KFrameItem *frame;
	QString validatorTip;
	QString validatorText;
	bool hasContextMenu;
};

KLineEdit::KLineEdit( QGraphicsItem *parent /*= NULL*/ )
:KWidget(*new KLineEditPrivate(), parent) 
{
	Q_D(KLineEdit);
	setDragPolicy(Nothing);
	d->lineEdit = new QLineEditEx();
	/*以下两种方式实现都是一样的。*/ 
#if 1
	d->lineEdit->setFrame(false);
	d->lineEdit->setAttribute(Qt::WA_TranslucentBackground);
	QPalette pal = d->lineEdit->palette();
	pal.setColor(QPalette::Base, QColor(255, 255, 255, 0));
	d->lineEdit->setPalette(pal);
#else
	d->lineEdit->setStyleSheet("QLineEdit { border-style: none; background: transparent}");
#endif
	d->editProxy = new QGraphicsProxyWidget(this);
	d->editProxy->setWidget(d->lineEdit);
	d->editProxy->setFocusPolicy(Qt::StrongFocus);
	//d->editProxy->setFocusProxy(this);
	d->editProxy->setZValue(1.0);
	QObject::connect(d->lineEdit, SIGNAL(clicked(QKeyEvent*)), this, SIGNAL(clicked(QKeyEvent*)));
	QObject::connect(d->lineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(textChanged(QString)));
	QObject::connect(d->lineEdit, SIGNAL(textEdited(QString)), this, SIGNAL(textEdited(QString)));
	QObject::connect(d->lineEdit, SIGNAL(cursorPositionChanged(int,int)), this, SIGNAL(cursorPositionChanged(int,int)));
	QObject::connect(d->lineEdit, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
	QObject::connect(d->lineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
	QObject::connect(d->lineEdit, SIGNAL(selectionChanged()), this, SIGNAL(selectionChanged()));
	QObject::connect(d->lineEdit, SIGNAL(focusOut()), this, SLOT(on_edit_focusOut()));
}

KLineEdit::~KLineEdit()
{

}

QObject* KLineEdit::realLineEdit()
{
	return d_func()->lineEdit;
}

bool KLineEdit::readOnly() const
{
	return d_func()->lineEdit->isReadOnly();
}

void KLineEdit::setReadOnly( bool on )
{
	Q_D(KLineEdit);
	d->lineEdit->setReadOnly(on);
}

bool KLineEdit::focus()
{
	return d_func()->lineEdit->hasFocus();
}

void KLineEdit::setFocus( bool on )
{
	d_func()->lineEdit->setFocus(Qt::MouseFocusReason);
}

QString KLineEdit::styleSheet() const
{
	return d_func()->lineEdit->styleSheet();
}

void KLineEdit::setStyleSheet( const QString& s )
{
	d_func()->lineEdit->setStyleSheet(s);
}

QString KLineEdit::text() const
{
	return d_func()->lineEdit->text();
}

void KLineEdit::setText( const QString& s )
{
	d_func()->lineEdit->setText(s);
}

int KLineEdit::maxLength() const
{
	return d_func()->lineEdit->maxLength();
}

void KLineEdit::setMaxLength( int l )
{
	d_func()->lineEdit->setMaxLength(l);
}

QLineEdit::EchoMode KLineEdit::echoMode() const
{
	return d_func()->lineEdit->echoMode();
}

void KLineEdit::setEchoMode( QLineEdit::EchoMode em )
{
	d_func()->lineEdit->setEchoMode(em);
}

Qt::Alignment KLineEdit::textAlignment() const
{
	return d_func()->lineEdit->alignment();
}

void KLineEdit::setTextAlignment( Qt::Alignment a )
{
	d_func()->lineEdit->setAlignment(a);
}

QMargins KLineEdit::textMargins() const
{
	return d_func()->lineEdit->textMargins();
}

void KLineEdit::setTextMargins( const QMargins& m )
{
	d_func()->lineEdit->setTextMargins(m);
}

void KLineEdit::insert( const QString & t )
{
	d_func()->lineEdit->insert(t);
}

bool KLineEdit::modified() const
{
	return d_func()->lineEdit->isModified();
}

void KLineEdit::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	__super::resizeEvent(event);
	updateLayout();
}

void KLineEdit::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	Q_D(KLineEdit);
	if(d->hasContextMenu)
	{
		QLineEditEx *lineEdit = d->lineEdit;
		lineEdit->showContextMenu();
	}
	event->ignore();
}

void KLineEdit::updateLayout()
{
	Q_D(KLineEdit);
	QRectF rt = rect();
	if(d->frame)
	{
		d->frame->setGeometry(rt.toRect());
	}
	d->editProxy->setGeometry(rt.toRect());
	d->lineEdit->setGeometry(rt.toRect());
}

bool KLineEdit::event(QEvent *event)
{
	QEvent::Type type = event->type();
	bool bok = __super::event(event);
	switch(type)
	{
	case QEvent::KeyRelease:
		/*修正持续按着鼠标情况，其不刷新问题。*/
		update();
		break;
	}
	return bok;
}

bool KLineEdit::hasFrame()
{
	Q_D(KLineEdit);
	if(d->frame)
		return d->frame->isVisible();
	return false;
}

void KLineEdit::setFrame( bool on )
{
	Q_D(KLineEdit);
	QString file = frameImage();
	if(file.isEmpty())
	{
		file = ":/image/theme/lineedit.png";
	}
	setFrameImage(file);
	d->frame->setVisible(on);
}

void KLineEdit::setLabel()
{
	Q_D(KLineEdit);
	d->lineEdit->setReadOnly(true);
	d->editProxy->setAcceptedMouseButtons(0);
	d->editProxy->setAcceptHoverEvents(false);
	d->editProxy->unsetCursor();
	setDragPolicy(AskParent);
	setBackgroundColor(QColor(0,0,0,0));
	setFrame(false);
}

void KLineEdit::setEdit()
{
	Q_D(KLineEdit);
	d->editProxy->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
	d->editProxy->setAcceptHoverEvents(true);
	d->lineEdit->setReadOnly(false);
	setDragPolicy(Nothing);
	setFrame(true);
	setBackgroundColor(QColor(255,255,255));
	setFocus(true);
}

QSize KLineEdit::contentSize()
{
	Q_D(KLineEdit);
	return d->lineEdit->contentSize();
}

void KLineEdit::setAdjustText( const QString& txt )
{
	Q_D(KLineEdit);
	setText(txt);
	QSize s = contentSize();
	setFixWidth(s.width());
}

void KLineEdit::setFont( const QFont &font )
{
	Q_D(KLineEdit);
	d->lineEdit->setFont(font);
}

QFont KLineEdit::font() const
{
	Q_D(const KLineEdit);
	return d->lineEdit->font();
}

QString KLineEdit::frameImage() const
{
	Q_D(const KLineEdit);
	if(d->frame == NULL)
		return "";
	return d->frame->imagePath();
}

void KLineEdit::setFrameImage( const QString& path )
{
	Q_D(KLineEdit);
	if(path.isEmpty())
		return;
	if(d->frame)
	{
		d->frame->setImagePath(path);
		return ;
	}
	d->frame = new KFrameItem(this);
	d->frame->setFrameType(KFrameItem::GridFour);
	d->frame->setImagePath(path);
	updateLayout();
}

QObject* KLineEdit::frame()
{
	Q_D(KLineEdit);
	return d->frame;
}

QString KLineEdit::inputMask() const
{
	Q_D(const KLineEdit);
	return d->lineEdit->inputMask();
}

void KLineEdit::setInputMask( const QString& m )
{
	Q_D(KLineEdit);
	d->lineEdit->setInputMask(m);
}

QSize KLineEdit::intValidator() const
{
	Q_D(const KLineEdit);
	const QIntValidator *v = qobject_cast<const QIntValidator*>(d->lineEdit->validator());
	if(v)
	{
		int b = v->bottom();
		int t = v->top();
		return QSize(b, t);
	}
	return QSize(-1,-1);
}

void KLineEdit::setIntValidator( const QSize& r )
{
	Q_D(KLineEdit);
	const QValidator *v = d->lineEdit->validator();
	const QIntValidator *vi = qobject_cast<const QIntValidator*>(v);
	if(vi)
	{
		const_cast<QIntValidator*>(vi)->setRange(r.width(), r.height());
		return;
	}
	if(v)
	{
		delete v;
	}
	d->lineEdit->setValidator(new QIntValidator(r.width(), r.height(), d->lineEdit));
}

QSizeF KLineEdit::doubleValidator() const
{
	Q_D(const KLineEdit);
	const QDoubleValidator *v = qobject_cast<const QDoubleValidator*>(d->lineEdit->validator());
	if(v)
	{
		int b = v->bottom();
		int t = v->top();
		return QSizeF(b, t);
	}
	return QSizeF(0,0);
}

void KLineEdit::setDoubleValidator( const QSizeF& r )
{
	Q_D(KLineEdit);
	const QValidator *v = d->lineEdit->validator();
	const QDoubleValidator *vd = qobject_cast<const QDoubleValidator*>(v);
	if(vd)
	{
		const_cast<QDoubleValidator*>(vd)->setRange(r.width(), r.height());
		return;
	}
	if(v)
	{
		delete v;
	}
	d->lineEdit->setValidator(new QDoubleValidator(r.width(), r.height(), 5, d->lineEdit));
}

QString KLineEdit::regExpValidator() const
{
	Q_D(const KLineEdit);
	const QRegExpValidator *v = qobject_cast<const QRegExpValidator*>(d->lineEdit->validator());
	if(v)
	{
		const QRegExp& re = v->regExp();
		return re.pattern();
	}
	return "";
}

void KLineEdit::setRegExpValidator( const QString& s )
{
	Q_D(KLineEdit);
	const QValidator *v = d->lineEdit->validator();
	const QRegExpValidator *vr = qobject_cast<const QRegExpValidator*>(v);
	if(vr)
	{
		const_cast<QRegExpValidator*>(vr)->setRegExp(QRegExp(s));
		return;
	}
	if(v)
	{
		delete v;
	}
	d->lineEdit->setValidator(new QRegExpValidator(QRegExp(s), d->lineEdit));
}

void KLineEdit::on_edit_focusOut()
{
	Q_D(KLineEdit);
	QString text = d->lineEdit->text();
	if(text == d->validatorText)
		return;
	d->validatorText = text;
	const QValidator *validator = d->lineEdit->validator();
	if (validator) 
	{
		int pos;
		if (validator->validate(text, pos) != QValidator::Acceptable)
		{
			KWindow *gv = qobject_cast<KWindow*>(view());
			QString tip = validatorTip();
			if(tip.isEmpty())
			{
				const QIntValidator *intValidator = qobject_cast<const QIntValidator*>(validator);
				const QDoubleValidator *floatValidator = qobject_cast<const QDoubleValidator*>(validator);
				const QRegExpValidator *regExpValidator = qobject_cast<const QRegExpValidator*>(validator);
				if(intValidator)
				{
					double bot = intValidator->bottom();
					double top = intValidator->top();
					KMessageBox::information(gv->rootWidget(), "提示", QString("输入[%1-%2]区间的整数").arg(bot).arg(top), "确认");
				}
				else if(floatValidator)
				{
					double bot = floatValidator->bottom();
					double top = floatValidator->top();
					KMessageBox::information(gv->rootWidget(), "提示", QString("输入[%1-%2]区间的浮点数").arg(bot).arg(top), "确认");
				}
				else if(regExpValidator)
				{
					const QRegExp &regExp = regExpValidator->regExp();
					KMessageBox::information(gv->rootWidget(), "提示", QString("输入的合法的格式：%1").arg(regExp.pattern()), "确认");
				}
				else
				{
					KMessageBox::information(gv->rootWidget(), "提示", "输入的字符不符合要求，请检查。", "确认");
				}
			}
			else
			{
				KMessageBox::information(gv->rootWidget(), "提示", tip, "确认");
			}
			return;
		}
	}
}

QString KLineEdit::validatorTip() const
{
	Q_D(const KLineEdit);
	return d->validatorTip;
}

void KLineEdit::setValidatorTip( const QString& tip )
{
	Q_D(KLineEdit);
	d->validatorTip = tip;
}

bool KLineEdit::hasContextMenu() const
{
	Q_D(const KLineEdit);
	return d->hasContextMenu;
}

void KLineEdit::setHasContextMenu( bool on )
{
	Q_D(KLineEdit);
	d->hasContextMenu = on;
}
