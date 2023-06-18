#ifndef FILEHASHTHREAD_H
#define FILEHASHTHREAD_H

#include "filehash.h"

#include <QThread>
#include <QCryptographicHash>

class FileHashThread : public QThread
{
    Q_OBJECT

public:
    FileHashThread(QString path, QObject* parent = nullptr);
    ~FileHashThread();

    void run();

private:
    QString path;

    // 文件缓冲区，4KB
    int bufferSize = 4096;

    // 校验算法
    QCryptographicHash* md4 = nullptr;
    QCryptographicHash* md5 = nullptr;
    QCryptographicHash* sha1 = nullptr;
    QCryptographicHash* sha256 = nullptr;
    QCryptographicHash* sha512 = nullptr;

signals:
    void hashStarted();
    void hashFinished(FileHash fileHash);
    void hashError(QString error);

    void hashProgressChanged(int size, long fileSize);
};

#endif // FILEHASHTHREAD_H
