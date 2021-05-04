#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updates();

    void putData(double x, double y, double vx, double vy, double ax, double ay);

private slots:
    void on_pushButtonEuler_clicked();


    void on_stop_stateChanged(int arg1);

    void on_pushButtonRk_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
