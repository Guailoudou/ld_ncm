#include "common.h"

common::common()
{

}

QString common::get_time()
{
    qint64 CurrentMEpoch = QDateTime::currentMSecsSinceEpoch();
    return QString::number(CurrentMEpoch);
}
