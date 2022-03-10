/* INCLUDES */
#include "dapqmlmodelserialhistory.h"
#include "DapDataLocal.h"

/* VARS */
static DapQmlModelSerialHistory *__inst = nullptr;
static QStringList *s_history           = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelSerialHistory::DapQmlModelSerialHistory(QObject *parent)
  : QAbstractTableModel{parent}
{
  s_history = new QStringList();
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelSerialHistory *DapQmlModelSerialHistory::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelSerialHistory;
  return __inst;
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelSerialHistory::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return s_history->size();
}

int DapQmlModelSerialHistory::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlModelSerialHistory::data(const QModelIndex &index, int role) const
{
  /* check index */
  if (!index.isValid() || role != 0)
    return QVariant();

  /* check boundaries */
  auto list   = DapDataLocal::instance()->bugReportHistory()->getBugReportsList();
  if (index.row() >= list->size())
    return QVariant();

  /* return value */
  auto item   = list->at (index.row());
  return item.number;
}

QHash<int, QByteArray> DapQmlModelSerialHistory::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "key");

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelSerialHistory::slotSetup()
{
  beginResetModel();
  *s_history  = DapDataLocal::instance()->getHistorySerialKeyData();
  endResetModel();
}

/*-----------------------------------------*/
