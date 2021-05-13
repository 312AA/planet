#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QColorDialog>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool selectedTh;
    bool selectedRk;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updates();

    void putData(double x, double y, double vx, double vy, double ax, double ay);
    void angle(double angle);

private slots:
    void on_stop_stateChanged(int arg1);

    void on_horizontalSlider_sliderMoved(int position);

    void on_ClearButton_clicked();

    void on_Corpsim_clicked();

    void on_Corpc_clicked();

    void on_radioButtonEuler_clicked();

    void on_radioButtonRk_clicked();

    void on_CreateButton_clicked();

private:
    Ui::MainWindow *ui;

    QPalette pOriginalTextEdit;
    QPalette pOriginalButton;

    void grayOut(QTextEdit *textEdit);
    void unGrayOut(QTextEdit *textEdit);
    void blueOut(QPushButton *pushButton);
    void unBlueOut(QPushButton *pushButton);
};
#endif // MAINWINDOW_H
