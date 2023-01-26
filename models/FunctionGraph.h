#ifndef CFUNCTIONGRAPH_H
#define CFUNCTIONGRAPH_H

#include <QObject>
#include <QString>
#include <QVector>

struct SFunctionPinIndex
{
    enum EPinType
    {
        OUT = 0,
        IN,
    };

    EPinType pinType{};
    int funcId{};
    int pin{};

    SFunctionPinIndex()
    {}
    SFunctionPinIndex( EPinType type, int functionId, int pinIndex )
        : pinType( type ), funcId( functionId ), pin( pinIndex )
    {}

    bool operator==( SFunctionPinIndex const & other ) const
    {
        return pinType == other.pinType
                && funcId == other.funcId
                && pin == other.pin;
    }
    bool operator!=( SFunctionPinIndex const & other ) const
    {
        return ! ( *this == other );
    }
};

using SFunctionPinIndexOpt = std::optional< SFunctionPinIndex >;

struct SFunctionNode
{
    int                                 id{};
    QString                             name{};
    /** Связь либо есть, либо её нет. Но pin существует в любом случае */
    QVector< SFunctionPinIndexOpt >     inPins{};
    QVector< SFunctionPinIndexOpt >     outPins{};
};

struct SFunctionInfo
{
    QString         funcName{};
    int             inputPinCount{};
    int             outputPinCount{};
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
    QVector< SFunctionNode > getFunctionNodes() const { return m_functionNodes; }
    int getExternalOutPinsId() const { return m_externalOutPinsInd; }
    int getExternalInPinsId() const { return m_externalInPinsInd; }

public:
    void saveToFile( const QString & filename );

signals:
    void updated();
    void connectionChanged( SFunctionPinIndex const & inVertex,
                            SFunctionPinIndex const & outVertex,
                            bool hasConnection );

private:
    SFunctionPinIndexOpt & unzipInIndex( SFunctionPinIndex const & index );
    SFunctionPinIndexOpt & unzipOutIndex( SFunctionPinIndex const & index );
    SFunctionPinIndexOpt & rget( bool isIn, SFunctionPinIndex const & index );

private:
    QVector< SFunctionNode >            m_functionNodes{};
    int                                 m_externalOutPinsInd{};
    int                                 m_externalInPinsInd{};
};

#endif // CFUNCTIONGRAPH_H
