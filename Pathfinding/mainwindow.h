#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>
#include "mapscene.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void buildMap();
    void prepareToBuildMap();
    void buildPath();
    void showNextStep();
    void showSteps();
    void toEuclidean();
    void toManhattan();
    void toSetWall();
    void toSetWay();
    void toSetDirt();
    void toSetStart();
    void toSetEnd();

private:
    Ui::MainWindow *ui;

    //QGraphicsScene *scene;
    //QGraphicsRectItem *frame;

    MapScene *mapScene;
};

#endif // MAINWINDOW_H
