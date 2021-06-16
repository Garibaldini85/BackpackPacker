#include "qmainpackingpackageofpackedpackpacker.h"

#include <QTimer>

QMainPackingPackageOfPackedPackPacker::QMainPackingPackageOfPackedPackPacker(QObject *parent) : QObject(parent)
{
    version = "ver_0.1.1";
}

void QMainPackingPackageOfPackedPackPacker::generateKeys(QString code, QString dir, int key, int bytes)
{
    emit sendStatus(true);
    if (code == ""
    ||  dir == "")
    {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
        emit sendStatus(false);
    } else {
        this->code = code;
        this->dir = dir;
        generator = new QGeneratorKey(bytes, key / 8);
        handler = new QFileHandler(version);

        connect(handler, &QFileHandler::doneWriting,
                this, &QMainPackingPackageOfPackedPackPacker::deleteGenAndHan);
        connect(generator, &QGeneratorKey::doneGenSec,
                this, &QMainPackingPackageOfPackedPackPacker::generateOpenKey);
        connect(generator, &QGeneratorKey::doneGenOpen,
                this, &QMainPackingPackageOfPackedPackPacker::writeNewKey);
        connect(generator, &QGeneratorKey::sendSecCounter,
                this, &QMainPackingPackageOfPackedPackPacker::recStatusSecCounter);
        connect(generator, &QGeneratorKey::sendOpenCounter,
                this, &QMainPackingPackageOfPackedPackPacker::recStatusOpenCounter);
        connect(generator, &QGeneratorKey::sendCompletedFindSec,
                this, &QMainPackingPackageOfPackedPackPacker::recStatusCompletedFindSec);

        generateSecKey();
    }
}

void QMainPackingPackageOfPackedPackPacker::sendFalse()
{
    emit sendStatus(false);
}

void QMainPackingPackageOfPackedPackPacker::generateSecKey()
{
    QtConcurrent::run(generator, &QGeneratorKey::generateSecKey);
}

void QMainPackingPackageOfPackedPackPacker::generateOpenKey()
{
    QtConcurrent::run(generator, &QGeneratorKey::generateOpenKey);
}

void QMainPackingPackageOfPackedPackPacker::writeNewKey()
{
    handler->writeNewKey(code, dir, generator->getSecKey(), generator->getOpenKey(), generator->getM(), generator->getT());
}

void QMainPackingPackageOfPackedPackPacker::deleteGenAndHan()
{
    for (auto x: qAsConst(secKey))
        BN_free(x);
    for (auto x: qAsConst(openKey))
        BN_free(x);
    secKey.clear();
    openKey.clear();
    disconnect(generator);
    disconnect(handler);
    generator->deleteLater();
    handler->deleteLater();

    emit sendError("izi", "gotovo");
    emit sendStatus(false);
}

void QMainPackingPackageOfPackedPackPacker::recStatusSecCounter(int counter)
{
    emit sendSecCounter(counter);
}

void QMainPackingPackageOfPackedPackPacker::recStatusOpenCounter(int counter)
{
    emit sendOpenCounter(counter);
}

void QMainPackingPackageOfPackedPackPacker::recStatusCompletedFindSec(bool isFind)
{
    emit sendCompletedFindSec(isFind);
}

void QMainPackingPackageOfPackedPackPacker::encodeFile(QString dirKey, QString dirFile, QString dirEncFile)
{
    emit sendStatus(true);
    if (dirKey     == ""
    ||  dirFile    == ""
    ||  dirEncFile == "") {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
        emit sendStatus(false);
    } else {
        QChecker checker(version, dirKey);
        QString log = checker.checkOpenKey();
        if (log != "") {
            emit sendError("Ошибка :(", log);
            emit sendStatus(false);
        } else {
            encoder = new QEncoder(dirKey, dirFile, dirEncFile);
            connect(encoder, &QEncoder::doneEncoding,
                    this, &QMainPackingPackageOfPackedPackPacker::deleteEncoder);
            connect(encoder, &QEncoder::sizeFile,
                    this, &QMainPackingPackageOfPackedPackPacker::sendSizeFile);
            connect(encoder, &QEncoder::statusEncode,
                    this, &QMainPackingPackageOfPackedPackPacker::sendStatusEnc);

            QtConcurrent::run(encoder, &QEncoder::encode);
        }
    }
}

void QMainPackingPackageOfPackedPackPacker::deleteEncoder()
{
    disconnect(encoder);
    encoder->deleteLater();

    emit sendError("izi", "gotovo");
    emit sendStatus(false);
}

void QMainPackingPackageOfPackedPackPacker::recSizeFile(long long size)
{
    emit sendSizeFile(size);
}

void QMainPackingPackageOfPackedPackPacker::recStatusEnc(long long status)
{
    emit sendStatusEnc(status);
}

void QMainPackingPackageOfPackedPackPacker::decodeFile(QString code, QString dirKey, QString dirFile, QString dirDecFile)
{
    emit sendStatus(true);
    if (dirKey     == ""
    ||  dirFile    == ""
    ||  dirDecFile == "")
    {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
        emit sendStatus(false);
    } else {
        QChecker checker(version, dirKey, code);
        QString log = checker.checkSecrKey();
        if (log != "") {
            emit sendError("Ошибка :(", log);
            emit sendStatus(false);
        } else {
            decoder = new QDecoder(dirKey, dirFile, dirDecFile);

            connect(decoder, &QDecoder::doneDecoding,
                    this, &QMainPackingPackageOfPackedPackPacker::deleteDecoder);
            connect(decoder, &QDecoder::sizeFile,
                    this, &QMainPackingPackageOfPackedPackPacker::sendSizeFile);
            connect(decoder, &QDecoder::statusDecode,
                    this, &QMainPackingPackageOfPackedPackPacker::recStatusDec);

            QtConcurrent::run(decoder, &QDecoder::decode);
        }
    }

}

void QMainPackingPackageOfPackedPackPacker::deleteDecoder()
{
    disconnect(decoder);
    decoder->deleteLater();

    emit sendError("izi", "gotovo");
    emit sendStatus(false);
}

void QMainPackingPackageOfPackedPackPacker::recStatusDec(long long status)
{
    emit sendStatusDec(status);
}

void QMainPackingPackageOfPackedPackPacker::checkValidKey(QString code, QString dirOpenKey, QString dirSecrKey)
{
    emit sendStatus(true);

    if (code == "" || dirOpenKey == "" || dirSecrKey == "") {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
        emit sendStatus(false);
    } else {
        QChecker checker(version, dirOpenKey, dirSecrKey, code);
        QString strLog = checker.checkKeys();
        emit sendError("", strLog);
        emit sendStatus(false);
    }
}
