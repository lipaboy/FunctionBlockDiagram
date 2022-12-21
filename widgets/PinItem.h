#ifndef CPINITEM_H
#define CPINITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QPainter>

class PinItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    PinItem( const QColor & baseColor, QObject * parent = nullptr );
    ~PinItem() override = default;

public slots:
    void setSelected( bool isSelected );

signals:
    void clicked();

private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
//    void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;

private:
    QColor                  m_baseColor{};
    QColor                  m_selectColor = Qt::blue;
};

#endif // CPINITEM_H
