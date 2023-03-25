#ifndef DAPSERVERLIST_H
#define DAPSERVERLIST_H

/* INCLUDES */
#include <QAbstractListModel>
#include <QLinkedList>

#include "DapServerInfo.h"

/* DEFS */
class DapServerList;
class DapSortedServerList;

/****************************************//**
 * @brief abstract server list model
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapAbstractServerList
{
  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static const QHash<int, QByteArray> &serverRoleNames();
  static const QHash<QString, QString> &countryMap();
  /// @}
};

/****************************************//**
 * @brief server list model
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapServerList
  : public QAbstractListModel
  , public DapAbstractServerList
{
  Q_OBJECT

  friend class DapSortedServerList;

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int current READ current WRITE setCurrent NOTIFY currentChanged)
  Q_PROPERTY (int size READ size NOTIFY sizeChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  typedef DapServerInfoList::iterator Iterator;
  typedef DapServerInfoList::const_iterator ConstIterator;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  DapServerInfoList m_list;
  int m_current;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapServerList();
  DapServerList (const DapServerInfoList &a_src);
  DapServerList (const DapServerList &a_src);
  DapServerList (DapServerList &&a_src);
  ~DapServerList() override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void currentChanged();
  void sizeChanged();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapServerList *instance();

  int append (const DapServerInfo &a_server);
  int append (DapServerInfo &&a_server);
  void insert (int a_index, const DapServerInfo &a_server);
  void insert (int a_index, DapServerInfo &&a_server);
  void remove (int a_index);
  int size() const;
  bool empty() const;
  inline bool isEmpty() const { return empty(); }
  int indexOf (const DapServerInfo &a_item) const;
  int indexOfName (const QString &a_name) const;
  int indexOfAddress (const QString &a_address) const;
  void erase (Iterator it);
  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;
  const DapServerInfo &first() const;
  const DapServerInfo &last() const;
  DapServerInfo &at (int a_index);
  const DapServerInfo &at (int a_index) const;
  DapServerInfo value (int a_index) const;
  Q_INVOKABLE QVariant qValue (int a_index) const;

  int current() const;
  void setCurrent (int a_index);
  const DapServerInfo &currentServer() const;

  void move (int a_source, int a_dest);
  void clear();

  inline operator DapServerInfoList () const { return m_list; }
protected:
  int _iteratorIndex (Iterator &a_it);
  int _iteratorIndex (ConstIterator &a_it);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator[] (int a_index);
  const DapServerInfo &operator[] (int a_index) const;
  DapServerList& operator = (const DapServerInfoList &a_src);
  DapServerList& operator = (const DapServerList &a_src);
  DapServerList& operator = (DapServerList &&a_src);
  DapServerList &operator<< (const DapServerInfo &a_server);
  DapServerList &operator<< (DapServerInfo &&a_server);
  /// @}
};

/****************************************//**
 * @brief sorted server list iterator
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapSortedServerListIterator
{
  friend class DapSortedServerListConstIterator;
public:
  typedef QLinkedList<int>::iterator Iterator;
protected:
  DapSortedServerList *p;
  Iterator i;
public:
  DapSortedServerListIterator();
  DapSortedServerListIterator (Iterator n, DapSortedServerList *p);
  DapSortedServerListIterator (const DapSortedServerListIterator &o);
  DapServerInfo &operator*();
  DapServerInfo *operator->();
  DapServerInfo &operator[] (int j);
  bool operator== (const DapSortedServerListIterator &o) const;
  bool operator!= (const DapSortedServerListIterator &o) const;
  bool operator< (const DapSortedServerListIterator &other) const;
  bool operator<= (const DapSortedServerListIterator &other) const;
  bool operator> (const DapSortedServerListIterator &other) const;
  bool operator>= (const DapSortedServerListIterator &other) const;
  DapSortedServerListIterator &operator++();
  DapSortedServerListIterator operator++ (int);
  DapSortedServerListIterator &operator--();
  DapSortedServerListIterator operator-- (int);
  DapSortedServerListIterator &operator+= (int j);
  DapSortedServerListIterator &operator-= (int j);
  DapSortedServerListIterator operator+ (int j) const;
  DapSortedServerListIterator operator- (int j) const;
  friend inline DapSortedServerListIterator operator+ (int j, DapSortedServerListIterator k) { return k + j; }
  int operator- (DapSortedServerListIterator j) const;
  operator DapServerInfo *();
  operator Iterator() const;
  int internalIndex() const;
  bool isNull() const;
};

/****************************************//**
 * @brief sorted server list const iterator
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapSortedServerListConstIterator
{
  friend class DapSortedServerListIterator;
public:
  typedef QLinkedList<int>::const_iterator ConstIterator;
protected:
  const DapSortedServerList *p;
  ConstIterator i;
public:
  DapSortedServerListConstIterator();
  DapSortedServerListConstIterator (ConstIterator n, const DapSortedServerList *p);
  DapSortedServerListConstIterator (const DapSortedServerListConstIterator &o);
  explicit DapSortedServerListConstIterator (const DapSortedServerListIterator &o);
  const DapServerInfo &operator*() const;
  const DapServerInfo *operator->() const;
  const DapServerInfo &operator[] (int j) const;
  bool operator== (const DapSortedServerListConstIterator &o) const;
  bool operator!= (const DapSortedServerListConstIterator &o) const;
  bool operator< (const DapSortedServerListConstIterator &other) const;
  bool operator<= (const DapSortedServerListConstIterator &other) const;
  bool operator> (const DapSortedServerListConstIterator &other) const;
  bool operator>= (const DapSortedServerListConstIterator &other) const;
  DapSortedServerListConstIterator &operator++();
  DapSortedServerListConstIterator operator++ (int);
  DapSortedServerListConstIterator &operator--();
  DapSortedServerListConstIterator operator-- (int);
  DapSortedServerListConstIterator &operator+= (int j);
  DapSortedServerListConstIterator &operator-= (int j);
  DapSortedServerListConstIterator operator+ (int j) const;
  DapSortedServerListConstIterator operator- (int j) const;
  friend inline DapSortedServerListConstIterator operator+ (int j, DapSortedServerListConstIterator k) { return k + j; }
  int operator- (DapSortedServerListConstIterator j) const;
  operator const DapServerInfo *() const;
  operator ConstIterator() const;
  int internalIndex() const;
  bool isNull() const;
};

/****************************************//**
 * @brief sorted server list model
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapSortedServerList
  : public QAbstractListModel
  , public DapAbstractServerList
{
  Q_OBJECT

  friend class DapSortedServerListIterator;
  friend class DapSortedServerListConstIterator;

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int current READ current WRITE setCurrent NOTIFY currentChanged)
  Q_PROPERTY (int size READ size NOTIFY sizeChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  typedef DapSortedServerListIterator Iterator;
  typedef DapSortedServerListConstIterator ConstIterator;
  enum OperationType { Inserted, Removed };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  DapServerList _list;
  QLinkedList<int> _sortedIndexes;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapSortedServerList();
  ~DapSortedServerList() override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void currentChanged();
  void sizeChanged();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapSortedServerList *instance();

  int append (const DapServerInfo &a_server);
  int append (DapServerInfo &&a_server);
  void insert (int a_index, const DapServerInfo &a_server);
  void insert (int a_index, DapServerInfo &&a_server);
  void remove (int a_index);
  int size() const;
  bool empty() const;
  inline bool isEmpty() const { return empty(); }
  int indexOf (const DapServerInfo &a_item) const;
  int indexOfName (const QString &a_name) const;
  int indexOfAddress (const QString &a_address) const;
  void erase (Iterator it);
  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;
  const DapServerInfo &first() const;
  const DapServerInfo &last() const;
  DapServerInfo &at (int a_index);
  const DapServerInfo &at (int a_index) const;
  DapServerInfo value (int a_index) const;
  Q_INVOKABLE QVariant qValue (int a_index) const;

  int current() const;
  void setCurrent (int a_index);
  const DapServerInfo &currentServer() const;

  void clear();
  /// sort all items from scratch
  inline void update() { _sort(); }
  /// sort only provided indexes
  void update (const QList<int> &a_indexes);
  /// get sorted items indexes
  const QLinkedList<int> &getSortedIndexes() const;

  operator DapServerList () const;
  operator DapServerInfoList () const;
protected:
  void _sort();
  int _appendServerIndex (const DapServerInfo &a_server, int a_index);
  void _increaseAllIndexes (int a_index);
  void _decreaseAllIndexes (int a_index);
  void _fixCurrent (int a_index, OperationType a_operationType);
  int _iteratorIndex (Iterator &a_it);
  int _iteratorIndex (ConstIterator &a_it);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator[] (int a_index);
  const DapServerInfo &operator[] (int a_index) const;
  DapSortedServerList &operator<< (const DapServerInfo &a_server);
  DapSortedServerList &operator<< (DapServerInfo &&a_server);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPSERVERLIST_H
