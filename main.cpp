#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    if (argc > 1)
    {
        QStringList pathList;
        for (int i = 1; i < argc; i++)
            pathList << QString(argv[i]);

        w.startHash(pathList);
    }

    return a.exec();
}
