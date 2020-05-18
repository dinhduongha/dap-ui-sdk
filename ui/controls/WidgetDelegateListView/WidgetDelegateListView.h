#ifndef WIDGETDELEGATELISTVIEW_H
#define WIDGETDELEGATELISTVIEW_H

#include <QItemDelegate>
#include <QListView>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QDebug>
#include <QGestureEvent>
#include "WidgetDelegateBase.h"
#include "CustomComboBoxListView.h"

class WidgetListViewItemDelegate;

class WidgetDelegateListView : public CustomComboBoxListView
{
    Q_OBJECT
public:
    WidgetDelegateListView(QWidget *a_parent = nullptr);

    virtual WidgetDelegateBase* createWidgetDelegate();

    WidgetDelegateBase* indexWidget(const QModelIndex &index);

    virtual void setModel(QAbstractItemModel *a_model) override;
protected slots:
    virtual void rowsInserted(const QModelIndex &parent, int start, int end) override;
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;

private:
    void createIndexDelegates(int a_start = 0, int a_end = -1);

    WidgetListViewItemDelegate *m_itemDelegate{};
};

class WidgetListViewItemDelegate: public QItemDelegate
{
public:
    explicit WidgetListViewItemDelegate(QObject *parent = Q_NULLPTR);;

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;;

};

#endif // WIDGETDELEGATELISTVIEW_H
