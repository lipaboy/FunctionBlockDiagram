#ifndef CFUNCTIONGRAPH_H
#define CFUNCTIONGRAPH_H

#include <QObject>
#include <QString>
#include <QVector>

struct SFunctionInfo
{
    QString         funcName{};
    int             inputPinCount{};
    int             outputPinCount{};
};

struct SFunctionNode
{
    struct SConnection
    {
        SFunctionNode *     funcPtr{ nullptr };
        int                 pinIndex{ 0 };
    };

    QString                     name{};
    QVector< SConnection >      inputPins{};
    QVector< SConnection >      outputPins{};
};

class FunctionGraph : public QObject
{
    Q_OBJECT
public:
    explicit FunctionGraph( QObject * parent = nullptr );

public:
    void loadVertices( QVector< SFunctionInfo > funcInfos );

public:
    QVector< SFunctionNode > getVertices() const { return m_vertices; }

signals:
    void updated();

private:
    QVector< SFunctionNode >        m_vertices{};
};

#endif // CFUNCTIONGRAPH_H
