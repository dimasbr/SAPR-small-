#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QTableWidget>
#include "processor.h"
#include <QCheckBox>
#include <vector>

namespace Ui {
class Postprocessor;
}

class Postprocessor : public QWidget
{
    Q_OBJECT

public:
    explicit Postprocessor(QWidget *parent, QTableWidget* barsT, QTableWidget* nodesT,
                           QCheckBox* leftT, QCheckBox* rightT);
    ~Postprocessor();

    QTableWidget* bars;
    QTableWidget* nodes;
    QCheckBox* left;
    QCheckBox* right;

private slots:
    void makeTable();
    void drawEpures();

    void on_addScale_clicked();

    void on_removeScale_clicked();

    void on_closeButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

private:
    Ui::Postprocessor *ui;
    QGraphicsScene scene;
    int scale;
};

#endif // POSTPROCESSOR_H
