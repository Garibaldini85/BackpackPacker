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

#include "qchecker.h"

QChecker::QChecker(QObject *parent) : QObject(parent) {}

QChecker::QChecker(QString &version, QString &dirOpenKey)
{
    this->version = version;
    this->dirOpenKey = dirOpenKey;
}

QChecker::QChecker(QString &version, QString &dirSecrKey, QString &code)
{
    this->code = code;
    this->version = version;
    this->dirSecrKey = dirSecrKey;
}

QChecker::QChecker(QString &version, QString &dirOpenKey, QString &dirSecrKey, QString &code)
{
    this->code = code;
    this->version = version;
    this->dirSecrKey = dirSecrKey;
    this->dirOpenKey = dirOpenKey;
}

void QChecker::genIdTask()
{ idTask = QDateTime::currentDateTime().toString("hh.mm:ss"); }

QString QChecker::getIdTask()
{ return idTask; }

QString QChecker::getDirOpenKey()
{ return dirOpenKey; }

QString QChecker::getDirSecrKey()
{ return dirSecrKey; }

void QChecker::checkOpenKey()
{
    QFile file(dirOpenKey);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString str = file.readLine();
    str.remove('\n');
    file.close();
    if (str == version)
        Err = "";
    else
        Err = "Invalid Version.";
}

void QChecker::checkSecrKey()
{
    QTotalSqueezer squeezer;
    QFile file(dirSecrKey);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    //ver
    QString str = file.readLine();
    str.remove('\n');
    if (str != version) {
        file.close();
        Err = "Invalid Version";
        return;
    }

    file.readLine();
    file.readLine();

    //code
    str = file.readLine();
    str.remove('\n');
    file.close();

    if (squeezer.decodeIntoUtf(str) != QString::number(qChecksum((code + "salt").toLatin1().data(), code.size() + 4)))
        Err = "Invalid Code.";
}

void QChecker::checkKeys()
{
    QTotalSqueezer squeezer;
    QFile secr(dirSecrKey), open(dirOpenKey);
    secr.open(QIODevice::ReadOnly | QIODevice::Text);
    open.open(QIODevice::ReadOnly | QIODevice::Text);
    //ver
    QString strS = secr.readLine(), strO = open.readLine();
    strS.remove('\n');
    strO.remove('\n');

    emit sendNameTask(idTask, "Проверка версии");
    if (!(strS == strO && strS == version)) {
        secr.close(); open.close();
        Err = "Version Error.";
        emit doneCheck(this);
        return;
    }

    emit sendNameTask(idTask, "Проверка кода");
    QString _m = secr.readLine(), _t = secr.readLine();
    strS = secr.readLine();
    strS.remove('\n');

    if (!(squeezer.decodeIntoUtf(strS) == QString::number(qChecksum((code + "salt").toLatin1().data(), code.size() + 4))))
    {
        secr.close(); open.close();
        Err = "Code Error.";
        emit doneCheck(this);
        return;
    }

    BN_CTX *context = BN_CTX_new();
    BIGNUM *m = BN_new(), *t = BN_new(), *i = BN_new();
    BN_dec2bn(&m, squeezer.decodeIntoUtf(_m).toLatin1().data());
    BN_dec2bn(&t, squeezer.decodeIntoUtf(_t).toLatin1().data());

    emit sendNameTask(idTask, "Проверка ключей");
    while (!secr.atEnd()) {
        QString num = secr.readLine(); num.remove('\n');
        QString _num = open.readLine(); _num.remove('\n');
        BN_dec2bn(&i, squeezer.decodeIntoUtf(num).toLatin1().data());
        BIGNUM *add = BN_new(), *secNum = BN_new();
        BN_dec2bn(&secNum, squeezer.decodeIntoUtf(_num).toLatin1().data());
        BN_mod_mul(add, i, t, m, context);

        if (BN_cmp(secNum, add) != 0) {
            secr.close(); open.close();
            Err = "Invalid Open Key";
            emit doneCheck(this);
            return;
        }
        BN_free(add); BN_free(secNum);
    }

    emit sendNameTask(idTask, "Очистка памяти");
    BN_free(m); BN_free(t); BN_free(i);
    secr.close(); open.close();
    Err = "";
    emit doneCheck(this);
}
