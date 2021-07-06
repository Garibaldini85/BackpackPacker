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

#include "qencoder.h"

QEncoder::QEncoder(QString &dirKey, QString &dirFile, QString &dirEncFile)
{
    this->dirKey = dirKey;
    this->dirFile = dirFile;
    QFileInfo sz(dirFile);
    sizeF = sz.size();
    if (dirEncFile == "") {
        pathToFile = sz.absolutePath();
        this->dirEncFile = sz.absolutePath() + "/" + "enc_" + sz.baseName() + "." + sz.completeSuffix();
    } else {
        pathToFile = dirEncFile;
        this->dirEncFile = dirEncFile + "/" + "enc_" + sz.baseName() + "." + sz.completeSuffix();
    }
}

QEncoder::~QEncoder() {}

QString QEncoder::getBinBlock(QString &array, int &blockSize)
{
    QString retStr;

    for (int i = 0; i < array.size(); i++) {
        std::bitset<16> binCh (array[i].unicode());
        retStr.push_back(QString::fromStdString(binCh.to_string()));
    }
    return retStr;
}

QString QEncoder::appendAddStr(QString &str, int &blockSize)
{
    int add = blockSize - str.size();
    if (add >= 10)
        add = 9;
    for (int i = 0; i < add; i++)
        str.push_back(QString::number(add));
    if (str.size() < blockSize)
        appendAddStr(str, blockSize);
    return str;
}

void QEncoder::genIdTask()
{
    idTask = QDateTime::currentDateTime().toString("hh.mm:ss");
}

QString QEncoder::getIdTask()
{ return idTask; }

QString QEncoder::getDirEncFile()
{ return dirEncFile; }

QString QEncoder::getPathToFile()
{ return pathToFile; }

void QEncoder::encode()
{
    emit sendNameTask(idTask, "Считываю ключ");
    long long _sizeF = 0;

    QFileHandler handler;
    QTotalSqueezer squeezer;
    QVector <BIGNUM*> vec_openKey = handler.readOpenKey(dirKey);
    BIGNUM *ans = BN_new();
    int blockSize = vec_openKey.size() / 16;
    handler.openBinFile(dirFile, dirEncFile, blockSize);
    QString x, bits;
    x = handler.readNextBlock();

    emit sendNameTask(idTask, "Шифрую файл");
    while (x.size()) {

        bits = getBinBlock(x, blockSize);
        BN_clear(ans);
        for (int i = 0; i < vec_openKey.size(); i++) {
            if (bits[i] == "1") {
                BN_add(ans, ans, vec_openKey[i]);
            }
        }

        char *_c = BN_bn2dec(ans);
        QString writeBlock(_c);
        OPENSSL_free(_c);
        writeBlock = squeezer.encodeToUtf(writeBlock);
        writeBlock.push_back("\n");
        handler.writeNextBlock(writeBlock);
        writeBlock.clear();
        x.clear();
        x = handler.readNextBlock();

        _sizeF += blockSize;
        emit sendStatus(idTask, _sizeF * 1.0 / sizeF);
    }
    emit sendNameTask(idTask, "Очищаю память");
    x.clear();
    BN_free(ans);
    for (int i = 0; i < vec_openKey.size(); i++)
        BN_free(vec_openKey[i]);
    vec_openKey.clear();
    handler.closeFile();

    emit doneEncoding(this);
}
