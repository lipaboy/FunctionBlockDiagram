#ifndef CFUNCTIONBLOCKDIAGRAMWIDGET_H
#define CFUNCTIONBLOCKDIAGRAMWIDGET_H

#include "models/CFunctionGraph.h"

#include "widgets/CBlockItem.h"

#include <QWidget>
#include <QGraphicsView>
#include <QHash>


class CFunctionBlockDiagramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CFunctionBlockDiagramWidget(QWidget *parent = nullptr);

signals:

public slots:
    void graphUpdated();

private:
    QGraphicsView *             m_graphicsView{};
    QGraphicsScene *            m_scene{};
    QPointF                     m_blockAppearPoint{ 100, 100 };
    QPointF                     m_stepAppearPoint{ 0, 100 };

    // i of m_blockMap == i of CFunctionGraph->getVertices()
    QVector< CBlockItem * >     m_blockMap{};

    CFunctionGraph *            m_functionGraph{};
};

#endif // CFUNCTIONBLOCKDIAGRAMWIDGET_H
