#include "filehashthread.h"

#include <QFile>
#include <QFileInfo>

FileHashThread::FileHashThread()
{
    isStop = false;
    bufferSize = 4096;

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

void FileHashThread::setFileList(QStringList fileList)
{
    this->fileList = fileList;
}

void FileHashThread::setStop(bool isStop)
{
    this->isStop = isStop;
}

void FileHashThread::run()
{
    int size = fileList.size();

    if (size > 0)
    {
        emit hashStarted(size);

        for (int i = 0; i < size && !isStop; i++)
            hash(fileList[i], i);

        emit hashEnded();
    }
}

void FileHashThread::hash(QString filePath, int index) {
    QFileInfo info(filePath);

    if (!info.exists())
    {
        emitError(index, "文件不存在");
        return;
    }
    else if (!info.isFile())
    {
        emitError(index, "不是文件");
        return;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        emitError(index, "打开文件失败");
        return;
    }

    long long size = 0;
    long long fileSize = file.size();

    if (fileSize != 0)
    {
        while (size < fileSize && !isStop)
        {
            QByteArray byteArray = file.read(bufferSize);

            md4->addData(byteArray);
            md5->addData(byteArray);
            sha1->addData(byteArray);
            sha256->addData(byteArray);
            sha512->addData(byteArray);

            size += byteArray.size();
            emit hashProgressChanged(static_cast<double>(size) / fileSize * 100);
        }
    }
    else
        emit hashProgressChanged(100);

    file.close();

    // 完成计算
    if (size == fileSize)
    {
        // 获取校验结果
        QString _md4 = md4->result().toHex().toUpper();
        QString _md5 = md5->result().toHex().toUpper();
        QString _sha1 = sha1->result().toHex().toUpper();
        QString _sha256 = sha256->result().toHex().toUpper();
        QString _sha512 = sha512->result().toHex().toUpper();

        // 格式化校验结果
        QString message = "文件: %1\n"
                          "大小: %2字节\n"
                          "MD4: %3\n"
                          "MD5: %4\n"
                          "SHA1: %5\n"
                          "SHA256: %6\n"
                          "SHA512: %7\n";
        QString result = message.arg(filePath, QString::number(fileSize), _md4, _md5, _sha1, _sha256, _sha512);

        emit hashResult(result);
        emit hashIndexChanged(index + 1);
    }

    // 重置检验结果
    md4->reset();
    md5->reset();
    sha1->reset();
    sha256->reset();
    sha512->reset();
}

void FileHashThread::emitError(int index, QString error)
{
    emit hashError(fileList[index] + "\n" + error + "\n");
    emit hashProgressChanged(100);
    emit hashIndexChanged(index + 1);
}
