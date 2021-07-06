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

#ifndef QMAINPACKINGPACKAGEOFPACKEDPACKPACKER_H
#define QMAINPACKINGPACKAGEOFPACKEDPACKPACKER_H

#include "qgeneratorkey.h"
#include "qfilehandler.h"
#include "qencoder.h"
#include "qdecoder.h"

#include <QObject>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QThreadPool>
#include <QDesktopServices>

class QMainPackingPackageOfPackedPackPacker : public QObject
{
    Q_OBJECT
public:
    explicit QMainPackingPackageOfPackedPackPacker(QObject *parent = nullptr);

private:
    QThread *trd;
    QGeneratorKey *generator;
    QFileHandler *handler;
    QEncoder *encoder;
    QDecoder *decoder;
    QThreadPool *pool;

    QVector <BIGNUM *> secKey, openKey;
    BIGNUM *m, *t;
    QString code,  dir, version;

public slots:

    /***********************************************************************
    ****************************GENERATOR SLOTS*****************************
    ************************************************************************/
    void generateKeys(QString code, QString dir, int key, int bytes);
    void deleteGenerator(QGeneratorKey *gen);
    void recStatusSecCounter(QGeneratorKey *gen, int counter);
    void recStatusOpenCounter(QGeneratorKey *gen, int counter);

    /***********************************************************************
    ****************************ENCODER SLOTS*******************************
    ************************************************************************/
    void encodeFile(QString dirKey, QString dirFile, QString dirEncFile);
    void deleteEncoder(QEncoder *enc);

    /***********************************************************************
    ****************************DECODER SLOTS*******************************
    ************************************************************************/
    void decodeFile(QString code, QString dirKey, QString dirFile, QString dirDecFile);
    void deleteDecoder(QDecoder *dec);

    /***********************************************************************
    ****************************OTHER SLOTS*********************************
    ************************************************************************/
    void checkValidKey(QString code, QString dirOpenKey, QString dirSecrKey);
    void recNameTask(QString idTask, QString nameTask);
    void recStatus(QString idTask, double status);
    void deleteChecker(QChecker *chk);
    void openFolder(QString path);
    void openGitRep();
signals:
    /***********************************************************************
    ******************************QML SIGNAL********************************
    ************************************************************************/
    void sendStatus (QString idTask, double status);
    void sendError  (QString titleErr, QString strErr);
    void createTask (QString idTask, QString description);
    void setStatusTask (QString idTask, double status);
    void setNameTask (QString idTask, QString nameTask);
    void setIndeterminate (QString idTask, bool boolshit);
    void deleteTask(QString idTask);
    void viewSystemNotif(QString titleNotification, QString textNotification, QString dir);

    /***********************************************************************
    ***************************GENERATOR SIGNAL*****************************
    ************************************************************************/
    void sendSecCounter(int counter);
    void sendOpenCounter(int counter);
    void sendCompletedFindSec(bool isFind);

};

#endif // QMAINPACKINGPACKAGEOFPACKEDPACKPACKER_H
