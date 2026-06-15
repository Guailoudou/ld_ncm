#include "common.h"

common::common()
{

}

qint64 common::get_time()
{
    qint64 CurrentMEpoch = QDateTime::currentMSecsSinceEpoch();
    return CurrentMEpoch;
}
