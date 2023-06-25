#include "filehashthread.h"

#include <QFile>

FileHashThread::FileHashThread(QStringList pathList, QObject* parent) : QThread(parent), pathList(pathList)
{
    md4 = new QCryptographicHash(QCryptographicHash::Md4);
    md5 = new QCryptographicHash(QCryptographicHash::Md5);
    sha1 = new QCryptographicHash(QCryptographicHash::Sha1);
    sha256 = new QCryptographicHash(QCryptographicHash::Sha256);
    sha512 = new QCryptographicHash(QCryptographicHash::Sha512);
}

FileHashThread::~FileHashThread()
{
    delete md4;
    delete md5;
    delete sha1;
    delete sha256;
    delete sha512;
}

void FileHashThread::run()
{
    emit hashStarted();

    for (int i = 0; i < pathList.size(); i++)
    {
        hash(pathList[i]);
        emit hashIndexChanged(i + 1);
    }

    emit hashEnded();
}

void FileHashThread::hash(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        emit hashError("文件打开失败");

    long long size = 0;
    long long fileSize = file.size();

    while (true)
    {
        QByteArray byteArray = file.read(bufferSize);

        if (size != fileSize)
        {
            md4->addData(byteArray);
            md5->addData(byteArray);
            sha1->addData(byteArray);
            sha256->addData(byteArray);
            sha512->addData(byteArray);

            size += byteArray.size();
            emit hashProgressChanged(size, fileSize);
        }
        else
            break;
    }

    file.close();

    QString _md4 = md4->result().toHex();
    QString _md5 = md5->result().toHex();
    QString _sha1 = sha1->result().toHex();
    QString _sha256 = sha256->result().toHex();
    QString _sha512 = sha512->result().toHex();

    FileHash FileHash(path, fileSize, _md4, _md5, _sha1, _sha256, _sha512);
    emit hashResult(FileHash);
}
