#include <QApplication>
#include "world.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    World world;
    world.resize(700, 700);
    world.move(0, 0);
    world.setWindowTitle("Lab3");
    world.show();
    return app.exec();
}
