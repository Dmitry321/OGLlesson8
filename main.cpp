#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // для увеличения детализации
    QSurfaceFormat  format;
    format.setSamples(16);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
