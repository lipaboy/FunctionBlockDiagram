#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QObject>
#include <QGraphicsLineItem>

class ConnectionItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    ConnectionItem( QObject * parent = nullptr );
};

#endif // CONNECTIONITEM_H
