#include <QApplication>
#include "imageprocessor_2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    imageprocessor_2 w;
    w.show();
    return a.exec();
}
