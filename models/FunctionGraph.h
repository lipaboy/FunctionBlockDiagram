#ifndef CFUNCTIONGRAPH_H
#define CFUNCTIONGRAPH_H

#include "GraphStruct.h"
#include "Operations.h"
#include "FunctionContainer.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QList>

class FunctionGraph : public QObject
{
    Q_OBJECT
public:
    explicit FunctionGraph( int externalOutPinsCount,
                            int externalInPinsCount,
                            QObject * parent = nullptr );

public:
    void loadFunctions( QVector< SFunctionInfo > funcInfos );
    void connectPins( SFunctionPinIndex const & inVertex,
                          SFunctionPinIndex const & outVertex );
    void disconnectPins( SFunctionPinIndex const & inVertex,
                             SFunctionPinIndex const & outVertex );
    void addOperation( LogicOperations operation );
    void removeFunction( int functionId );

public:
    QList< SFunctionNode > getFunctionNodes() const { return m_functionNodes.toList(); }
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
    FunctionContainer                   m_functionNodes{};
    int                                 m_externalOutPinsInd{};
    int                                 m_externalInPinsInd{};
};

#endif // CFUNCTIONGRAPH_H
