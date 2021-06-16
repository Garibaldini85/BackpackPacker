#ifndef QTOTALSQUEEZER_H
#define QTOTALSQUEEZER_H

#include "openssl/bn.h"

#include "qfilehandler.h"

#include <iostream>
#include <bitset>
#include <cstring>

#include <QObject>

class QTotalSqueezer : public QObject
{
    Q_OBJECT
public:
    explicit QTotalSqueezer(QObject *parent = nullptr);
    ~QTotalSqueezer();

private:
    BIGNUM *num, *rem, *d, *h, *temp;
    BN_CTX *context;
    std::string strStd;

public slots:
    QString decodeIntoUtf (QString &str);
    QString encodeToUtf(QString &str);
    QByteArray compress(QString &str);

signals:

};

#endif // QTOTALSQUEEZER_H
