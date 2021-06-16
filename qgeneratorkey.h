#ifndef QGENERATORKEY_H
#define QGENERATORKEY_H

#include <QObject>
#include <QCoreApplication>
#include <QVector>
#include <QString>
#include <QChar>
#include <QtDebug>
#include <QFile>
#include <QDataStream>

#include <iostream>
#include <bitset>
#include <cstring>

#include "openssl/bn.h"
#include "Random.hpp"

class QGeneratorKey : public QObject
{
    Q_OBJECT
public:
    explicit QGeneratorKey(int bytesSize, int blockSize);
    ~QGeneratorKey();

private:
    BIGNUM *t, *u, *m;
    int bytesSize, blockSize;
    QVector <BIGNUM *> openVec, secVec;

    bool generateM     (QVector <BIGNUM *> vec_secKey);
    bool generateT     ();
    int  bn_count_bits (BIGNUM *bn);

public slots:
    void generateSecKey();
    void generateOpenKey();
    BIGNUM* getM ();
    BIGNUM* getT ();
    QVector <BIGNUM *> getOpenKey ();
    QVector <BIGNUM *> getSecKey ();

signals:
    void doneGenOpen();
    void doneGenSec();
    void sendSecCounter(int counter);
    void sendOpenCounter(int counter);
    void sendCompletedFindSec(bool isFind);

};

#endif // QGENERATORKEY_H
