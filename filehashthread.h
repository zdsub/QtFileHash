#ifndef FILEHASHTHREAD_H
#define FILEHASHTHREAD_H

#include "filehash.h"

#include <QThread>
#include <QCryptographicHash>

class FileHashThread : public QThread
{
    Q_OBJECT

public:
    FileHashThread(QStringList pathList, QObject* parent = nullptr);
    ~FileHashThread();

    void run();

private:
    // 计算文件哈希
    void hash(QString path);

private:
    // 待计算文件路径集合
    QStringList pathList;

    // 文件缓冲区，4KB
    int bufferSize = 4096;

    // 校验算法
    QCryptographicHash* md4 = nullptr;
    QCryptographicHash* md5 = nullptr;
    QCryptographicHash* sha1 = nullptr;
    QCryptographicHash* sha256 = nullptr;
    QCryptographicHash* sha512 = nullptr;

signals:
    // 哈希校验开始
    void hashStarted();
    // 哈希校验结束
    void hashEnded();
    // 哈希校验错误
    void hashError(QString error);

    void hashProgressChanged(int size, long fileSize);
    void hashIndexChanged(int index);

    void hashResult(FileHash fileHash);
};

#endif // FILEHASHTHREAD_H
