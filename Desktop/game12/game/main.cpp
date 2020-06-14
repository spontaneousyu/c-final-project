#include "mainwindow.h"
#include <QApplication>

int main(int paraa, char *parab[])
{
    QApplication a(paraa, parab);
    MainWindow game;
    game.show();
	return a.exec();
}
