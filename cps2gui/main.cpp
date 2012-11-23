#include <QtGui/QApplication>
#include "cps2emu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CPS2emu w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif

    return a.exec();
}
