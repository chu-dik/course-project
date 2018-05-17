#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "render.h"

#include <QLabel>
#include <math.h>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(nullptr),
    QTimage(nullptr)
{
    ui->setupUi(this);

    QRectF scene_rect = GetViewRect();

    scene = new QGraphicsScene(scene_rect); //canvas
    QTimage = new QImage(scene_rect.width(), scene_rect.height(), QImage::Format_RGB32);

    ui->graphicsView->setScene(scene);

    QRectF scene_rect2 = GetViewRect2();

    scene2 = new QGraphicsScene(scene_rect2); //color pre view
    QTimage2 = new QImage(scene_rect2.width(), scene_rect2.height(), QImage::Format_RGB32);

    ui->graphicsView_2->setScene(scene2);

    lights.push_back(Light(Vec3(0),Vec3(5)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QRectF MainWindow::GetViewRect()
{
    QGraphicsView *view = ui->graphicsView;

    int width = view->width() - 3;
    int height = view->height() - 3;

    return QRect(0, 0, width, height);
}

QRectF MainWindow::GetViewRect2()
{
    QGraphicsView *view = ui->graphicsView_2;

    int width = view->width() - 3;
    int height = view->height() - 3;

    return QRect(0, 0, width, height);
}

void MainWindow::UpdateImage2()
{
    scene2->addPixmap(QPixmap::fromImage(*QTimage2));
}

void MainWindow::UpdateImage()
{
    scene->addPixmap(QPixmap::fromImage(*QTimage));
}

void MainWindow::on_pushButton_clicked()
{
//         ...
    //unsigned int start_time =  clock();

    add_light();

    render();
    UpdateImage();
    objects.pop_back();

    //unsigned int end_time = clock();
    //unsigned int search_time = end_time - start_time;

    //printf("%d  %d\n", search_time,CLOCKS_PER_SEC);
}

struct Camera {
    Vec3 pos;
    Vec3 lookAt;
    Vec3 up;
    double fov;
    int width;
    int height;
};

Camera camera;

Vec3 MainWindow::sendRay(Vec3 rayOrigin, Vec3 rayDirection, int level, vector<Object*> objects, vector<Light> lights){
    double intersectPoint = INFINITY;
    bool doesIntersect = false;
    Object* intersectObject = NULL;
    /*checking for nearest intersection*/
    for(vector<Object*>::iterator it = objects.begin() ; it != objects.end(); ++it) {
        double point = (*it)->intersectionPoints(rayOrigin, rayDirection);
        if(point<0) continue;
        else {
            doesIntersect = true;
            if(intersectPoint > point){
                intersectPoint = point;
                intersectObject = (*it);
            }
        }
    }
    if(!doesIntersect) return Vec3(0.9);


    Vec3 point = rayOrigin + rayDirection * intersectPoint;
    Vec3 normal = intersectObject->getNormal(point);
    Vec3 finalColor(0);

    if(intersectObject->objectType == LIGHT)
        return Vec3(255.0,255.0,0.0);

    for(vector<Light>::iterator light = lights.begin() ; light != lights.end(); ++light) {
        Vec3 pos = light->position;
        Vec3 lightDir = point - pos;
        lightDir.normalize();

        bool shadow = false;
        intersectPoint = INFINITY;

        for(vector<Object*>::iterator it = objects.begin() ; it != objects.end(); ++it){
            double point = (*it)->intersectionPoints(pos, lightDir);
            if(point<0) continue;
            else {
                if((*it)->objectType != LIGHT){
                    shadow = true;
                    if(intersectPoint > point){
                        intersectPoint = point;
                        intersectObject = (*it);
                    }
                }
            }
        }
        if(point == (pos + lightDir * intersectPoint)) shadow = false;

        if(!shadow) {
            lightDir = lightDir * -1;
            double colorFactor = normal.dot(lightDir);
            if(colorFactor > 0)
                finalColor += intersectObject->surfaceColor * light->colors * colorFactor * intersectObject->phongCoeffs.x * sqrt((3/(point - pos).length2()));//pow((1/(point - pos).length2()),3);
        }
    }

    return finalColor;
}

void MainWindow::init(vector<Object*> objects, vector<Light> lights){

/*
CAMERA
        POS 0 0 0
        LOOKAT 0 0 -1
        UP 0 1 0
        DIMENSION 800 600
        FOV 50
        TOTAL_SAMPLES 1
        RECURSION_DEPTH 5
    camera.lookAt = camera.pos;
    camera.lookAt.z = camera.lookAt.z -1;
*/
    camera.pos = Vec3(0,0,0);
    camera.lookAt = Vec3(0,0,-1);
    camera.up = Vec3(0,1,0);
    camera.fov = 50;
    camera.height = scene->height();
    camera.width = scene->width();
    Vec3 n = (camera.pos - camera.lookAt).normalize();
    Vec3 u = camera.up.cross(n).normalize();
    Vec3 v = n.cross(u);

    Vec3 e;
    e.x = u.dot(camera.pos)*-1;
    e.y = v.dot(camera.pos)*-1;
    e.z = n.dot(camera.pos)*-1;

    double V[16] = {u.x, u.y, u.z, e.x, v.x, v.y, v.z, e.y, n.x, n.y, n.z, e.z, 0, 0, 0, 1};
    Matrix viewingTransform(V);


    double rad = 57.1744;
    double an = ui->spinBox_aX->value()/rad;
    Matrix temp(ROTATEX, an);
    viewingTransform =  temp * viewingTransform;

    an = ui->spinBox_aY->value()/rad;
    Matrix temp1(ROTATEY, an);
    viewingTransform =  temp1 * viewingTransform;

    an = ui->spinBox_aZ->value()/rad;
    Matrix temp2(ROTATEZ, an);
    viewingTransform =  temp2 * viewingTransform;

    double aspectRatio = camera.width/(double) camera.height;
    double angle = tan((M_PI/2)*camera.fov/180);

    //printf("%f", angle);
    vector<vector<Vec3> > image(camera.width, vector<Vec3>(camera.height, Vec3()));
    for(int j=0;j<camera.height;++j){
        for(int i=0;i<camera.width;++i){
            image[i][j] = Vec3(0);
            //for(int sample = 0; sample<camera.total_samples; sample++){


                double x = (2 * ((i + 0.5) / camera.width) - 1) * angle * aspectRatio;
                double y = (1 - 2 * ((j + 0.5) / camera.height)) * angle;
                //printf("%f %f\n", xx, yy);
                Vec3 rayDirection(x, y, -1);

                /*ray transformation to world coordinate system*/
                rayDirection = viewingTransform.transform(rayDirection, 0);
                rayDirection.normalize();

                /*starting of ray tracer*/
                image[i][j]+= sendRay(Vec3(0), rayDirection, 0, objects, lights);
            //}
            //image[i][j] = image[i][j] * (1/(double)camera.total_samples);
        }
    }

    double V1[16] = {ui->spinBox_L1->value()-ui->spinBox_cX->value(), 0, 0, 0, 0, ui->spinBox_L2->value()-ui->spinBox_cY->value(), 0, 0, 0, 0, ui->spinBox_L3->value()-ui->spinBox_cZ->value(), 0, 0, 0, 0, 1};

    Matrix lightPoint(V1);

    //lightPoint = viewingTransform*lightPoint;
    //lightPoint.print();

    for(int j=0;j<camera.height;j++)
        for(int i=0;i<camera.width;i++)
            QTimage->setPixel(i,j,qRgb(int(min(1., image[i][j].x)*255),int(min(1., image[i][j].y)*255),int(min(1., image[i][j].z)*255)));

}

void MainWindow::render(){

    stack <Matrix> transform_stack;
    Matrix current_transform;


    //vector<Object*> objects;
    //vector<Light> lights;

    lights.clear();
    lights.push_back(Light(Vec3(ui->spinBox_L1->value()-ui->spinBox_cX->value(),
                                ui->spinBox_L2->value()-ui->spinBox_cY->value(),
                                ui->spinBox_L3->value()-ui->spinBox_cZ->value()),Vec3(1)));

    for(vector<Object*>::iterator it = objects.begin() ; it != objects.end(); ++it){
        Matrix current_transform = Matrix();
        Vec3 val(-ui->spinBox_cX->value(),-ui->spinBox_cY->value(),-ui->spinBox_cZ->value());
        Matrix temp(TRANSLATE, val);
        (*it)->transformMatrix = (*it)->transformMatrix * (*it)->inverseMatrix;

        (*it)->transformMatrix = (*it)->transformMatrix * temp;
        (*it)->inverseMatrix = (*it)->transformMatrix.inverse();
        (*it)->transposeMatrix = (*it)->transformMatrix.transpose();
     }

    //lights.pop_back();
   //lights.push_back(Light(Vec3(ui->spinBox_L1->value(), ui->spinBox_L2->value(), ui->spinBox_L3->value(),Vec3(5)));
    init(objects, lights);
}

void MainWindow::add_light(){
    Matrix current_transform = Matrix();

    Vec3 center, color, phong;
    double radius = 3;  //= ui->spinBox_C1_6->value();
    center.x = ui->spinBox_L1->value()- ui->spinBox_aX->value() - 50;
    center.y = ui->spinBox_L2->value()- ui->spinBox_aX->value() - 50;
    center.z = ui->spinBox_L3->value()- ui->spinBox_aX->value() - 50;

    color.x = 255.0;
    color.y = 255.0;
    color.z = 0;

    phong.x = phong.y = phong.z = 0;
    Sphere* tmp = new Sphere(center,radius,color,0,LIGHT,current_transform,phong);
    objects.push_back(tmp);

//printf("cent:%f     rad:%f\n", tmp->center, tmp->radius);
//tmp->transformMatrix.print();
}

void MainWindow::add_triangle()
{
    Matrix current_transform = Matrix();

    Vec3 p1,p2,p3;
    Vec3 color;

    color.x = ui->Rval->value()/255.0;
    color.y = ui->Gval->value()/255.0;
    color.z = ui->Bval->value()/255.0;

    p1.x = ui->spinBox_C1_3->value();
    p1.y = ui->spinBox_C2_3->value();
    p1.z = ui->spinBox_C3_3->value();

    p2.x = ui->spinBox_C1_4->value();
    p2.y = ui->spinBox_C2_4->value();
    p2.z = ui->spinBox_C3_4->value();

    p3.x = ui->spinBox_C1_6->value();
    p3.y = ui->spinBox_C2_6->value();
    p3.z = ui->spinBox_C3_6->value();

    Triangle* temp = new Triangle(p1,p2,p3,color,0,DIFFUSED,current_transform,Vec3(0.5));
    objects.push_back(temp);
}

void MainWindow::add_sphere(){

    Matrix current_transform = Matrix();

    Vec3 center, color, phong;
    double radius = ui->spinBox_C1_6->value();
    center.x = ui->spinBox_C1_3->value();
    center.y = ui->spinBox_C2_3->value();
    center.z = ui->spinBox_C3_3->value();

    color.x = ui->Rval->value()/255.0;
    color.y = ui->Gval->value()/255.0;
    color.z = ui->Bval->value()/255.0;

    phong.x = phong.y = phong.z = 0.5;
    Sphere* tmp = new Sphere(center,radius,color,0,DIFFUSED,current_transform,phong);
    objects.push_back(tmp);

    //printf("cent:%f     rad:%f\n", tmp->center, tmp->radius);
    //tmp->transformMatrix.print();
}

void MainWindow::add_cylinder()
{

    Matrix current_transform = Matrix();

    Vec3 center,color;

    double height = ui->spinBox_C2_6->value();
    double radius = ui->spinBox_C1_6->value();

    center.x = ui->spinBox_C1_3->value();
    center.y = ui->spinBox_C2_3->value();
    center.z = ui->spinBox_C3_3->value();

    color.x = ui->Rval->value()/255.0;
    color.y = ui->Gval->value()/255.0;
    color.z = ui->Bval->value()/255.0;

    //ifs>>center.x>>center.y>>center.z>>upVector.x>>upVector.y>>upVector.z>>radius>>height>>color.x>>color.y>>color.z>>transparency>>type>>phong.x>>phong.y>>phong.z;
    Cylinder* temp = new Cylinder(center,Vec3(0,1,0),radius,height,color,0,DIFFUSED,current_transform,Vec3(0.5));
    objects.push_back(temp);
}

void MainWindow::add_cone()
{
    Matrix current_transform = Matrix();

    Vec3 center, color;
    double height = ui->spinBox_C1_6->value();
    double alpha = ui->spinBox_C2_6->value();

    center.x = ui->spinBox_C1_3->value();
    center.y = ui->spinBox_C2_3->value();
    center.z = ui->spinBox_C3_3->value();

    color.x = ui->Rval->value()/255.0;
    color.y = ui->Gval->value()/255.0;
    color.z = ui->Bval->value()/255.0;

    Cone* temp = new Cone(center,Vec3(0,1,0),alpha/57.0,height,color,0,DIFFUSED,current_transform,Vec3(0.5));
    objects.push_back(temp);
}

void MainWindow::on_pushButton_add_clicked()
{
    switch(figure){
    case 1:
        strlist->append("Triangle");
        add_triangle();
        break;
    case 2:
        strlist->append("Sphere");
        add_sphere();
        break;
    case 3:
        strlist->append("Cylinder");
        add_cylinder();
        break;
    case 4:
        strlist->append("Cone");
        add_cone();
        break;
    }

    model->setStringList(*strlist);
    ui->listView->setModel(model);
}

//Change buttons in case of choosed figure
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    figure = index;
    if(figure == 1){
        ui->param1->setText("Точка 1:");
        ui->param2->setText("Точка 2:");
        ui->param3->setText("Точка 3:");

        ui->label_p12->setText("X:");
        ui->label_p22->setText("Y:");
        ui->label_p32->setText("Z:");
        ui->label_p13->setText("X:");
        ui->label_p23->setText("Y:");
        ui->label_p33->setText("Z:");
    }
    else if(figure == 3){
        ui->param1->setText("Центр:");
        ui->param2->setText("");
        ui->param3->setText("Размер");
        ui->label_p12->setText("");
        ui->label_p22->setText("");
        ui->label_p32->setText("");
        ui->label_p13->setText("r:");
        ui->label_p23->setText("h:");
        ui->label_p33->setText("");
    }
    else if(figure == 4){
        ui->param1->setText("Центр:");
        ui->param2->setText("");
        ui->param3->setText("Размер:");
        ui->label_p12->setText("");
        ui->label_p22->setText("");
        ui->label_p32->setText("");
        ui->label_p13->setText("h:");
        ui->label_p23->setText("alph:");
        ui->label_p33->setText("");
    }
    else if(figure == 2){
        ui->param1->setText("Центр:");
        ui->param2->setText("");
        ui->param3->setText("Размер");

        ui->label_p12->setText("");
        ui->label_p22->setText("");
        ui->label_p32->setText("");
        ui->label_p13->setText("r:");
        ui->label_p23->setText("");
        ui->label_p33->setText("");
    }
}

//COLOUR EXAMPLE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MainWindow::on_Rval_valueChanged(int arg1)
{
    for (int x = 0; x < QTimage2->width(); x++)
            for (int y = 0; y < QTimage2->height(); y++)
                QTimage2->setPixel(x, y,qRgb(arg1,ui->Gval->value(),ui->Bval->value()));
    UpdateImage2();
}

void MainWindow::on_Gval_valueChanged(int arg1)
{
    for (int x = 0; x < QTimage2->width(); x++)
            for (int y = 0; y < QTimage2->height(); y++)
                QTimage2->setPixel(x, y,qRgb(ui->Rval->value(),arg1,ui->Bval->value()));
    UpdateImage2();
}

void MainWindow::on_Bval_valueChanged(int arg1)
{
    for (int x = 0; x < QTimage2->width(); x++)
            for (int y = 0; y < QTimage2->height(); y++)
                QTimage2->setPixel(x, y,qRgb(ui->Rval->value(),ui->Gval->value(),arg1));
    UpdateImage2();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Get index of pressed figure
void MainWindow::on_listView_pressed(const QModelIndex &index)
{
    list_index = index.row();
}

void MainWindow::on_pushButton_del_clicked()
{
    objects.erase(objects.begin() + list_index);

    strlist->removeAt(list_index);
    model->setStringList(*strlist);
    ui->listView->setModel(model);
}
