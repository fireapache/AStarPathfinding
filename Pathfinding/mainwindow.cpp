#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //scene = new QGraphicsScene(QRect(0, 0, 300, 300));
    //frame = scene->addRect(0, 0, 100, 100);

    mapScene = new MapScene(ui->viewport);
    ui->viewport->setScene(mapScene);

    connect(ui->btnBuildMap, SIGNAL(clicked()), this, SLOT(buildMap()));
    connect(ui->spinMapWidth, SIGNAL(valueChanged(int)), this, SLOT(prepareToBuildMap()));
    connect(ui->spinMapHeight, SIGNAL(valueChanged(int)), this, SLOT(prepareToBuildMap()));
    connect(ui->btnShowPath, SIGNAL(clicked()), this, SLOT(buildPath()));
    connect(ui->btnShowStep, SIGNAL(clicked()), this, SLOT(showNextStep()));
    connect(ui->spinMapScale, SIGNAL(valueChanged(int)), this, SLOT(prepareToBuildMap()));
    connect(ui->btnWall, SIGNAL(clicked()), this, SLOT(toSetWall()));
    connect(ui->btnWay, SIGNAL(clicked()), this, SLOT(toSetWay()));
    connect(ui->btnDirt, SIGNAL(clicked()), this, SLOT(toSetDirt()));
    connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(toSetStart()));
    connect(ui->btnEnd, SIGNAL(clicked()), this, SLOT(toSetEnd()));
    connect(ui->btnShowSteps, SIGNAL(clicked()), this, SLOT(showSteps()));
    connect(ui->radioBtnEuclidean, SIGNAL(toggled(bool)), this, SLOT(toEuclidean()));
    connect(ui->radioBtnManhattan, SIGNAL(toggled(bool)), this, SLOT(toManhattan()));
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete scene;
    //delete frame;
    delete mapScene;
}

void MainWindow::buildMap()
{
    UINT width, height, scale;

    width = (UINT)ui->spinMapWidth->value();
    height = (UINT)ui->spinMapHeight->value();
    scale = (UINT)ui->spinMapScale->value();

    mapScene->setScale(scale);
    mapScene->setMap(width, height);

    ui->btnBuildMap->setText("Rebuild Map");
    ui->btnShowPath->setEnabled(true);
    ui->btnShowStep->setEnabled(true);
    //ui->btnShowSteps->setEnabled(true);
}

void MainWindow::prepareToBuildMap()
{
    ui->btnBuildMap->setText("Build Map");
    ui->btnShowPath->setEnabled(false);
    ui->btnShowStep->setEnabled(false);
    //ui->btnShowSteps->setEnabled(false);
}

void MainWindow::buildPath()
{
    mapScene->showPath();
}

void MainWindow::showNextStep()
{
    mapScene->showNextStep();
}

void MainWindow::showSteps()
{
    //mapScene->showSteps();
}

void MainWindow::toEuclidean()
{
    ui->radioBtnManhattan->setChecked(false);
    mapScene->setHeuristics(euclidean);
}

void MainWindow::toManhattan()
{
    ui->radioBtnEuclidean->setChecked(false);
    mapScene->setHeuristics(manhattan);
}

void MainWindow::toSetWall()
{
    mapScene->setToSetState(TOSETWALL);
}

void MainWindow::toSetWay()
{
    mapScene->setToSetState(TOSETWAY);
}

void MainWindow::toSetDirt()
{
    mapScene->setToSetState(TOSETDIRT);
}

void MainWindow::toSetStart()
{
    mapScene->setToSetState(TOSETSTART);
}

void MainWindow::toSetEnd()
{
    mapScene->setToSetState(TOSETEND);
}
