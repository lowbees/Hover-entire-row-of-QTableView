#include "tableview.h"
#include <QMouseEvent>

TableView::TableView(QWidget *parent)
    : QTableView (parent)
    , mHoverRow(-1)
    , mHoverColumn(-1)
{
    setMouseTracking(true);
    setSelectionBehavior(SelectRows);
}


void TableView::mouseMoveEvent(QMouseEvent *event)
{
    QTableView::mouseMoveEvent(event);

    QModelIndex index = indexAt(event->pos());
    int oldHoverRow = mHoverRow;
    int oldHoverColumn = mHoverColumn;
    mHoverRow = index.row();
    mHoverColumn = index.column();

    if (selectionBehavior() == SelectRows && oldHoverRow != mHoverRow) {
        for (int i = 0; i < model()->columnCount(); ++i)
            update(model()->index(mHoverRow, i));
    }
    if (selectionBehavior() == SelectColumns && oldHoverColumn != mHoverColumn) {
        for (int i = 0; i < model()->rowCount(); ++i) {
            update(model()->index(i, mHoverColumn));
            update(model()->index(i, oldHoverColumn));       // 由于列不会主动刷新，所以这里的update函数是必须的
        }
    }
}
