#ifndef FILEHASHTHREAD_H
#define FILEHASHTHREAD_H

#include "filehash.h"

#include <QThread>
#include <QCryptographicHash>

class FileHashThread : public QThread
{
    Q_OBJECT

public:
    FileHashThread(QObject* parent = nullptr);
    ~FileHashThread();

    void run();
    // 设置待校验文件路径集合
    void setPathList(QStringList pathList);

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
    // 哈希校验进度改变
    void hashProgressChanged(int size, long fileSize);
    // 哈希校验索引改变
    void hashIndexChanged(int index);
    // 哈希校验结果
    void hashResult(FileHash fileHash);
};

#endif // FILEHASHTHREAD_H
