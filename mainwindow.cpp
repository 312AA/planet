#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::updates));
    timer->start(30);
}

void MainWindow::updates()
{
    ui->planetGLWidget->repaint();
}


MainWindow::~MainWindow()
{
    delete ui;
}

