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

#include "qmainpackingpackageofpackedpackpacker.h"

#include <QTimer>

QMainPackingPackageOfPackedPackPacker::QMainPackingPackageOfPackedPackPacker(QObject *parent) : QObject(parent)
{ version = "ver_0.2.0"; }

void QMainPackingPackageOfPackedPackPacker::generateKeys(QString code, QString dir, int key, int bytes)
{
    if (code == ""
    ||  dir == "")
    {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
    } else {
        generator = new QGeneratorKey(bytes, key / 16, version, code, dir);
        generator->genIdTask();
        emit createTask(generator->getIdTask(), "Инициализация структуры");
        emit setIndeterminate(generator->getIdTask(), false);
        emit setStatusTask(generator->getIdTask(), 0);

        connect(generator, &QGeneratorKey::sendSecCounter,
                this, &QMainPackingPackageOfPackedPackPacker::recStatusSecCounter);
        connect(generator, &QGeneratorKey::sendOpenCounter,
                this, &QMainPackingPackageOfPackedPackPacker::recStatusOpenCounter);
        connect(generator, &QGeneratorKey::sendCompletedTask,
                this, &QMainPackingPackageOfPackedPackPacker::deleteGenerator);
        connect(generator, &QGeneratorKey::sendNameTask,
                this, &QMainPackingPackageOfPackedPackPacker::recNameTask);

        QtConcurrent::run(generator, &QGeneratorKey::generateKeys);
    }
}

void QMainPackingPackageOfPackedPackPacker::deleteGenerator(QGeneratorKey *gen)
{
    emit deleteTask(gen->getIdTask());
    emit viewSystemNotif(gen->getIdTask(), "Ключи сгенерированы", gen->getDir());
    disconnect(gen);
    gen->deleteLater();
}

void QMainPackingPackageOfPackedPackPacker::recStatusSecCounter(QGeneratorKey *gen, int counter)
{ emit setStatusTask(gen->getIdTask(), (counter) * 1.0 / (gen->getBlockSize() * 32)); }

void QMainPackingPackageOfPackedPackPacker::recStatusOpenCounter(QGeneratorKey *gen, int counter)
{ emit setStatusTask(gen->getIdTask(), (counter + gen->getBlockSize() * 16) * 1.0 / (gen->getBlockSize() * 32)); }

void QMainPackingPackageOfPackedPackPacker::encodeFile(QString dirKey, QString dirFile, QString dirEncFile)
{
    if (dirKey     == ""
    ||  dirFile    == "") {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
    } else {
        QChecker checker(version, dirKey);
        checker.checkOpenKey();
        if (checker.Err != "") {
            emit sendError("Ошибка :(", checker.Err);
        } else {
            encoder = new QEncoder(dirKey, dirFile, dirEncFile);
            encoder->genIdTask();
            emit createTask(encoder->getIdTask(), "Инициализация структуры");
            emit setIndeterminate(encoder->getIdTask(), false);
            emit setStatusTask(encoder->getIdTask(), 0);

            connect(encoder, &QEncoder::doneEncoding,
                    this, &QMainPackingPackageOfPackedPackPacker::deleteEncoder);
            connect(encoder, &QEncoder::sendNameTask,
                    this, &QMainPackingPackageOfPackedPackPacker::recNameTask);
            connect(encoder, &QEncoder::sendStatus,
                    this, &QMainPackingPackageOfPackedPackPacker::recStatus);

            QtConcurrent::run(encoder, &QEncoder::encode);
        }
    }
}

void QMainPackingPackageOfPackedPackPacker::deleteEncoder(QEncoder *enc)
{
    emit deleteTask(enc->getIdTask());
    emit viewSystemNotif(enc->getIdTask(), "Данные зашифрованы", enc->getPathToFile());
    disconnect(enc);
    enc->deleteLater();
}

