#include "styleddelegate.h"
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QTableView>


StyledDelegate::StyledDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    hoverBehavior = HoverRows;
}

void StyledDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    viewOption.state &= ~QStyle::State_HasFocus;
    auto *model = index.model();

    if (option.state & QStyle::State_MouseOver) {
        if (option.state & QStyle::State_Selected)           // 如果是已选中状态，则去除掉hover状态
            viewOption.state &= ~QStyle::State_MouseOver;
        else {
            if (hoverBehavior != HoverItems) {
                QTableView *view = qobject_cast<QTableView *>(option.styleObject);
                int count = (hoverBehavior == HoverRows ?
                            model->columnCount() : model->rowCount());
                for (int i = 0; i < count; ++i) {
                    QModelIndex idx = (hoverBehavior == HoverRows ?
                                           model->index(index.row(), i) : model->index(i, index.column()));

                    //! 注意这里用的是index而不是idx，我们不处理当前的hover项，交给父类处理
                    if (i == (hoverBehavior == HoverRows ? index.column() : index.row()))
                        continue;

                    QStyleOptionViewItem opt(viewOption);
                    initStyleOption(&opt, idx);

                    int x = view->columnViewportPosition(idx.column());
                    int y = view->rowViewportPosition(idx.row());
                    int itemWidth = view->columnWidth(idx.column()) - painter->pen().width();
                    int itemHeight = view->rowHeight(idx.row()) - painter->pen().width();
                    opt.rect = QRect(x, y, itemWidth, itemHeight);


                    //! [1] 设置文字对齐方式
                    opt.text = model->data(idx).toString();
                    QVariant value = model->data(idx, Qt::TextAlignmentRole);
                    if (value.isValid())
                        opt.displayAlignment = value.value<Qt::Alignment>();
                    else
                        opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

                    //! [2] 设置图标
                    value = model->data(idx, Qt::DecorationRole);
                    if (value.isValid() && value.canConvert<QIcon>()) {
                        opt.icon = value.value<QIcon>();
                        opt.decorationSize = opt.icon.pixmap(0).size();
                        opt.decorationAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
                    }
                    else {
                        opt.icon = QIcon();
                        opt.decorationSize = QSize();
                        opt.decorationAlignment = Qt::AlignLeft;
                    }
                    QStyledItemDelegate::paint(painter, opt, idx);

                    //! 修正hover列时不及时绘制的bug
                    view->viewport()->update();
                } //! for
            }
            //! HoverItems 交给父类处理
        }  //! else
    }  //!   if
    QStyledItemDelegate::paint(painter, viewOption, index);
}

