#include "fenguest.h"
#include "fenGuestW.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fenGuestW fen;
    fen.show();

    return a.exec();
}
