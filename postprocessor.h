#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <QWidget>
//#include <QMainWindow>
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

private:
    Ui::Postprocessor *ui;
};

#endif // POSTPROCESSOR_H
