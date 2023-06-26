#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qRegisterMetaType<FileHash>("FileHash");

    connect(fileHashThread, &FileHashThread::hashStarted, this, &Widget::resetProgressBar);
    connect(fileHashThread, &FileHashThread::hashResult, this, static_cast<void (Widget::*)(FileHash)> (&Widget::addMessage));
    connect(fileHashThread, &FileHashThread::hashError, this, static_cast<void (Widget::*)(QString)> (&Widget::addMessage));
    connect(fileHashThread, &FileHashThread::hashProgressChanged, this, &Widget::currentChange);
    connect(fileHashThread, &FileHashThread::hashIndexChanged, this, &Widget::totalChange);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resetProgressBar(int count)
{
    ui->currentProgressBar->setValue(0);

    ui->totalProgressBar->setMaximum(count);
    ui->totalProgressBar->setValue(0);
}

void Widget::addMessage(FileHash fileHash)
{
    ui->messageTextEdit->appendPlainText(fileHash.toString());
}

void Widget::addMessage(QString error)
{
    ui->messageTextEdit->appendPlainText(error);
}

void Widget::currentChange(int progress)
{
    ui->currentProgressBar->setValue(progress);
}

void Widget::totalChange(int index)
{
    ui->totalProgressBar->setValue(index);
}


void Widget::on_openButton_clicked()
{
    QStringList pathList = QFileDialog::getOpenFileNames(this, "选择文件");
    if (pathList.size() != 0)
    {
        fileHashThread->setPathList(pathList);
        fileHashThread->start();
    }
}


void Widget::on_clearButton_clicked()
{
    ui->messageTextEdit->clear();
}

