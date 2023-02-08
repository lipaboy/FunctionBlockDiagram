#include "FunctionGraph.h"

#include <QSaveFile>

FunctionGraph::FunctionGraph( int externalOutPinsCount,
                              int externalInPinsCount,
                              QObject * parent )
    : QObject( parent )
{
    if ( externalOutPinsCount > 0 )
    {
        SFunctionNode node{};
        node.outPins.resize( externalOutPinsCount );
        m_externalOutPinsInd = m_functionNodes.add( node );
    }
    if ( externalInPinsCount > 0 )
    {
        SFunctionNode node{};
        node.inPins.resize( externalInPinsCount );
        m_externalInPinsInd = m_functionNodes.add( node );
    }
}

void FunctionGraph::loadFunctions( QVector< SFunctionInfo > funcInfos )
{
    for ( auto & info : funcInfos )
    {
        SFunctionNode node{};
        node.name = info.funcName;
        node.inPins.resize( info.inputPinCount );
        node.outPins.resize( info.outputPinCount );
        m_functionNodes.add( node );
    }

    emit updated();
}

void FunctionGraph::connectPins( const SFunctionPinIndex & inVertex,
                                     const SFunctionPinIndex & outVertex )
{
    SFunctionPinIndexOpt & rInPin = unzipInIndex( inVertex );
    SFunctionPinIndexOpt & rOutPin = unzipOutIndex( outVertex );
    if ( inVertex.funcId != outVertex.funcId
         && ! rInPin.has_value()
         && ! rOutPin.has_value() )
    {
        rInPin = outVertex;
        rOutPin = inVertex;

        emit connectionChanged( inVertex, outVertex, true );
    }
}

void FunctionGraph::disconnectPins( const SFunctionPinIndex & inVertex,
                                        const SFunctionPinIndex & outVertex )
{
    SFunctionPinIndexOpt & rInPin = unzipInIndex( inVertex );
    SFunctionPinIndexOpt & rOutPin = unzipOutIndex( outVertex );
    if ( rInPin == outVertex && rOutPin == inVertex )
    {
        rInPin.reset();
        rOutPin.reset();

        emit this->connectionChanged( inVertex, outVertex, false );
    }
}

void FunctionGraph::saveToFile(const QString &filename)
{
    QSaveFile file( filename );
    file.open( QIODevice::WriteOnly );

    QByteArray data{};

    auto appendUint8 = [] ( QByteArray & data, quint8 r ) -> QByteArray &
    {
        return data.append( r );
    };
    auto appendUint16 =
            [ appendUint8 ] ( QByteArray & data, quint16 r ) -> QByteArray &
    {
        return appendUint8( appendUint8( data, r >> 8 ), r );
    };

    for ( auto & node : m_functionNodes )
    {
        /** Индекс блока */
        appendUint16( data, static_cast< quint16 >( node.id ) );
        /** Адрес функции в микроконтроллере */
        appendUint16( data, static_cast< quint16 >( 0 ) );
        for ( int inPinInd = 0; inPinInd < node.inPins.size(); ++inPinInd )
        {
            auto & inPinOpt = node.inPins[ inPinInd ];
            if ( inPinOpt.has_value() )
            {
                auto inPin = inPinOpt.value();
                /** Индекс входного пина */
                appendUint16( data, static_cast< quint16 >( inPinInd ) );
                quint16 zip = inPin.funcId;
                quint16 mask = ~( ( quint16( 1 ) << 4 ) - 1 );
                zip <<= 4;
                zip = ( mask & zip ) | static_cast< quint16 >( inPin.pin );
                appendUint16( data, zip );
            }
        }
        data.append( '\n' ).append( '\r' );
    }

    file.write( data );

    file.commit();
}

SFunctionPinIndexOpt & FunctionGraph::rget( bool isIn,
                                            const SFunctionPinIndex & index)
{
    return isIn ? m_functionNodes[ index.funcId ].inPins[ index.pin ]
            : m_functionNodes[ index.funcId ].outPins[ index.pin ];
}

SFunctionPinIndexOpt & FunctionGraph::unzipInIndex( const SFunctionPinIndex & index )
{
    return rget( true, index );
}

SFunctionPinIndexOpt &FunctionGraph::unzipOutIndex(const SFunctionPinIndex &index)
{
    return rget( false, index );
}

void FunctionGraph::addOperation( LogicOperations operation )
{
    m_functionNodes.add( SFunctionNode{ "AND", 2, 1 } );
    emit updated();
}

void FunctionGraph::removeFunction( int functionId )
{
    if ( functionId != m_externalInPinsInd
         && functionId != m_externalOutPinsInd )
    {
        m_functionNodes.remove( functionId );
        emit updated();
    }
}
