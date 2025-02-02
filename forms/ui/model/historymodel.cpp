/* INCLUDES */
#include "historymodel.h"
#include "DapDataLocal.h"
#include <dapguibutton.h>
#include <QApplication>
#include <QClipboard>
#include <QScrollBar>
#include <QTimer>

/* DEFS */
struct _HistoryRecord
{
  QString name;
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

HistoryModel::HistoryModel (QWidget *parent)
  : ModelBase (parent)
{

}

HistoryModel::~HistoryModel()
{

}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void HistoryModel::slotSetup()
{
  setupLayout();

  /* delete old buttons */
  foreach (auto *oldItem, m_list)
    {
      lay->removeWidget (oldItem);
      delete oldItem;
    }
  clearLayout(lay);
  m_list.clear();

#ifndef TestApp
  /* create new buttons */
  foreach (auto &item, DapDataLocal::instance()->getHistorySerialKeyData())
    {
      /* create item */
      DapGuiButton *btn = new DapGuiButton;
      m_list << btn;

      btn->setBtnStyle (DapGuiButton::IconMainSub);

      btn->setMainText (item);
      btn->setMainCssClass ("darkblue uppercase font16");

      btn->setSubText (" ");

      btn->setSeparator (true);
      btn->setIconCssClass ("history-icon ic_key-item");

      btn->setSubCssClass ("history-icon-pixmap ic_copy");

      btn->setCursor(Qt::PointingHandCursor);

      btn->setCssStyle ("history-item");

      new _ButtonClickAnimation (btn);

      connect(btn, &DapGuiButton::clicked, [btn](){
          QApplication::clipboard()->setText(btn->mainText());
      });
      lay->addWidget (btn);
    }
#endif // TestApp
  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
  emit filled(m_list.count());
}

/********************************************
 * OVERRIDE
 *******************************************/

bool HistoryModel::eventFilter(QObject *o, QEvent *e)
{
  // This works because QScrollArea::setWidget installs an eventFilter on the widget
  if(o && o == widget() && e->type() == QEvent::Resize)
    setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());

  return QScrollArea::eventFilter(o, e);
}

/*-----------------------------------------*/

_ButtonClickAnimation::_ButtonClickAnimation (DapGuiButton *a_button)
  : QObject (a_button)
  , m_timer (new QTimer (this))
  , m_button (a_button)
{
  m_timer->setInterval (100);
  m_timer->setSingleShot (true);
  connect (m_button, &DapGuiButton::clicked,
           this, &_ButtonClickAnimation::slotStart);
  connect (m_timer.get(), &QTimer::timeout,
           this, &_ButtonClickAnimation::slotFinish);
}

void _ButtonClickAnimation::slotStart()
{
  if (m_button)
    m_button->setSubCssClass ("history-icon-pixmap ic_copy_pressed");
  m_timer->start();
}

void _ButtonClickAnimation::slotFinish()
{
  if (m_button)
    m_button->setSubCssClass ("history-icon-pixmap ic_copy");
}

/*-----------------------------------------*/
