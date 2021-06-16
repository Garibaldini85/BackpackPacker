/***********************************************************************
************************************************************************
************************************************************************/

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
    void sendFalse();
    void generateSecKey();
    void generateOpenKey();
    void writeNewKey();
    void deleteGenAndHan();
    void recStatusSecCounter(int counter);
    void recStatusOpenCounter(int counter);
    void recStatusCompletedFindSec(bool isFind);

    /***********************************************************************
    ****************************ENCODER SLOTS*******************************
    ************************************************************************/
    void encodeFile(QString dirKey, QString dirFile, QString dirEncFile);
    void deleteEncoder();
    void recSizeFile(long long size);
    void recStatusEnc(long long status);


    void decodeFile(QString code, QString dirKey, QString dirFile, QString dirDecFile);
    void deleteDecoder();
    void recStatusDec(long long status);

    void checkValidKey(QString code, QString dirOpenKey, QString dirSecrKey);
signals:
    /***********************************************************************
    ******************************QML SIGNAL********************************
    ************************************************************************/
    void sendStatus (bool status);
    void sendError  (QString titleErr, QString strErr);

    /***********************************************************************
    ***************************GENERATOR SIGNAL*****************************
    ************************************************************************/
    void sendSecCounter(int counter);
    void sendOpenCounter(int counter);
    void sendCompletedFindSec(bool isFind);

    void sendSizeFile(long long size);
    void sendStatusEnc(long long status);

    void sendStatusDec(long long status);

};

#endif // QMAINPACKINGPACKAGEOFPACKEDPACKPACKER_H
