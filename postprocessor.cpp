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
    scale = 10;


    ui->setupUi(this);

    ui->renderArea->setScene(&scene);
    ui->renderArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->renderArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    makeTable();
    drawEpures();

    QTableWidgetItem* tempN = new QTableWidgetItem;
    QTableWidgetItem* tempU = new QTableWidgetItem;
    QTableWidgetItem* tempS = new QTableWidgetItem;
    QTableWidgetItem* tempMS = new QTableWidgetItem;


    ui->pointResults->insertRow(0);
    ui->pointResults->setItem(0, 0, tempN);
    ui->pointResults->setItem(0, 1, tempU);
    ui->pointResults->setItem(0, 2, tempS);
    ui->pointResults->setItem(0, 3, tempMS);
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

    double l;
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
    double coordBeg;
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
        coordBeg = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toDouble();
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

            if(sigma >= maxSigma || sigma <= -maxSigma)
            {
                ui->dataTable->item(ui->dataTable->rowCount()-1, 1)->setBackgroundColor(Qt::red);
                ui->dataTable->item(ui->dataTable->rowCount()-1, 2)->setBackgroundColor(Qt::red);
                ui->dataTable->item(ui->dataTable->rowCount()-1, 3)->setBackgroundColor(Qt::red);
                ui->dataTable->item(ui->dataTable->rowCount()-1, 4)->setBackgroundColor(Qt::red);
            }
        }
    }
}

void Postprocessor::drawEpures()
{
    QGraphicsScene* sceneTemp = ui->renderArea->scene();
    sceneTemp->clear();

    QPen standartPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap);
    QPen nFPen(Qt::blue, 4, Qt::SolidLine, Qt::RoundCap);
    QPen bFPen(Qt::green, 4, Qt::SolidLine, Qt::RoundCap);

    unsigned int wep = 450;

    sceneTemp->addLine(0, 0, wep, 0, standartPen);
    sceneTemp->addLine(wep, 0, wep-10, 10, standartPen);
    sceneTemp->addLine(wep, 0, wep-10, -10, standartPen);

    sceneTemp->addLine(0, 100, wep, 100, standartPen);
    sceneTemp->addLine(wep, 100, wep-10, 110, standartPen);
    sceneTemp->addLine(wep, 100, wep-10, 90, standartPen);

    sceneTemp->addLine(0, 200, wep, 200, standartPen);
    sceneTemp->addLine(wep, 200, wep-10, 210, standartPen);
    sceneTemp->addLine(wep, 200, wep-10, 190, standartPen);

    std::vector<std::vector<double> > matrixA = makeAMatrix(bars, nodes, left, right);
    std::vector<double> vectorB = makeBVector(bars, nodes, left, right);
    std::vector<double> delta = solveSystem(matrixA, vectorB);

    std::vector<std::pair<double, double> > uXCoords = makeCoordsUx(delta, bars, nodes);
    std::vector<std::pair<double, double> > nXCoords = makeCoordsNx(delta, bars, nodes);
    std::vector<std::pair<double, double> > sigmaXCoords = makeCoordsSigmax(nXCoords, bars, nodes);

    unsigned int vSize = uXCoords.size();
    double tempCoord1;
    double tempValue1;
    double tempCoord2;
    double tempValue2;

    double constructionSize = nodes->item(nodes->rowCount()-1, 0)->text().toDouble() -
                                    nodes->item(0, 0)->text().toDouble();

    //if (vSize%2 == 1) vSize--;

    for (unsigned int i = 0; i < vSize-1; i++)
    {
        tempCoord1 = nXCoords.at(i).first * ((double)450)/constructionSize;
        tempValue1 = - nXCoords.at(i).second * scale;
        tempCoord2 = nXCoords.at(i+1).first * ((double)450)/constructionSize;
        tempValue2 = - nXCoords.at(i+1).second * scale;
        sceneTemp->addLine(tempCoord1, tempValue1, tempCoord2, tempValue2, standartPen);
        sceneTemp->addLine(tempCoord1, 0, tempCoord1, tempValue1, standartPen);

        tempCoord1 = uXCoords.at(i).first * ((double)450)/constructionSize;
        tempValue1 = - uXCoords.at(i).second * scale;
        tempCoord2 = uXCoords.at(i+1).first * ((double)450)/constructionSize;
        tempValue2 = - uXCoords.at(i+1).second * scale;
        sceneTemp->addLine(tempCoord1, tempValue1+100, tempCoord2, tempValue2+100, standartPen);
        sceneTemp->addLine(tempCoord1, 100, tempCoord1, tempValue1+100, standartPen);

        tempCoord1 = sigmaXCoords.at(i).first * ((double)450)/constructionSize;
        tempValue1 = - sigmaXCoords.at(i).second * scale;
        tempCoord2 = sigmaXCoords.at(i+1).first * ((double)450)/constructionSize;
        tempValue2 = - sigmaXCoords.at(i+1).second * scale;
        sceneTemp->addLine(tempCoord1, tempValue1+200, tempCoord2, tempValue2+200, standartPen);
        sceneTemp->addLine(tempCoord1, 200, tempCoord1, tempValue1+200, standartPen);
    }
}