void QMainPackingPackageOfPackedPackPacker::recStatus(QString idTask, double status)
{ emit setStatusTask(idTask, status); }

void QMainPackingPackageOfPackedPackPacker::deleteChecker(QChecker *chk)
{
    emit deleteTask(chk->getIdTask());
    QString str = chk->Err, dir = "";
    if (str == "")
        str = "Ключи валидны";
    else
        str = "Ошибка ключей:" + str;
    QFileInfo DOK(chk->getDirOpenKey()), DSK(chk->getDirSecrKey());
    if (DOK.absolutePath() == DSK.absolutePath())
        dir = DOK.absolutePath();
    emit viewSystemNotif(chk->getIdTask(), str, dir);
    disconnect(chk);
    chk->deleteLater();
}

void QMainPackingPackageOfPackedPackPacker::openFolder(QString path)
{ QDesktopServices::openUrl( QUrl::fromLocalFile( path ) ); }

void QMainPackingPackageOfPackedPackPacker::openGitRep()
{ QDesktopServices::openUrl( QUrl("https://github.com/Garibaldini85/BackpackPacker") ); }

void QMainPackingPackageOfPackedPackPacker::decodeFile(QString code, QString dirKey, QString dirFile, QString dirDecFile)
{
    if (dirKey     == ""
    ||  dirFile    == "")
    {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
    } else {
        QChecker checker(version, dirKey, code);
        checker.checkSecrKey();
        if (checker.Err != "") {
            emit sendError("Ошибка :(", checker.Err);
        } else {
            decoder = new QDecoder(dirKey, dirFile, dirDecFile);
            decoder->genIdTask();
            emit createTask(decoder->getIdTask(), "bayan");
            emit setIndeterminate(decoder->getIdTask(), false);
            emit setStatusTask(decoder->getIdTask(), 0);

            connect(decoder, &QDecoder::doneDecoding,
                    this, &QMainPackingPackageOfPackedPackPacker::deleteDecoder);
            connect(decoder, &QDecoder::sendNameTask,
                    this, &QMainPackingPackageOfPackedPackPacker::recNameTask);
            connect(decoder, &QDecoder::sendStatus,
                    this, &QMainPackingPackageOfPackedPackPacker::recStatus);

            QtConcurrent::run(decoder, &QDecoder::decode);
        }
    }

}

void QMainPackingPackageOfPackedPackPacker::deleteDecoder(QDecoder *dec)
{
    emit deleteTask(dec->getIdTask());
    emit viewSystemNotif(dec->getIdTask(), "Данные расшифрованы", dec->getPathToFile());
    disconnect(dec);
    dec->deleteLater();
}

void QMainPackingPackageOfPackedPackPacker::checkValidKey(QString code, QString dirOpenKey, QString dirSecrKey)
{
    if (code == "" || dirOpenKey == "" || dirSecrKey == "") {
        emit sendError("Ошибка :(", "Неправильный ввод получается(((");
    } else {
        QChecker *checker = new QChecker(version, dirOpenKey, dirSecrKey, code);
        checker->genIdTask();
        emit createTask(checker->getIdTask(), "Инициализация структуры");
        emit setIndeterminate(checker->getIdTask(), true);
        emit setStatusTask(checker->getIdTask(), 0);

        connect(checker, &QChecker::doneCheck,
                this, &QMainPackingPackageOfPackedPackPacker::deleteChecker);
        connect(checker, &QChecker::sendNameTask,
                this, &QMainPackingPackageOfPackedPackPacker::recNameTask);
        connect(checker, &QChecker::sendStatus,
                this, &QMainPackingPackageOfPackedPackPacker::recStatus);
        //QString strLog = checker.checkKeys();
        QtConcurrent::run(checker, &QChecker::checkKeys);
    }
}

void QMainPackingPackageOfPackedPackPacker::recNameTask(QString idTask, QString nameTask)
{ emit setNameTask(idTask, nameTask); }
