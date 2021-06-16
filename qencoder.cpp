#include "qencoder.h"

QEncoder::QEncoder(QString &dirKey, QString &dirFile, QString &dirEncFile)
{
    this->dirKey = dirKey;
    this->dirFile = dirFile;
    QFileInfo suf(dirEncFile);
    QFileInfo sz(dirFile);
    sizeF = sz.size();
    if (suf.suffix() == "")
        this->dirEncFile = dirEncFile + "." + dirFile.split(".")[dirFile.split(".").size() - 1];
    else
        this->dirEncFile = dirEncFile;
}

QEncoder::~QEncoder()
{
}

QString QEncoder::getBinBlock(QString &array, int &blockSize)
{
    QString retStr;

    for (int i = 0; i < array.size(); i++) {
        std::bitset<16> binCh (array[i].unicode());
        //qDebug() << QString::fromStdString(binCh.to_string()) << " ~ "<< array[i] << " ~ " << array[i].unicode();
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

void QEncoder::encode()
{
    emit sizeFile(sizeF);
    sizeF = 0;

    QFileHandler handler;
    QTotalSqueezer squeezer;
    QVector <BIGNUM*> vec_openKey = handler.readOpenKey(dirKey);
    BIGNUM *ans = BN_new();
    int blockSize = vec_openKey.size() / 16;
    handler.openBinFile(dirFile, dirEncFile, blockSize);
    QString x, bits;
    x = handler.readNextBlock();

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

        sizeF += blockSize;
        emit statusEncode(sizeF);
    }
    x.clear();
    BN_free(ans);
    for (int i = 0; i < vec_openKey.size(); i++)
        BN_free(vec_openKey[i]);
    vec_openKey.clear();
    handler.closeFile();

    emit doneEncoding();
}
