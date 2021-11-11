/* INCLUDES */
#include "choosethememodel.h"
#include "ui/choosetheme.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QBoxLayout>
#include <QTimer>
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseThemeModel::ChooseThemeModel (QWidget *parent)
  : ModelBase (parent)
  , _hook (false)
{

}

ChooseThemeModel::~ChooseThemeModel()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void ChooseThemeModel::setLanguageArray (const QJsonArray &a_array, ChooseTheme *a_cs)
{
  /* parse and store languages */
  for (auto i = a_array.constBegin(), e = a_array.constEnd(); i != e; i++)
    {
      auto j  = i->toObject();
      auto k  = j.value("name").toString();
      m_array.append (k);
    }

  /* store and invoke setup */
  m_cs    = a_cs;
  QMetaObject::invokeMethod(this, &ChooseThemeModel::slotSetup, Qt::QueuedConnection);
}

const QStringList &ChooseThemeModel::array() const
{
  return m_array;
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void ChooseThemeModel::slotSetup()
{
  setupLayout();

  /* delete old buttons */
  foreach (auto *oldItem, m_list)
    {
      lay->removeWidget (oldItem);
      delete oldItem;
    }
  m_list.clear();

  /* create new buttons */
  int size  = m_array.size();
  for (int i = 0; i < size; i++)
    {
      /* get data */
      QString text = m_array.value(i);

      /* create item */
      auto item = new DapGuiRadio;
      m_list << item;

      /* setup */
      item->setText (text);
      item->setSeparator (i + 1 < size);
      item->setCssStyle ("choser-item");
      lay->addWidget (item);

      /* signals */
      connect (item, &DapGuiRadio::toggled,
               this, &ChooseThemeModel::slotToggled);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);

  if (!m_list.isEmpty())
    emit filled();
}

void ChooseThemeModel::slotToggled(bool checked)
{
//  if(_hook)
//    {
//      qDebug() << __PRETTY_FUNCTION__ << "skipped";
//      return;
//    }

  /* get sender radio */
  auto s = qobject_cast<DapGuiRadio*> (sender());
  if (!s || checked == false)
    return;

  /* get it's index */
  m_currentIndex = m_list.indexOf (s);
  if (m_currentIndex == -1)
    return;

  /* get current server */
  m_currentText = s->text();

  /* show selection and select */
  QTimer::singleShot(300, [&]
  {
    /* send selection event signal */
    emit m_cs->sigSelect (m_currentIndex, m_currentText);
  });
}

/*-----------------------------------------*/
