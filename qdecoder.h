#ifndef QDECODER_H
#define QDECODER_H

#include "openssl/bn.h"

#include "qfilehandler.h"
#include "qtotalsqueezer.h"

#include <iostream>
#include <bitset>
#include <cstring>

#include <QObject>
#include <QtDebug>
#include <QTextCodec>
#include <QFileInfo>

class QDecoder : public QObject
{
    Q_OBJECT
public:
    QDecoder(QString &dirKey, QString &dirEncFile, QString &dirFile);


private:
    QFileHandler *handler;
    QString dirKey, dirFile, dirEncFile;
    long long sizeF;

    QString getTextBlock(QString bits);

public slots:
    void decode();

signals:
    void doneDecoding();
    void statusDecode(long long status);
    void sizeFile(long long size);

};

#endif // QDECODER_H
