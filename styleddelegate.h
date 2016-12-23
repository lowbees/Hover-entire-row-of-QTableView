#ifndef STYLEDDELEGATE_H
#define STYLEDDELEGATE_H

#include <QStyledItemDelegate>

class StyledDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum HoverBehavior {
        HoverItems,
        HoverRows,
        HoverColumns
    };

    void setHoverBehavior(HoverBehavior behavior) {
        hoverBehavior = behavior;
    }

public:
    explicit StyledDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
private:
    HoverBehavior hoverBehavior;
};

#endif // STYLEDDELEGATE_H
