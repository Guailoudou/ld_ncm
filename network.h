#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>

class network : public QObject
{
    Q_OBJECT
public:
    explicit network(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NETWORK_H