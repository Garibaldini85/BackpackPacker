#include "qfilehandler.h"

QFileHandler::QFileHandler(QObject *parent) : QObject(parent)
{
    fileName = QDateTime::currentDateTime();
    m = BN_new();
    t = BN_new();
}

QFileHandler::QFileHandler(QString &ver)
{
    this->ver = ver;
    fileName = QDateTime::currentDateTime();
    m = BN_new();
    t = BN_new();
}

QFileHandler::~QFileHandler()
{

}

void QFileHandler::writeNewKey(QString code, QString dir, QVector<BIGNUM *> sec, QVector<BIGNUM *> open, BIGNUM *m, BIGNUM *t)
{
    QTotalSqueezer squeezer;
    QString _1 = QString(dir + "/" + fileName.toString("dd.MM.yyyy_hh.mm") + ".okey");
    _1.replace('\\', "/");
    QFile file(_1);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << ver << Qt::endl;
    for (auto x: open) {
        QString str = QString::fromStdString(std::string(BN_bn2dec(x)));
        out << squeezer.encodeToUtf(str) << Qt::endl;

    }

    QString _2 = QString(dir + "/" + fileName.toString("dd.MM.yyyy_hh.mm") + ".skey");
    _2.replace("\\", "/");
    QFile _file(_2);
    _file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream _out(&_file);

    QString _m = QString::fromStdString(std::string(BN_bn2dec(m)));
    QString _t = QString::fromStdString(std::string(BN_bn2dec(t)));

    _out << ver << Qt::endl;
    QString sum = QString::number(qChecksum((code + "salt").toLatin1().data(), code.size() + 4));
    _out << squeezer.encodeToUtf(_m) << Qt::endl << squeezer.encodeToUtf(_t) << Qt::endl << squeezer.encodeToUtf(sum) << Qt::endl;

    for (auto x: sec) {
        QString str = QString::fromStdString(std::string(BN_bn2dec(x)));
        _out << squeezer.encodeToUtf(str) << Qt::endl;

    }

    out.flush();
    file.close();
    _out.flush();
    _file.close();

    emit doneWriting();
}

QVector<BIGNUM*> QFileHandler::readOpenKey(QString &dirOpenKey)
{
    QTotalSqueezer squeezer;
    QFile file(dirOpenKey);
    QVector<BIGNUM*> retVec;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    file.readLine();
    while (!file.atEnd()) {
        QString str = file.readLine();
        str.remove('\n');
        str = squeezer.decodeIntoUtf(str);
        if (str != "") {
            BIGNUM *_add = BN_new();
            BN_dec2bn(&_add, str.toLocal8Bit().data());
            retVec.push_back(_add);
        }
    }

    return retVec;
}

QVector<BIGNUM *> QFileHandler::readSecrKey(QString &dirSecrKey)
{
    QTotalSqueezer squeezer;
    QFile file(dirSecrKey);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    file.readLine();
    QString _m = file.readLine().replace('\n', ""), _t = file.readLine().replace('\n', "");
    _m = squeezer.decodeIntoUtf(_m); _t = squeezer.decodeIntoUtf(_t);
    QVector<BIGNUM*> vecRet;

    BN_dec2bn(&m, _m.toLatin1().data());
    BN_dec2bn(&t, _t.toLatin1().data());

    file.readLine().replace('\n', "");
    while (!file.atEnd()) {
        QString r = file.readLine().replace('\n', "");
        r = squeezer.decodeIntoUtf(r);
        BIGNUM *add = BN_new();
        BN_dec2bn(&add, r.toLatin1().data());
        vecRet.push_back(add);
    }
    file.close();
    return vecRet;
}

BIGNUM *QFileHandler::getM()
{
    return m;
}

BIGNUM *QFileHandler::getT()
{
    return t;
}

void QFileHandler::openBinFile(QString &dirFileIn, QString &dirFileOut, int &blockSize)
{
    fileIn.setFileName(dirFileIn);
    fileIn.open(QIODevice::ReadOnly);
    streamIn.setDevice(&fileIn);

    fileOut.setFileName(dirFileOut);
    fileOut.open(QIODevice::WriteOnly | QIODevice::Text);
    streamOut.setDevice(&fileOut);

    this->blockSize = blockSize;
}

QString QFileHandler::readNextBlock()
{
    counter = 0;
    str = QString(blockSize, '\0');

    while (!streamIn.atEnd() && counter < blockSize) {
        streamIn >> c;
        str[counter] = c;
        counter++;
    }
    if (counter == 0)
        return "";
    else
        return str;
}

void QFileHandler::writeNextBlock(QString &str)
{
    for (i = 0; i < str.size(); i++) {
        streamOut << str[i];
    }
    streamOut.flush();
    fileOut.flush();
}

void QFileHandler::closeFile()
{
    streamIn.flush();
    streamOut.flush();
    fileOut.close();
    fileIn.close();
}

void QFileHandler::openForDecFile(QString &dirFile, QString &dirFileOut)
{
    fileIn.setFileName(dirFile);
    fileIn.open(QIODevice::ReadOnly | QIODevice::Text);

    fileOut.setFileName(dirFileOut);
    fileOut.open(QIODevice::WriteOnly);
    streamOut.setDevice(&fileOut);
}

QString QFileHandler::readNextEncBlock()
{
    return fileIn.readLine();
}
