#include "filehash.h"

FileHash::FileHash()
{

}

FileHash::FileHash(QString path, long size, QString md4, QString md5, QString sha1, QString sha256, QString sha512) :
    _path(path), _size(size), _md4(md4), _md5(md5), _sha1(sha1), _sha256(sha256), _sha512(sha512)
{

}

FileHash::~FileHash()
{

}

QString FileHash::path()
{
    return _path;
}

long FileHash::size()
{
    return _size;
}

QString FileHash::md4()
{
    return _md4;
}

QString FileHash::md5()
{
    return _md5;
}

QString FileHash::sha1()
{
    return _sha1;
}

QString FileHash::sha256()
{
    return _sha256;
}

QString FileHash::sha512()
{
    return _sha512;
}

QString FileHash::toString()
{
    QString message = "文件: %1\n"
                      "大小: %2字节\n"
                      "MD4: %3\n"
                      "MD5: %4\n"
                      "SHA1: %5\n"
                      "SHA256: %6\n"
                      "SHA512: %7\n";
    return message.arg(_path, QString::number(_size), _md4, _md5, _sha1, _sha256, _sha512);
}
