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
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Type
  {
    Invalid,
    ServerList,
    SortedServerList,
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  Type m_type;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapAbstractServerList() : m_type (Type::Invalid) {}
  DapAbstractServerList (Type a_type) : m_type (a_type) {}
  virtual ~DapAbstractServerList() {}
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Type type() { return m_type; }

  virtual int append (const DapServerInfo &a_server) = 0;
  virtual int append (DapServerInfo &&a_server) = 0;
  virtual void insert (int a_index, const DapServerInfo &a_server) = 0;
  virtual void insert (int a_index, DapServerInfo &&a_server) = 0;
  virtual void remove (int a_index) = 0;
  virtual int size() const = 0;
  virtual bool empty() const = 0;
  inline bool isEmpty() { return empty(); }
  virtual int indexOf (const DapServerInfo &a_item) const = 0;
  virtual int indexOfName (const QString &a_name) const = 0;
  virtual int indexOfAddress (const QString &a_address) const = 0;

  virtual const DapServerInfo &first() const = 0;
  virtual const DapServerInfo &last() const = 0;
  virtual DapServerInfo &at (int a_index) = 0;
  virtual const DapServerInfo &at (int a_index) const = 0;
  virtual DapServerInfo value (int a_index) const = 0;
  /// return server item copy as QVariantMap
  Q_INVOKABLE virtual QVariant qValue (int a_index) const = 0;

  virtual int current() const = 0;
  virtual void setCurrent (int a_index) = 0;
  virtual const DapServerInfo &currentServer() const = 0;

  virtual void clear() = 0;

  template<class T>
  T *as() { return dynamic_cast<T*> (this); }

  static const QHash<int, QByteArray> &serverRoleNames();
  static const QHash<QString, QString> &countryMap();
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  inline DapServerInfo &operator[] (int a_index)                { return at (a_index); }
  inline const DapServerInfo &operator[] (int a_index) const    { return at (a_index); }
  inline DapAbstractServerList &operator<< (const DapServerInfo &a_server)  { append (a_server); return *this; }
  inline DapAbstractServerList &operator<< (DapServerInfo &&a_server)       { append (std::move (a_server)); return *this; }
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

  int append (const DapServerInfo &a_server) override;
  int append (DapServerInfo &&a_server) override;
  void insert (int a_index, const DapServerInfo &a_server) override;
  void insert (int a_index, DapServerInfo &&a_server) override;
  void remove (int a_index) override;
  int size() const override;
  bool empty() const override;
  int indexOf (const DapServerInfo &a_item) const override;
  int indexOfName (const QString &a_name) const override;
  int indexOfAddress (const QString &a_address) const override;
  void erase (Iterator it);
  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;
  const DapServerInfo &first() const override;
  const DapServerInfo &last() const override;
  DapServerInfo &at (int a_index) override;
  const DapServerInfo &at (int a_index) const override;
  DapServerInfo value (int a_index) const override;
  Q_INVOKABLE QVariant qValue (int a_index) const override;

  int current() const override;
  void setCurrent (int a_index) override;
  const DapServerInfo &currentServer() const override;

  void move (int a_source, int a_dest);
  void clear() override;

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

  int append (const DapServerInfo &a_server) override;
  int append (DapServerInfo &&a_server) override;
  void insert (int a_index, const DapServerInfo &a_server) override;
  void insert (int a_index, DapServerInfo &&a_server) override;
  void remove (int a_index) override;
  int size() const override;
  bool empty() const override;
  int indexOf (const DapServerInfo &a_item) const override;
  int indexOfName (const QString &a_name) const override;
  int indexOfAddress (const QString &a_address) const override;
  void erase (Iterator it);
  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;
  const DapServerInfo &first() const override;
  const DapServerInfo &last() const override;
  DapServerInfo &at (int a_index) override;
  const DapServerInfo &at (int a_index) const override;
  DapServerInfo value (int a_index) const override;
  Q_INVOKABLE QVariant qValue (int a_index) const override;

  int current() const override;
  void setCurrent (int a_index) override;
  const DapServerInfo &currentServer() const override;

  void clear() override;
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
