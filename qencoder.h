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

#ifndef QENCODER_H
#define QENCODER_H

#include "openssl/bn.h"

#include "qfilehandler.h"
#include "qtotalsqueezer.h"
#include "qchecker.h"

#include <iostream>
#include <bitset>
#include <cstring>

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

class QEncoder : public QObject
{
    Q_OBJECT
public:
    QEncoder(QString &dirKey, QString &dirFile, QString &dirEncFile);
    ~QEncoder();

    void genIdTask();
    QString getIdTask();
    QString getDirEncFile();
    QString getPathToFile();

private:
    QFileHandler *handler;
    QString dirKey, dirFile, dirEncFile, idTask, pathToFile;
    long long sizeF;

    QString getBinBlock(QString &array, int &blockSize);
    QString appendAddStr(QString &str, int &blockSize);

public slots:
    void encode();

signals:
    void doneEncoding(QEncoder *enc);
    void sendStatus(QString idTask, double status);
    void sendNameTask(QString idTask, QString nameTask);

    //void sizeFile(long long size);
    //void statusEncode(long long bytes);

};

#endif // QENCODER_H
