#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->planetGLWidget->mw = this;

    selectedTh = false;

    pOriginalButton = ui->Corpc->palette();
    pOriginalTextEdit = ui->Xedit->palette();

    on_Corpsim_clicked();
    selectedRk = false;
    ui->radioButtonEuler->setChecked(true);

    ui->stop->setChecked(true);

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

/*
void MainWindow::on_pushButtonEuler_clicked()
{
    ui->planetGLWidget->createBody = true;
    ui->planetGLWidget->createBodyType = BodyTypeEuler;
}
*/

/*
void MainWindow::on_pushButtonRk_clicked()
{
    ui->planetGLWidget->createBody = true;
    ui->planetGLWidget->createBodyType = BodyTypeRk;
}
*/

void MainWindow::on_stop_stateChanged(int arg1)
{
    ui->planetGLWidget->stopped = arg1;
}

void MainWindow::putData(double x, double y, double vx, double vy, double ax, double ay)
{
    char buf[50];

    sprintf(buf, "x: %.4f km", (x == 0) ? 0 : x/1e3);
    ui->xLabel->setText(QString(buf));
    sprintf(buf, "y: %.4f km", (y == 0) ? 0 : y/1e3);
    ui->yLabel->setText(QString(buf));
    sprintf(buf, "vx: %.4f m/s", vx);
    ui->vxLabel->setText(QString(buf));
    sprintf(buf, "vy: %.4f m/s", vy);
    ui->vyLabel->setText(QString(buf));
    sprintf(buf, "ax: %.4f m/s^2", ax);
    ui->axLabel->setText(QString(buf));
    sprintf(buf, "ay: %.4f m/s^2", ay);
    ui->ayLabel->setText(QString(buf));
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    std::cerr << "Pos of slider: "<<position;
    M = 5.9722e24 * (1+ position / 10.0);
}

void MainWindow::on_ClearButton_clicked()
{
    ui->planetGLWidget->clear();
    ui->xLabel->clear();
    ui->yLabel->clear();
    ui->vxLabel->clear();
    ui->vyLabel->clear();
    ui->axLabel->clear();
    ui->ayLabel->clear();

    //ui->xLabel()
}

/*
void MainWindow::on_ThButton_clicked()
{
    ui->planetGLWidget->clear();
    double x = ui->Xedit->toPlainText().toDouble() * 1000;
    double y = ui->Yedit->toPlainText().toDouble() * 1000;
    double len = ui->LengthEdit->toPlainText().toDouble() * 1000;
    double wcm = ui->WCMedit->toPlainText().toDouble();
    double wp = ui->WPedit->toPlainText().toDouble();
    double k = ui->Kedit->toPlainText().toDouble();
    double c = ui->Cedit->toPlainText().toDouble();
    ui->planetGLWidget->createThBodies(x,y,len,wcm,wp,k,c);

}
*/

void MainWindow::angle(double angle)
{
    ui->unghi->setText(QString::number(angle));
}

void MainWindow::grayOut(QTextEdit *textEdit)
{
    textEdit->clear();
    textEdit->setReadOnly(true);
    QPalette palette = textEdit->palette();
    //QColor alor = QColorDialog::getColor(Qt::white,this); // in here your color pallete will open..

    QPalette p = textEdit->palette(); // define pallete for textEdit..
    p.setColor(QPalette::Base, Qt::gray); // set color "Red" for textedit base
    textEdit->setPalette(p); // change textedit palette
}

void MainWindow::unGrayOut(QTextEdit *textEdit)
{
    textEdit->setReadOnly(false);
    textEdit->setPalette(pOriginalTextEdit);
}

void MainWindow::blueOut(QPushButton *pushButton)
{
    QPalette p = pushButton->palette();
    p.setColor(QPalette::Button, Qt::blue);
    pushButton->setAutoFillBackground(true);
    pushButton->setPalette(p);
}

void MainWindow::unBlueOut(QPushButton *pushButton)
{
    pushButton->setPalette(pOriginalButton);
}

void MainWindow::on_Corpsim_clicked()
{
    on_ClearButton_clicked();

    blueOut(ui->Corpsim);
    unBlueOut(ui->Corpc);

    grayOut(ui->LengthEdit);
    grayOut(ui->Kedit);
    grayOut(ui->Cedit);
    grayOut(ui->WCMedit);
    grayOut(ui->WPedit);
    unGrayOut(ui->VXedit);
    unGrayOut(ui->VYedit);

    ui->Xedit->clear();
    ui->Yedit->clear();

    ui->radioButtonEuler->setEnabled(true);
    ui->radioButtonRk->setEnabled(true);

    selectedTh = false;
}

void MainWindow::on_Corpc_clicked()
{
    on_ClearButton_clicked();

    blueOut(ui->Corpc);
    unBlueOut(ui->Corpsim);

    grayOut(ui->VXedit);
    grayOut(ui->VYedit);
    unGrayOut(ui->LengthEdit);
    unGrayOut(ui->Kedit);
    unGrayOut(ui->Cedit);
    unGrayOut(ui->WCMedit);
    unGrayOut(ui->WPedit);

    ui->Xedit->clear();
    ui->Yedit->clear();

    ui->radioButtonEuler->setEnabled(false);
    ui->radioButtonRk->setEnabled(false);

    selectedTh = true;
}

void MainWindow::on_radioButtonEuler_clicked()
{
    selectedRk = false;
    ui->radioButtonRk->setChecked(false);
}

void MainWindow::on_radioButtonRk_clicked()
{
    selectedRk = true;
    ui->radioButtonEuler->setChecked(false);
}

void MainWindow::on_CreateButton_clicked()
{
    if (selectedTh)
    {
        ui->planetGLWidget->clear();
        double x = ui->Xedit->toPlainText().toDouble() * 1000;
        double y = ui->Yedit->toPlainText().toDouble() * 1000;
        double len = ui->LengthEdit->toPlainText().toDouble() * 1000;
        double wcm = ui->WCMedit->toPlainText().toDouble();
        double wp = ui->WPedit->toPlainText().toDouble();
        double k = ui->Kedit->toPlainText().toDouble();
        printf("k din create = %f",k);
        double c = ui->Cedit->toPlainText().toDouble();
        ui->planetGLWidget->createThBodies(x,y,len,wcm,wp,k,c);
    }
    else {
        double x = ui->Xedit->toPlainText().toDouble() * 1000;
        double y = ui->Yedit->toPlainText().toDouble() * 1000;
        double vx = ui->VXedit->toPlainText().toDouble() * 1000;
        double vy = ui->VYedit->toPlainText().toDouble() * 1000;
        ui->planetGLWidget->createSimBody(x, y, vx, vy, selectedRk);
    }
}
