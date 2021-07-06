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

#ifndef QTOTALSQUEEZER_H
#define QTOTALSQUEEZER_H

#include "openssl/bn.h"

#include "qfilehandler.h"

#include <iostream>
#include <bitset>
#include <cstring>

#include <QObject>

class QTotalSqueezer : public QObject
{
    Q_OBJECT
public:
    explicit QTotalSqueezer(QObject *parent = nullptr);
    ~QTotalSqueezer();

private:
    BIGNUM *num, *rem, *d, *h, *temp;
    BN_CTX *context;
    std::string strStd;

public slots:
    QString decodeIntoUtf (QString &str);
    QString encodeToUtf(QString &str);
    QByteArray compress(QString &str);

signals:

};

#endif // QTOTALSQUEEZER_H
