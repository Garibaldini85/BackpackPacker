#ifndef QENCODER_H
#define QENCODER_H

#include "openssl/bn.h"

#include "qfilehandler.h"
#include "qtotalsqueezer.h"
#include "qchecker.h"

#include <iostream>
#include <bitset>
#include <cstring>

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

class QEncoder : public QObject
{
    Q_OBJECT
public:
    QEncoder(QString &dirKey, QString &dirFile, QString &dirEncFile);
    ~QEncoder();

private:
    QFileHandler *handler;
    QString dirKey, dirFile, dirEncFile;
    long long sizeF;

    QString getBinBlock(QString &array, int &blockSize);
    QString appendAddStr(QString &str, int &blockSize);

public slots:
    void encode();

signals:
    void doneEncoding();
    void sizeFile(long long size);
    void statusEncode(long long bytes);
};

#endif // QENCODER_H
