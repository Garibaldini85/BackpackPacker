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

#include "qtotalsqueezer.h"

QTotalSqueezer::QTotalSqueezer(QObject *parent) : QObject(parent)
{
    num = BN_new();
    d = BN_new();
    h = BN_new();
    temp = BN_new();
    rem = BN_new();
}

QTotalSqueezer::~QTotalSqueezer()
{
    BN_clear_free(num);
    BN_clear_free(d);
    BN_clear_free(h);
    BN_clear_free(temp);
    BN_clear_free(rem);
}

QString QTotalSqueezer::decodeIntoUtf(QString &str)
{
    BN_clear(num);
    BN_clear(d);
    BN_clear(h);
    BN_clear(temp);
    BN_clear(rem);
    context = BN_CTX_new();
    BN_CTX_start(context);

    QString _str = str;
    _str.remove('\n');
    QString alpha("#!$%&()0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{|}:;<>+=^");
    BN_dec2bn(&num, "0");
    BN_dec2bn(&d, "1");
    BN_dec2bn(&h, QString::number(alpha.size()).toLatin1().data());

    for (auto x: _str) {
       BN_dec2bn(&temp, QString::number(alpha.indexOf(x)).toLatin1().data());
       BN_mul(temp, temp, d, context);
       BN_mul(d, d, h, context);
       BN_add(num, temp, num);
    }
    char *c = BN_bn2dec(num);
    QString strRet(c);
    OPENSSL_free(c);
    BN_CTX_end(context);
    BN_CTX_free(context);
    return strRet;
}

QString QTotalSqueezer::encodeToUtf(QString &str)
{
    BN_clear(num);
    BN_clear(d);
    BN_clear(h);
    BN_clear(temp);
    BN_clear(rem);
    context = BN_CTX_new();
    BN_CTX_start(context);
    QString strRet;
    QString alpha("#!$%&()0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{|}:;<>+=^");
    BN_dec2bn(&num, str.toLatin1().data());
    BN_dec2bn(&d, QString::number(alpha.size()).toLatin1().data());

    while (!BN_is_zero(num)) {
        BN_div(num, rem, num, d, context);
        char *_c = BN_bn2dec(rem);
        QChar c = alpha[QString::fromStdString(std::string(_c)).toInt()];
        OPENSSL_free(_c);
        strRet.push_back(c);
    }
    BN_CTX_end(context);
    BN_CTX_free(context);
    return strRet;
}

QByteArray QTotalSqueezer::compress(QString &str)
{
    QByteArray strRet = qCompress(str.toLatin1(), 9);
    return strRet;
}
