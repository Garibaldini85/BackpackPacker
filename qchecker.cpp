#include "qchecker.h"

QChecker::QChecker(QObject *parent) : QObject(parent)
{

}

QChecker::QChecker(QString &version, QString &dirOpenKey)
{
    this->version = version;
    this->dirOpenKey = dirOpenKey;
}

QChecker::QChecker(QString &version, QString &dirSecrKey, QString &code)
{
    this->code = code;
    this->version = version;
    this->dirSecrKey = dirSecrKey;
}

QChecker::QChecker(QString &version, QString &dirOpenKey, QString &dirSecrKey, QString &code)
{
    this->code = code;
    this->version = version;
    this->dirSecrKey = dirSecrKey;
    this->dirOpenKey = dirOpenKey;
}

QString QChecker::checkOpenKey()
{
    QFile file(dirOpenKey);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString str = file.readLine();
    str.remove('\n');
    file.close();
    if (str == version)
        return "";
    else
        return "Invalid Version.";
}

QString QChecker::checkSecrKey()
{
    QTotalSqueezer squeezer;
    QFile file(dirSecrKey);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    //ver
    QString str = file.readLine();
    str.remove('\n');
    if (str != version) {
        file.close();
        return "Invalid Version";
    }

    file.readLine();
    file.readLine();

    //code
    str = file.readLine();
    str.remove('\n');
    file.close();

    if (squeezer.decodeIntoUtf(str) != QString::number(qChecksum((code + "salt").toLatin1().data(), code.size() + 4)))
        return "Invalid Code.";
    return "";
}

QString QChecker::checkKeys()
{
    QTotalSqueezer squeezer;
    QFile secr(dirSecrKey), open(dirOpenKey);
    secr.open(QIODevice::ReadOnly | QIODevice::Text);
    open.open(QIODevice::ReadOnly | QIODevice::Text);
    //ver
    QString strS = secr.readLine(), strO = open.readLine();
    strS.remove('\n');
    strO.remove('\n');

    if (!(strS == strO && strS == version)) {
        secr.close(); open.close();
        return "Version Error.";
    }

    QString _m = secr.readLine(), _t = secr.readLine();
    strS = secr.readLine();
    strS.remove('\n');

    if (!(squeezer.decodeIntoUtf(strS) == QString::number(qChecksum((code + "salt").toLatin1().data(), code.size() + 4))))
    {
        secr.close(); open.close();
        return "Code Error.";
    }

    BN_CTX *context = BN_CTX_new();
    BIGNUM *m = BN_new(), *t = BN_new(), *i = BN_new();
    BN_dec2bn(&m, squeezer.decodeIntoUtf(_m).toLatin1().data());
    BN_dec2bn(&t, squeezer.decodeIntoUtf(_t).toLatin1().data());

    while (!secr.atEnd()) {
        QString num = secr.readLine(); num.remove('\n');
        QString _num = open.readLine(); _num.remove('\n');
        BN_dec2bn(&i, squeezer.decodeIntoUtf(num).toLatin1().data());
        BIGNUM *add = BN_new(), *secNum = BN_new();
        BN_dec2bn(&secNum, squeezer.decodeIntoUtf(_num).toLatin1().data());
        BN_mod_mul(add, i, t, m, context);

        if (BN_cmp(secNum, add) != 0) {
            secr.close(); open.close();
            return "Invalid Open Key";
        }
        BN_free(add); BN_free(secNum);
    }
    BN_free(m); BN_free(t); BN_free(i);
    secr.close(); open.close();
    return "Валидные ключи";
}
