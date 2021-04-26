#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->planetGLWidget->mw = this;

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


void MainWindow::on_pushButton_clicked()
{
    ui->planetGLWidget->createBody = true;
}

void MainWindow::putData(double x, double y, double vx, double vy, double ax, double ay)
{
    char buf[50];

    /*sprintf(buf, "x: %.4f km", x/1e3);
    ui->xLabel->setText(QString(buf));
    sprintf(buf, "y: %.4f km", y/1e3);
    ui->yLabel->setText(QString(buf));
    sprintf(buf, "vx: %.4f m/s", vx);
    ui->vxLabel->setText(QString(buf));
    sprintf(buf, "vy: %.4f m/s", vy);
    ui->vyLabel->setText(QString(buf));
    sprintf(buf, "ax: %.4f m/s^2", ax);
    ui->axLabel->setText(QString(buf));
    sprintf(buf, "ay: %.4f m/s^2", ay);
    ui->ayLabel->setText(QString(buf));*/
}



void MainWindow::on_stop_stateChanged(int arg1)
{
    ui->planetGLWidget->stopped = arg1;
}
