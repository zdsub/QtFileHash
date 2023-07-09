#include "filehashthread.h"

#include <QFile>

FileHashThread::FileHashThread(QObject* parent) : QThread(parent)
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

void FileHashThread::setPathList(QStringList pathList)
{
    this->pathList = pathList;
}

void FileHashThread::run()
{
    emit hashStarted(pathList.size());

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
    {
        emit hashError(path + "\n文件打开失败\n");
        return;
    }

    long long size = file.size();
    long long currentSize = 0;

    while (true)
    {
        QByteArray byteArray = file.read(bufferSize);

        if (currentSize != size)
        {
            md4->addData(byteArray);
            md5->addData(byteArray);
            sha1->addData(byteArray);
            sha256->addData(byteArray);
            sha512->addData(byteArray);

            currentSize += byteArray.size();
            emit hashProgressChanged(static_cast<double>(currentSize) / size * 100);
        }
        else
            break;
    }

    file.close();

    // 获取校验结果
    QString _md4 = md4->result().toHex();
    QString _md5 = md5->result().toHex();
    QString _sha1 = sha1->result().toHex();
    QString _sha256 = sha256->result().toHex();
    QString _sha512 = sha512->result().toHex();

    // 重置检验结果
    md4->reset();
    md5->reset();
    sha1->reset();
    sha256->reset();
    sha512->reset();

    // 格式化校验结果
    QString message = "文件: %1\n"
                      "大小: %2字节\n"
                      "MD4: %3\n"
                      "MD5: %4\n"
                      "SHA1: %5\n"
                      "SHA256: %6\n"
                      "SHA512: %7\n";
    QString result = message.arg(path, QString::number(size), _md4, _md5, _sha1, _sha256, _sha512);

    emit hashResult(result);
}
