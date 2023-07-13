#ifndef FILEHASHTHREAD_H
#define FILEHASHTHREAD_H

#include <QThread>
#include <QCryptographicHash>

class FileHashThread : public QThread
{
    Q_OBJECT

public:
    // 构造函数
    FileHashThread();
    // 析构函数
    ~FileHashThread();

    // 线程函数
    void run() Q_DECL_OVERRIDE;

    // 设置待计算文件集合
    void setFileList(QStringList fileList);
    // 设置是否停止计算
    void setStop(bool isStop);

private:
    // 计算文件哈希
    void hash(QString path);

private:
    // 待计算文件集合
    QStringList fileList;

    // 是否停止计算
    bool isStop;
    // 文件缓冲区
    int bufferSize;

    // 校验算法
    QCryptographicHash* md4;
    QCryptographicHash* md5;
    QCryptographicHash* sha1;
    QCryptographicHash* sha256;
    QCryptographicHash* sha512;

signals:
    // 哈希校验开始
    void hashStarted(int count);
    // 哈希校验结束
    void hashEnded();
    // 哈希校验错误
    void hashError(QString error);
    // 哈希校验进度改变
    void hashProgressChanged(int progress);
    // 哈希校验索引改变
    void hashIndexChanged(int index);
    // 哈希校验结果
    void hashResult(QString result);
};

#endif // FILEHASHTHREAD_H
