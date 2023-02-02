#ifndef GRAPHSTRUCT_H
#define GRAPHSTRUCT_H

#include <optional>

#include <QString>
#include <QVector>

namespace fbd::model
{

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

    SFunctionNode( int funcId, const QString & funcName,
                   int inPinCount, int outPinCount )
        : id( funcId )
        , name( funcName )
        , inPins( inPinCount )
        , outPins( outPinCount )
    {

    }
    SFunctionNode()
    {

    }
};

struct SFunctionInfo
{
    QString         funcName{};
    int             inputPinCount{};
    int             outputPinCount{};
};

}

#endif // GRAPHSTRUCT_H
