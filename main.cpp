#include "mainwindow.h"
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}




/*#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsScene>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    QGraphicsScene scene;
    QPixmap image;
    image.load("test.jpg");
    scene.addPixmap(image);
    scene.setSceneRect(image.rect());
    QGraphicsView view(&scene);
    view.show();

    return a.exec();
}*/


/*
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QImage myImage;
    myImage.load("D:\\Study\\STUFF\\Kursach\\anime.png");

    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(myImage));

    myLabel.show();

    return a.exec();
}*/
