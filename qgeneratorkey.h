/*
 *  BackpackPacker Copyright (C) 2021  Kambarov I. G.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Subsequent modifications must be distributed under the same license.
 */

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
#include <QDateTime>

#include <iostream>
#include <bitset>
#include <cstring>

#include "openssl/bn.h"
#include "Random.hpp"
#include "qfilehandler.h"

class QGeneratorKey : public QObject
{
    Q_OBJECT
public:
    explicit QGeneratorKey(int bytesSize, int blockSize, QString version, QString code, QString dir);
    ~QGeneratorKey();

    void    genIdTask();
    QString getIdTask();
    QString getDir();
    int     getBlockSize();

private:
    BIGNUM *t, *u, *m;
    int bytesSize, blockSize;
    QVector <BIGNUM *> openVec, secVec;
    QString idTask, code, dir;
    QFileHandler *handler;

    bool generateM     (QVector <BIGNUM *> vec_secKey);
    bool generateT     ();
    int  bn_count_bits (BIGNUM *bn);

public slots:
    void generateSecKey();
    void generateOpenKey();
    void generateKeys();
    BIGNUM* getM ();
    BIGNUM* getT ();
    QVector <BIGNUM *> getOpenKey ();
    QVector <BIGNUM *> getSecKey ();

signals:
    //void doneGenOpen(QGeneratorKey &retGen);
    //void doneGenSec(QGeneratorKey &retGen);
    void sendSecCounter(QGeneratorKey *retGen, int counter);
    void sendOpenCounter(QGeneratorKey *retGen, int counter);
    void sendCompletedTask(QGeneratorKey *retGen);
    void sendNameTask(QString idTask, QString nameTask);
    //void sendNameTask(QGeneratorKey *retGen);

};

#endif // QGENERATORKEY_H
