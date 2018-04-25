#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "render.h"
#include <QStringListModel>


#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include "vec3.h"
#include "object.h"
#include "light.h"
#include "matrix.h"
#include <algorithm>
#include <cstdlib>
#include <stack>
#include "mainwindow.h"
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //place for my functions

private slots:

    void on_pushButton_clicked(); //render

    void on_pushButton_add_clicked(); //add figure

    void on_comboBox_currentIndexChanged(int index); //changed figure's num

    void on_Rval_valueChanged(int arg1);

    void on_Gval_valueChanged(int arg1);

    void on_Bval_valueChanged(int arg1);

    void on_listView_pressed(const QModelIndex &index);

    //void on_listView_clicked(const QModelIndex &index);

    //void on_listView_activated(const QModelIndex &index);
/*

    void on_pushButton_2_clicked();



    void on_listView_pressed(const QModelIndex &index);

    void on_pushButton_3_clicked();
*/

    void on_pushButton_del_clicked();

protected:
    QRectF GetViewRect();
    void UpdateImage();

    QRectF GetViewRect2();
    void UpdateImage2();

    void add_triangle();
    void add_sphere();
    void add_cylinder();
    void add_cone();
    void add_light();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage *QTimage;

    QGraphicsScene *scene2;
    QImage *QTimage2;

    vector<Object*> objects;
    //Light* light;
    vector<Light> lights;

    QStringListModel *model= new QStringListModel();
    QStringList *strlist = new QStringList();

    int list_index = -1;
    int figure = 0;

    Vec3 sendRay(Vec3 rayOrigin, Vec3 rayDirection, int level, vector<Object*> objects, vector<Light> lights);
    void init(vector<Object*> objects, vector<Light> lights);
    void render();
/*

    int x;
    int y;
    int list_index = -1;
    int obj_num = 0;

    Camera *cam = new Camera();
    Light *lght = new Light();
    ObjectList *LIST_HEAD = new ObjectList();

    void GetData();

    void add_object(ObjectList** LIST_HEAD, ObjectList *objlist);
    ObjectList* add_plate(Point p1, Point p2, Point p3, QRgb color);
    PolygonList* plate_polygonlist(Point p1, Point p2, Point p3);
    void render(Camera *cam, Light *lght, ObjectList *LIST_HEAD);
    QRgb find_intersection(Camera *cam, Point dir, Light *lght, ObjectList *LIST_HEAD);
    QRgb Obj_intersection(ObjectList* obj, float *distance,
                          Camera *cam, Light *lght,
                          float sina, float cosa, float sinb, float cosb);
    Polygon transformed_polygon(Polygon plgn,Camera *cam,
                                            float sina,float cosa,float sinb,float cosb);

    ObjectList* add_cube(Point center, float size, float ax, float ay, float az, QRgb color);
    PolygonList* cube_polygonlist(Point center, float size, float ax, float ay, float az);
    Light transformed_light(Light lght,Camera *cam,
                                                float sina,float cosa,float sinb,float cosb);
*/
};

#endif // MAINWINDOW_H
