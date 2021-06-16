#include "qgeneratorkey.h"

QGeneratorKey::QGeneratorKey(int bytesSize, int blockSize)
{
    this->bytesSize = bytesSize;
    this->blockSize = blockSize;
    m = BN_new();
    t = BN_new();
    u = BN_new();
}

QGeneratorKey::~QGeneratorKey()
{
    BN_free(m);
    BN_free(t);
    BN_free(u);
}

int QGeneratorKey::bn_count_bits (BIGNUM *bn)
{
    BIGNUM *num = BN_new();
    BN_copy(num, bn);
    int n = 1, counter = 0;
    while (strcmp(BN_bn2dec(num), "0") != 0)
    {
        BN_rshift(num, num, n);
        counter += 1;
    }
    BN_free(num);

    return counter;
}

bool QGeneratorKey::generateM(QVector <BIGNUM *> vec_secKey)
{
    BIGNUM *m = BN_new(), *sum = BN_new();
    BN_dec2bn(&m, "2");
    BN_dec2bn(&sum, "0");

    for (auto x: vec_secKey)
        BN_add(sum, sum, x);

    while (BN_cmp(m, vec_secKey[vec_secKey.size() - 1]) != 1)
        BN_generate_prime_ex(m, bn_count_bits(sum), 0, NULL, NULL, NULL);

    BN_copy(this->m, m);
    BN_free(m);
    BN_free(sum);

    return true;
}

bool QGeneratorKey::generateT()
{
    BIGNUM *_m_2 = BN_new();
    BN_CTX *context = BN_CTX_new();

    BN_dec2bn(&t, "2");
    BN_div(_m_2, NULL, m, t, context);

    while (BN_cmp(t, _m_2) != 1 || BN_cmp(t, m) != -1) {
        BN_rand(t, Random::get(bn_count_bits(_m_2), bn_count_bits(m)), NULL, NULL);
    }

    BN_mod_inverse(u, t, m, context);
    BN_free(_m_2);
    BN_CTX_free(context);

    return true;
}

void QGeneratorKey::generateSecKey()
{
    BN_CTX *context = BN_CTX_new();
    BIGNUM *add = BN_new(), *ans = BN_new(), *step = BN_new(), *_one = BN_new();

    BN_rand(add, bytesSize, 1234567890, NULL);

    for (int i = 0; i < blockSize * 8; i++) {
        QString str = BN_bn2dec(add);
        BN_rand(step, bytesSize / 2, 1234567890, NULL);

        int x = Random::get(str.size() - str.size() / 2, str.size() - 1);
        str.remove(str.size() - x - 1, x);
        if ( str[0] == "0" ) {
            str[0] = '1';
        }

        BN_mul(add, add, step, context);
        BN_dec2bn(&ans, str.toLocal8Bit().data());
        BN_add(add, add, ans);

        BIGNUM *_add = BN_new();
        BN_copy(_add, add);
        str = BN_bn2dec(_add);

        secVec.push_back(_add);
        emit sendSecCounter(i + 1);
    }

    generateM(secVec);
    generateT();

    BN_free(add);
    BN_free(ans);
    BN_free(step);
    BN_free(_one);
    BN_CTX_free(context);

    emit sendCompletedFindSec(true);
    emit doneGenSec();
}

void QGeneratorKey::generateOpenKey()
{
    BN_CTX *context = BN_CTX_new();
    int counter = 0;

    for (auto i: qAsConst(secVec)) {
        emit sendOpenCounter(counter);
        counter ++;
        BIGNUM *add = BN_new();
        BN_mod_mul(add, i, t, m, context);
        openVec.push_back(add);
    }

    emit doneGenOpen();
}

BIGNUM *QGeneratorKey::getM()
{
    return m;
}

BIGNUM *QGeneratorKey::getT()
{
    return t;
}

QVector<BIGNUM *> QGeneratorKey::getOpenKey()
{
    return openVec;
}

QVector<BIGNUM *> QGeneratorKey::getSecKey()
{
    return secVec;
}
