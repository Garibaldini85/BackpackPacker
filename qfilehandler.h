#ifndef QFILEHANDLER_H
#define QFILEHANDLER_H

#include "qtotalsqueezer.h"

#include <QtDebug>
#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDataStream>

#include "openssl/bn.h"

class QFileHandler : public QObject
{
    Q_OBJECT
public:
    explicit QFileHandler(QObject *parent = nullptr);
    QFileHandler(QString &ver);
    ~QFileHandler();

private:
    QDateTime    fileName;
    QFile        fileIn, fileOut;
    QTextStream  streamIn, streamOut;
    //uint         counter;
    BIGNUM *m, *t;
    QString ver, str;
    int i, blockSize, counter;
    QChar c;

public slots:
    void             writeNewKey(QString code, QString dir, QVector <BIGNUM*> sec, QVector <BIGNUM*> open, BIGNUM* m, BIGNUM* t);
    QVector<BIGNUM*> readOpenKey(QString &dirOpenKey);
    QVector<BIGNUM*> readSecrKey (QString &dirSecrKey);
    BIGNUM*          getM();
    BIGNUM*          getT();

    void       openBinFile(QString &dirFile, QString &dirFileOut, int &blockSize);
    QString    readNextBlock();
    void       writeNextBlock(QString &str);
    void       closeFile();

    void    openForDecFile(QString &dirFile, QString &dirFileOut);
    QString readNextEncBlock();

signals:
    void doneWriting();
};

#endif // QFILEHANDLER_H
