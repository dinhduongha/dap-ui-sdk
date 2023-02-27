#ifndef DAPQMLMODELAUTOSERVERLIST_H
#define DAPQMLMODELAUTOSERVERLIST_H

/* INCLUDES */
#include <QAbstractTableModel>
#include "DapServerList.h"

/****************************************//**
 * @brief servers model list
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelAutoServerList : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /// link to global servers list
  DapServerList &_serverList;
  /// auto servers list
  DapServerList _autoServers;
  /// general location
  QString _location;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlModelAutoServerList();
  DapQmlModelAutoServerList (DapServerList &a_serverList);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// set general location
  /// @note will call @ref update
  Q_INVOKABLE void setLocation (const QString &a_location);
  /// update auto servers list based on information from attached server list
  /// @note if location is not set, use @ref setLocation method which will also invoke this method, so there is no need to call this method after calling @ref setLocation
  Q_INVOKABLE void update();
  const DapServerInfo &at (int a_index) const;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELAUTOSERVERLIST_H
