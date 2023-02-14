#include "Operations.h"

#include <QVector>

QString getLogicOperationName(LogicOperations operation)
{
    static QVector< QString > list =
    {
        "AND",
        "OR",
        "XOR",
        "NOT",

        "AND bit",
        "OR bit",
        "XOR bit",
        "NOT bit",
    };

    if ( operation != last )
    {
        return list[ static_cast< int >( operation ) ];
    }
    return "";
}
