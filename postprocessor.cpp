#include "postprocessor.h"
#include "ui_postprocessor.h"

Postprocessor::Postprocessor(QWidget *parent, QTableWidget* barsT, QTableWidget* nodesT,
                             QCheckBox* leftT, QCheckBox* rightT) :
    QWidget(parent),
    ui(new Ui::Postprocessor)
{
    bars = barsT;
    nodes = nodesT;
    left = leftT;
    right = rightT;


    ui->setupUi(this);
    makeTable();
}

Postprocessor::~Postprocessor()
{
    delete ui;
}

void Postprocessor::makeTable()
{
    std::vector<std::vector<double> > matrixA = makeAMatrix(bars, nodes, left, right);
    std::vector<double> vectorB = makeBVector(bars, nodes, left, right);
    std::vector<double> delta = solveSystem(matrixA, vectorB);

    unsigned int l;
    double q;
    double e;
    double a;
    double up0;
    double upl;
    //double temp;
    double up;
    double n;
    double sigma;
    double maxSigma;
    unsigned int coordBeg;
    //unsigned int coordEnd;

    for(unsigned int i = 0; i < bars->rowCount(); i++)
    {
        l = getLength(i+1, bars, nodes);
        q = bars->item(i, 5)->text().toDouble();
        e = bars->item(i, 3)->text().toDouble();
        a = bars->item(i, 2)->text().toDouble();
        up0 = delta.at(i);
        upl = delta.at(i+1);
        maxSigma = bars->item(i, 4)->text().toDouble();
        coordBeg = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toUInt();
        //coordEnd = nodes->item(bars->item(i, 1)->text().toInt()-1, 0)->text().toUInt();

        for(double x = 0; x < l; x+=( (double)l )/10)
        {
            ui->dataTable->insertRow(ui->dataTable->rowCount());

            QTableWidgetItem* itemCoord = new QTableWidgetItem;
            ui->dataTable->setItem(ui->dataTable->rowCount()-1, 0, itemCoord);
            QTableWidgetItem* itemNp = new QTableWidgetItem;
            ui->dataTable->setItem(ui->dataTable->rowCount()-1, 1, itemNp);
            QTableWidgetItem* itemUx = new QTableWidgetItem;
            ui->dataTable->setItem(ui->dataTable->rowCount()-1, 2, itemUx);
            QTableWidgetItem* itemSigma = new QTableWidgetItem;
            ui->dataTable->setItem(ui->dataTable->rowCount()-1, 3, itemSigma);
            QTableWidgetItem* itemMaxSigma = new QTableWidgetItem;
            ui->dataTable->setItem(ui->dataTable->rowCount()-1, 4, itemMaxSigma);

            ui->dataTable->item(ui->dataTable->rowCount()-1, 0)->setText(QString::number(coordBeg+x));

            up = upx(x, l, up0, upl, q, e, a);
            n = npx(x, e, a, l, up0, upl, q);
            sigma = sigmax(n, a);

            ui->dataTable->item(ui->dataTable->rowCount()-1, 1)->setText(QString::number(n));
            ui->dataTable->item(ui->dataTable->rowCount()-1, 2)->setText(QString::number(up));
            ui->dataTable->item(ui->dataTable->rowCount()-1, 3)->setText(QString::number(sigma));
            ui->dataTable->item(ui->dataTable->rowCount()-1, 4)->setText(QString::number(maxSigma));
        }
    }
}
