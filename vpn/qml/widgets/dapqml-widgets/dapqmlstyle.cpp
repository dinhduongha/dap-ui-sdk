/* INCLUDES */
#include "dapqmlstyle.h"
#include "style/qssmap.h"
#include "style/qsslink.h"
#include "helper/brand.h"
#include <QQmlProperty>
#include <QRect>
#include <QFontMetrics>
#include <QDebug>

/* VARS */
static DapQmlStyle *s_globalSignal  = nullptr;
static bool s_gsHook                = false;
static QString s_styleSheet;
static thread_local double s_screenWidth = 428, s_screenHeight = 926;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlStyle::DapQmlStyle(QObject *parent)
  : QObject (parent)
  , m_item (nullptr)
{
  /* global instance */
  if (s_globalSignal == nullptr && s_gsHook == false)
    {
      s_gsHook        = true;
      s_globalSignal  = new DapQmlStyle;
      s_gsHook        = false;
    }

  /* signals */
  connect (this, &DapQmlStyle::qssChanged,
           this, &DapQmlStyle::_applyStyle,
           Qt::QueuedConnection);
  connect (this, &DapQmlStyle::itemChanged,
           this, &DapQmlStyle::_applyStyle,
           Qt::QueuedConnection);

  if (s_globalSignal)
    {
      connect (s_globalSignal, &DapQmlStyle::resized,
               this, &DapQmlStyle::_resized);//,
               //Qt::QueuedConnection);
      connect (s_globalSignal, &DapQmlStyle::redrawRequested,
               this, &DapQmlStyle::_applyStyle);//,
               //Qt::QueuedConnection);
    }
}

/********************************************
 * METHODS
 *******************************************/

QString DapQmlStyle::getQss() const
{
  return m_qss;
}

void DapQmlStyle::setQss(const QString &a_newQss)
{
  m_qss = a_newQss;
  emit qssChanged();
}

QObject *DapQmlStyle::getItem() const
{
  return m_item;
}

void DapQmlStyle::setItem (QObject *a_newItem)
{
  m_item = a_newItem;
  emit itemChanged();
}

void DapQmlStyle::windowResized(int a_width, int a_height)
{
  sWindowResized (a_width, a_height);
}

void DapQmlStyle::requestRedraw()
{
  sRequestRedraw();
}

double DapQmlStyle::centerHor (QObject *a_root, QObject *a_item)
{
  /* vars */
  double width[2];
  bool ok[2];

  /* get values */
  width[0] = a_root->property ("width").toDouble (&ok[0]);
  width[1] = a_item->property ("width").toDouble (&ok[1]);

  /* check issues */
  if (!ok[0] || !ok[1])
    return 0;

  /* calc result */
  return (width[0] / 2) - (width[1] / 2);
}

double DapQmlStyle::centerVer (QObject *a_root, QObject *a_item)
{
  /* vars */
  double height[2];
  bool ok[2];

  /* get values */
  height[0] = a_root->property ("height").toDouble (&ok[0]);
  height[1] = a_item->property ("height").toDouble (&ok[1]);

  /* check issues */
  if (!ok[0] || !ok[1])
    return 0;

  /* calc result */
  return (height[0] / 2) - (height[1] / 2);
}

QSize DapQmlStyle::textOnScreenSize(QObject *a_item)
{
  /* check issues */
  if (a_item == nullptr)
    return QSize();

  /* get qml iterm boundingRect, original font size and name and text */
  auto fontSize = a_item->property ("fontSize").toInt();
  auto text     = a_item->property ("text").toString();
  auto fontRect = QRect(
      a_item->property ("x").toInt(),
      a_item->property ("y").toInt(),
      a_item->property ("width").toInt(),
      a_item->property ("height").toInt()
    );
  auto fontName = Brand::fontName();

  /* setup font metrics */
  QFont font (fontName, fontSize);
  QFontMetrics fm (font);

  /* get actual metrics */
  auto rect = fm.boundingRect (fontRect, Qt::AlignVCenter | Qt::AlignHCenter, text);
  return rect.size();
}

void DapQmlStyle::setup(const QString &styleSheet)
{
  s_styleSheet  = styleSheet;
  update();
}

void DapQmlStyle::changeCategory(const QString &a_category)
{
  DapStyle::QssLink::changeCategory (a_category);
}

void DapQmlStyle::update()
{
  DapStyle::QssMap::setup (s_styleSheet);
}

void DapQmlStyle::sWindowResized(int a_width, int a_height)
{
  if (s_globalSignal)
    emit s_globalSignal->resized (a_width, a_height);
}

void DapQmlStyle::sRequestRedraw()
{
  if (s_globalSignal)
    {
      update();
      emit s_globalSignal->redrawRequested();
    }
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlStyle::_applyStyle()
{
  /* send to local listeners */
  emit redrawRequested();

  /* check ptr */
  if (m_item == nullptr)
    return;

  /* get items by style list */
  auto items  = DapStyle::QssMap::items (m_qss);

  /* cycle thru all items */
  for (auto i = items.begin(), e = items.end(); i != e; i++)
    {
      /* ge item */
      auto &item = *i;

      /* cycle thru all item properties */
      for (auto it = item->begin(), en = item->end(); it != en; it++)
        {
          /* set scale */
          if (it.key() == "scaledRect" || it.key() == "scaledFont")
            {
              auto scaled  = it.value().value<DapStyle::Scaled> ();
              if (scaled.adjust (m_item, s_screenWidth, s_screenHeight))
                *it = QVariant::fromValue (scaled);
              continue;
            }

          /* set properties */
          QQmlProperty::write (m_item, it.key(), it.value()); // m_item->setProperty (it.key().toStdString().c_str(), it.value());
        }
    }
}

void DapQmlStyle::_resized(int a_width, int a_height)
{
  s_screenWidth   = a_width;
  s_screenHeight  = a_height;
  _applyStyle();

  if (this != s_globalSignal)
    emit resized (a_width, a_height);
}

/*-----------------------------------------*/
