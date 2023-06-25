#ifndef WIDGET_H
#define WIDGET_H

#include "filehashthread.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    // 重置进度条
    void resetProgressBar();
    // 添加信息 - 校验结果
    void addMessage(FileHash fileHash);
    // 添加信息 - 错误消息
    void addMessage(QString error);
    // 当前进度改变
    void currentChange(long long currentSize, long long totalSize);
    // 总进度改变
    void totalChange(int current, int total);

    void on_openButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::Widget *ui;

    FileHashThread* fileHashThread = new FileHashThread(this);
};
#endif // WIDGET_H
