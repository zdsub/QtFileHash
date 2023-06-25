#ifndef FILEHASH_H
#define FILEHASH_H

#include <QString>

class FileHash
{
public:
    FileHash();
    FileHash(QString path, long size, QString md4, QString md5, QString sha1, QString sha256, QString sha512);
    ~FileHash();

    QString path();
    long size();
    QString md4();
    QString md5();
    QString sha1();
    QString sha256();
    QString sha512();

    QString toString();

private:
    QString _path;
    long _size;
    QString _md4;
    QString _md5;
    QString _sha1;
    QString _sha256;
    QString _sha512;
};

#endif // FILEHASH_H
