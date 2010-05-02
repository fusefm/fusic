#include <QtGui/QApplication>
#include "fusic.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    fusic foo;
    foo.show();
    return app.exec();
}
