#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QMimeData>

Widget::Widget()
    : ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    fileHashThread = new FileHashThread();
    connect(fileHashThread, SIGNAL(hashStarted(int)), this, SLOT(resetProgressBar(int)));
    connect(fileHashThread, SIGNAL(hashResult(QString)), this, SLOT(addMessage(QString)));
    connect(fileHashThread, SIGNAL(hashError(QString)), this, SLOT(addMessage(QString)));
    connect(fileHashThread, SIGNAL(hashProgressChanged(int)), this, SLOT(currentChange(int)));
    connect(fileHashThread, SIGNAL(hashIndexChanged(int)), this, SLOT(totalChange(int)));
    connect(fileHashThread, SIGNAL(hashEnded()), this, SLOT(resetOpen()));
}

Widget::~Widget()
{
    delete fileHashThread;
    delete ui;  
}

void Widget::dragEnterEvent(QDragEnterEvent* event)
{
    if (ui->openButton->isEnabled() && event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();

        int i = 0;
        while (i < urls.length())
        {
            QString path = urls[i].toLocalFile();
            QFileInfo info(path);

            if (!info.isFile())
                break;

            i++;
        }

        if (i == urls.length())
        {
            event->acceptProposedAction();
            return;
        }

    }

    event->ignore();
}

void Widget::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    QStringList fileList;
    for (int i = 0; i < urls.count(); i++)
        fileList << urls[i].toLocalFile();

    startHash(fileList);
}

void Widget::closeEvent(QCloseEvent* event)
{
    fileHashThread->setStop(true);
    fileHashThread->wait();

    event->accept();
}

void Widget::resetProgressBar(int count)
{
    ui->currentProgressBar->setValue(0);

    ui->totalProgressBar->setMaximum(count);
    ui->totalProgressBar->setValue(0);
}

void Widget::addMessage(QString message)
{
    ui->clearButton->setEnabled(true);
    ui->copyButton->setEnabled(true);
    ui->saveButton->setEnabled(true);

    ui->messageTextEdit->appendPlainText(message);
}

void Widget::currentChange(int progress)
{
    ui->currentProgressBar->setValue(progress);
}

void Widget::totalChange(int index)
{
    ui->totalProgressBar->setValue(index);
}

void Widget::resetOpen()
{
    ui->stopButton->setEnabled(false);
    ui->openButton->setEnabled(true);
}

void Widget::on_openButton_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this, NULL, NULL, "所有文件(*.*)");
    if (fileList.size() > 0)
        startHash(fileList);
}

void Widget::on_clearButton_clicked()
{
    ui->clearButton->setEnabled(false);
    ui->copyButton->setEnabled(false);
    ui->saveButton->setEnabled(false);

    ui->messageTextEdit->clear();
}

void Widget::on_saveButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, NULL, "hash.txt", "文本文件(*.txt);;所有文件(*.*)");
    if (path.isEmpty())
        return;

    QString message = ui->messageTextEdit->toPlainText();

    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(message.toUtf8());
        file.close();
    }
    else
        QMessageBox::critical(this, "错误", "文件打开失败");
}

void Widget::on_copyButton_clicked()
{
    QString message = ui->messageTextEdit->toPlainText();
    QApplication::clipboard()->setText(message);
}

void Widget::startHash(QStringList fileList)
{
    ui->openButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    fileHashThread->setFileList(fileList);
    fileHashThread->setStop(false);
    fileHashThread->start();
}

void Widget::on_stopButton_clicked()
{
    ui->stopButton->setEnabled(false);
    fileHashThread->setStop(true);
}
