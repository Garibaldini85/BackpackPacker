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

#ifndef QFILEHANDLER_H
#define QFILEHANDLER_H

#include "qtotalsqueezer.h"

#include <QtDebug>
#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDataStream>

#include "openssl/bn.h"

class QFileHandler : public QObject
{
    Q_OBJECT
public:
    explicit QFileHandler(QObject *parent = nullptr);
    QFileHandler(QString &ver);
    ~QFileHandler();

private:
    QDateTime    fileName;
    QFile        fileIn, fileOut;
    QTextStream  streamIn, streamOut;
    //uint         counter;
    BIGNUM *m, *t;
    QString ver, str;
    int i, blockSize, counter;
    QChar c;

public slots:
    void             writeNewKey(QString &code, QString &dir, QVector <BIGNUM*> &sec, QVector <BIGNUM*> &open, BIGNUM* m, BIGNUM* t);
    QVector<BIGNUM*> readOpenKey(QString &dirOpenKey);
    QVector<BIGNUM*> readSecrKey (QString &dirSecrKey);
    BIGNUM*          getM();
    BIGNUM*          getT();

    void       openBinFile(QString &dirFile, QString &dirFileOut, int &blockSize);
    QString    readNextBlock();
    void       writeNextBlock(QString &str);
    void       closeFile();

    void    openForDecFile(QString &dirFile, QString &dirFileOut);
    QString readNextEncBlock();

signals:
    void doneWriting();
};

#endif // QFILEHANDLER_H
