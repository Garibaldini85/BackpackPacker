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

#include "qdecoder.h"

QDecoder::QDecoder(QString &dirKey, QString &dirEncFile, QString &dirFile)
{
    this->dirKey = dirKey;
    this->dirFile = dirFile;
    this->dirEncFile = dirEncFile;
    QFileInfo sz(dirEncFile);

    if (dirFile == "") {
        pathToFile = sz.absolutePath();
        this->dirFile = sz.absolutePath() + "/" + "dec_" + sz.baseName() + "." + sz.completeSuffix();
    } else {
        pathToFile = dirFile;
        this->dirFile = dirFile + "/" + "dec_" + sz.baseName() + "." + sz.completeSuffix();
    }
    sizeF = QFileInfo(dirEncFile).size();
}

QString QDecoder::getTextBlock(QString bits)
{
    int blockSize = bits.size() / 16;
    QString retStr;

    for (int i = 0; i < blockSize; i++) {
        std::bitset<16> bitCh;
        for (int j  = 0; j < 16; j++) {
            if (bits[i * 16 + j] == '1')
                bitCh[15 - j] = 1;
            else
                bitCh[15 - j] = 0;
        }
        QChar c(int(bitCh.to_ulong()));
        retStr.push_back(c);
    }
    return retStr;
}

void QDecoder::genIdTask()
{ idTask = QDateTime::currentDateTime().toString("hh.mm:ss"); }

QString QDecoder::getIdTask()
{ return idTask; }

QString QDecoder::getFile()
{ return dirFile; }

QString QDecoder::getPathToFile()
{ return pathToFile; }

void QDecoder::decode()
{
    //emit sizeFile(sizeF);
    emit sendNameTask(idTask, "Считываю ключ");
    long long _sizeF = 0;

    QFileHandler handler;
    QTotalSqueezer squeezer;
    auto vecKey = handler.readSecrKey(dirKey);
    BIGNUM *m = handler.getM(), *t = handler.getT(), *u = BN_new();

    BN_CTX *context = BN_CTX_new();
    handler.openForDecFile(dirEncFile, dirFile);
    QString str = handler.readNextEncBlock();

    emit sendNameTask(idTask, "Дешифрую файл");
    while (str != "") {
        _sizeF += str.size();
        str = squeezer.decodeIntoUtf(str);
        BIGNUM * _x = BN_new();
        BN_mod_inverse(u, t, m, context);
        BN_dec2bn(&_x, str.toLatin1().data());
        BN_mod_mul(_x, _x, u, m, context);
        QString bits(vecKey.size(), '0');

        for (int i = 0; i < vecKey.size(); i++) {
            if (BN_cmp(_x, vecKey[vecKey.size() - i - 1]) == -1) {
                bits[vecKey.size() - i - 1] = '0';
            } else if (BN_cmp(_x, vecKey[vecKey.size() - i - 1]) == 0) {
                bits[vecKey.size() - i - 1] = '1';
                BN_sub(_x, _x, vecKey[vecKey.size() - i - 1]);
            } else {
                bits[vecKey.size() - i - 1] = '1';
                BN_sub(_x, _x, vecKey[vecKey.size() - i - 1]);
            }
        }

        BN_free(_x);
        QString writeBlock = getTextBlock(bits);
        handler.writeNextBlock(writeBlock);
        str = handler.readNextEncBlock();
        emit sendStatus(idTask, _sizeF * 1.0 / sizeF);
    }

    emit sendNameTask(idTask, "Очищаю память");
    handler.closeFile();
    BN_CTX_free(context);
    BN_free(m);
    BN_free(t);
    BN_free(u);

    emit doneDecoding(this);
}