void Postprocessor::on_addScale_clicked()
{
    scale++;
    drawEpures();
}

void Postprocessor::on_removeScale_clicked()
{
    scale--;
    drawEpures();
}

void Postprocessor::on_closeButton_clicked()
{
    this->close();
}

void Postprocessor::on_lineEdit_textEdited(const QString &arg1)
{
    std::vector<std::vector<double> > matrixA = makeAMatrix(bars, nodes, left, right);
    std::vector<double> vectorB = makeBVector(bars, nodes, left, right);
    std::vector<double> delta = solveSystem(matrixA, vectorB);

    double l;
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
//    unsigned int coordBeg;
    double searchPoint;

    double tempCoord;
    try
    {
        searchPoint = ui->lineEdit->text().toDouble();
    }
    catch(...)
    {
        ui->lineEdit->setText("0");
    }

    ui->pointResults->item(0, 0)->setBackground(Qt::white);
    ui->pointResults->item(0, 1)->setBackground(Qt::white);
    ui->pointResults->item(0, 2)->setBackground(Qt::white);
    ui->pointResults->item(0, 3)->setBackground(Qt::white);

    unsigned int barTemp;

    if (searchPoint < nodes->item(0, 0)->text().toDouble() ||
            searchPoint > nodes->item(nodes->rowCount()-1, 0)->text().toDouble())
        return;

//    for(unsigned int x = 0; x < nodes->rowCount(); x++)
//    {
//        tempCoord = nodes->item(x, 0)->text().toUInt();
//        if(searchPoint >= tempCoord)
//        {
//            barTemp = getBarBeginsIn(x+1, bars);
//            break;
//        }
//    }

    unsigned int x = 1;
    tempCoord = nodes->item(x, 0)->text().toDouble();

    while(tempCoord<searchPoint)
    {
        x++;
        tempCoord = nodes->item(x, 0)->text().toDouble();
    }
    barTemp = getBarEndsIn(x+1, bars);
    tempCoord = nodes->item(bars->item(barTemp-1, 0)->text().toInt()-1, 0)->text().toDouble();

    l = getLength(barTemp, bars, nodes);
    q = bars->item(barTemp-1, 5)->text().toDouble();
    e = bars->item(barTemp-1, 3)->text().toDouble();
    a = bars->item(barTemp-1, 2)->text().toDouble();
    //tempCoord = nodes->item(bars->item(barTemp-1, 0)->text().toInt()-1, 0)->text().toUInt();

    up0 = delta.at(bars->item(barTemp-1, 0)->text().toUInt()-1);
    upl = delta.at(bars->item(barTemp-1, 1)->text().toUInt()-1);

    n = npx(searchPoint - tempCoord, e, a, l, up0, upl, q);
    up = upx(searchPoint - tempCoord, l, up0, upl, q, e, a);
    sigma = sigmax(n, a);
    maxSigma = bars->item(barTemp-1, 4)->text().toDouble();

    ui->pointResults->item(0, 0)->setText(QString::number(n));
    ui->pointResults->item(0, 1)->setText(QString::number(up));
    ui->pointResults->item(0, 2)->setText(QString::number(sigma));
    ui->pointResults->item(0, 3)->setText(QString::number(maxSigma));

    if(sigma >= maxSigma || sigma <= -maxSigma)
    {
        ui->pointResults->item(0, 0)->setBackground(Qt::red);
        ui->pointResults->item(0, 1)->setBackground(Qt::red);
        ui->pointResults->item(0, 2)->setBackground(Qt::red);
        ui->pointResults->item(0, 3)->setBackground(Qt::red);
    }
}
