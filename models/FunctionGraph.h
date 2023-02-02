#ifndef CFUNCTIONGRAPH_H
#define CFUNCTIONGRAPH_H

#include "GraphStruct.h"
#include "Operations.h"

#include <QObject>
#include <QString>
#include <QVector>

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
    void addOperation( LogicOperations operation );

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
