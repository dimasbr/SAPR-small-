#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "draw.h"

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

    void on_addNode_clicked();

    void on_deleteNode_clicked();

    void on_addBar_clicked();

    void on_deleteBar_clicked();

    void on_nodesTable_cellChanged(int row, int column);

    void on_barsTable_cellChanged(int row, int column);

    void swapNodes(int first, int second);

    void sortNodes();

    bool check();

//    void redraw();
private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
};

#endif // MAINWINDOW_H
