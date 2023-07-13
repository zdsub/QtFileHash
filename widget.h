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
    // 构造函数
    Widget();
    // 析构函数
    ~Widget();

    // 拖拽事件
    void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

private slots:
    // 重置进度条
    void resetProgressBar(int count);
    // 添加信息
    void addMessage(QString message);
    // 当前进度改变
    void currentChange(int progress);
    // 总进度改变
    void totalChange(int index);

    // 自动槽函数
    void on_openButton_clicked();
    void on_clearButton_clicked();
    void on_saveButton_clicked();
    void on_copyButton_clicked();
    void on_stopButton_clicked();

private:
    Ui::Widget *ui;

    // 哈希校验线程
    FileHashThread* fileHashThread;

    // 开始校验
    void startHash(QStringList fileList);
};
#endif // WIDGET_H
