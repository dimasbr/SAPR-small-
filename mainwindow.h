#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include "draw.h"
#include "saveload.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void sortBars();

    void sortNodes();

    void swapBars(int first, int right);

    void swapNodes(int first, int second);

    bool check();

    void on_addNode_clicked();

    void on_deleteNode_clicked();

    void on_addBar_clicked();

    void on_deleteBar_clicked();

    void on_nodesTable_cellChanged(int row, int column);

    void on_barsTable_cellChanged(int row, int column);

//    void redraw();
    void on_verticalSlider_valueChanged(int value);

    void on_rightFix_stateChanged(int arg1);

    void on_leftFix_stateChanged(int arg1);

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
};

#endif // MAINWINDOW_H
