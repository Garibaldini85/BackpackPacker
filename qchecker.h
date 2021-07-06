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

#ifndef QCHECKER_H
#define QCHECKER_H

#include "qtotalsqueezer.h"

#include <QObject>
#include <QFile>

#include "openssl/bn.h"

class QChecker : public QObject
{
    Q_OBJECT
public:
    QString Err;

    explicit QChecker(QObject *parent = nullptr);
    QChecker (QString &version, QString &dirOpenKey);
    QChecker (QString &version, QString &dirSecrKey, QString &code);
    QChecker (QString &version, QString &dirOpenKey, QString &dirSecrKey, QString &code);

    void genIdTask();
    QString getIdTask();
    QString getDirOpenKey();
    QString getDirSecrKey();

private:
    QString dirOpenKey, dirSecrKey, code, version, idTask;

public slots:
    void checkOpenKey();
    void checkSecrKey();
    void checkKeys();

signals:
    void doneCheck(QChecker *chk);
    void sendStatus(QString idTask, double status);
    void sendNameTask(QString idTask, QString nameTask);
};

#endif // QCHECKER_H
