#ifndef CFUNCTIONGRAPH_H
#define CFUNCTIONGRAPH_H

#include <QObject>
#include <QString>
#include <QVector>

#include <optional>

struct SFunctionInfo
{
    QString         funcName{};
    int             inputPinCount{};
    int             outputPinCount{};
};

struct SFunctionPinIndex
{
    int func{};
    int pin{};

    bool operator==( SFunctionPinIndex const & other ) const
    {
        return func == other.func && pin == other.pin;
    }
    bool operator!=( SFunctionPinIndex const & other ) const
    {
        return ! ( *this == other );
    }
};

using SFunctionPinIndexOpt = std::optional< SFunctionPinIndex >;

struct SFunctionNode
{
    QString                             name{};
    /** Связь либо есть, либо её нет. Но pin существует в любом случае */
    QVector< SFunctionPinIndexOpt >     inPins{};
    QVector< SFunctionPinIndexOpt >     outPins{};
};

struct SFunctionDiagram
{
public:
    SFunctionDiagram( const QVector< SFunctionNode > & functionNodes,
                      int externalOutPinsIndex,
                      int externalInPinsIndex )
    {}

    QVector< SFunctionNode > getFunctionNodes() const { return m_functionNodes; }
    SFunctionNode getExternalOutPins() const { return m_functionNodes[ m_externalOutPins ]; }
    SFunctionNode getExternalInPins() const { return m_functionNodes[ m_externalInPins ]; }

private:
    QVector< SFunctionNode >            m_functionNodes{};
    int                                 m_externalOutPins{};
    int                                 m_externalInPins{};
};

class FunctionGraph : public QObject
{
    Q_OBJECT
public:
    explicit FunctionGraph( int externalOutPinsCount,
                            int externalInPinsCount,
                            QObject * parent = nullptr );

public:
    void loadVertices( QVector< SFunctionInfo > funcInfos );
    void connectVertices( SFunctionPinIndex const & inVertex,
                          SFunctionPinIndex const & outVertex );
    void disconnectVertices( SFunctionPinIndex const & inVertex,
                             SFunctionPinIndex const & outVertex );

public:
    SFunctionDiagram getDiagram() const;

signals:
    void updated();
    void connectionChanged( SFunctionPinIndex const & inVertex,
                            SFunctionPinIndex const & outVertex,
                            bool hasConnection );

private:
    SFunctionPinIndexOpt & unzipInIndex( SFunctionPinIndex const & index );
    SFunctionPinIndexOpt & unzipOutIndex( SFunctionPinIndex const & index );

private:
    QVector< SFunctionNode >            m_vertices{};
    QVector< SFunctionPinIndexOpt >     m_externalOutPins{};
    QVector< SFunctionPinIndexOpt >     m_externalInPins{};
};

#endif // CFUNCTIONGRAPH_H
