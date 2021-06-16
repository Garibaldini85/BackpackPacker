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
    explicit QChecker(QObject *parent = nullptr);
    QChecker (QString &version, QString &dirOpenKey);
    QChecker (QString &version, QString &dirSecrKey, QString &code);
    QChecker (QString &version, QString &dirOpenKey, QString &dirSecrKey, QString &code);

private:
    QString dirOpenKey, dirSecrKey, code, version;

public slots:
    QString checkOpenKey();
    QString checkSecrKey();
    QString checkKeys();

signals:

};

#endif // QCHECKER_H
