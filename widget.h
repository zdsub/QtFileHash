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
    void resetProgressBar(int count);
    // 添加信息
    void addMessage(QString message);
    // 当前进度改变
    void currentChange(int progress);
    // 总进度改变
    void totalChange(int index);

    void on_openButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::Widget *ui;

    FileHashThread* fileHashThread = new FileHashThread(this);
};
#endif // WIDGET_H
