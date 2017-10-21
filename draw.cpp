#include "draw.h"

void draw(QTableWidget* nodes, QTableWidget* bars, QGraphicsView* graphVW, QCheckBox* leftFix, QCheckBox* rightFix)
{
    graphVW->scene()->clear();

    std::vector<unsigned int> nodesCoords;
    std::vector<double> nodesForces;

    std::vector<std::pair<unsigned int, unsigned int> > barsCoords;
    std::vector<double> barsAreas;
    std::vector<double> barsFreq;
    std::vector<double> barsMaxFrc;
    std::vector<double> barsFrc;

    for (int i = 0; i < nodes->rowCount(); i++)
    {
        nodesCoords.push_back(nodes->item(i, 0)->text().toUInt());
        nodesForces.push_back(nodes->item(i, 1)->text().toDouble());
    }

    for (int i = 0; i < bars->rowCount(); i++)
    {
        std::pair<unsigned int, unsigned int> temp(bars->item(i, 0)->text().toUInt(),
                                                   bars->item(i, 1)->text().toUInt());
        barsCoords.push_back(temp);
        barsAreas.push_back(bars->item(i, 2)->text().toDouble());
        barsFreq.push_back(bars->item(i, 3)->text().toDouble());
        barsMaxFrc.push_back(bars->item(i, 4)->text().toDouble());
        barsFrc.push_back(bars->item(i, 5)->text().toDouble());
    }

 //   graphVW->scene()->addLine(graphVW->);

    unsigned int spaceH = 40;
    unsigned int spaceV = 40;

    unsigned int difX;
    unsigned int difY;

    unsigned int wep = graphVW->viewport()->width();
    unsigned int hep = graphVW->viewport()->height();

    QPen standartPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap);
    graphVW->scene()->addLine(0, 0, wep, 0, standartPen);
    graphVW->scene()->addLine(wep, 0, wep-10, 10, standartPen);
    graphVW->scene()->addLine(wep, 0, wep-10, -10, standartPen);

    if(!nodes->rowCount()) return;
    difX = nodesCoords[nodesCoords.size()-1] - nodesCoords[0];

    unsigned int perOneMmH;
    if(difX)
    {
        perOneMmH = (wep - 2*spaceH)/difX;
    }
    else
    {
        perOneMmH = (wep - 2*spaceH)/2;
    }

    std::vector<unsigned int> nodesCoordsOnScreen;
    for(int i=0; i<nodesCoords.size(); i++)
    {
        unsigned int tempCoord = spaceH+nodesCoords.at(i)*perOneMmH;
        graphVW->scene()->addEllipse(tempCoord, -4, 8, 8, standartPen);
        QGraphicsTextItem* tempText = graphVW->scene()->addText(QString::number(i+1));
        tempText->setPos(tempCoord+8, 0);
        graphVW->scene()->addEllipse(tempCoord+8, 5, 15, 15, standartPen);
        nodesCoordsOnScreen.push_back(tempCoord);
    }


    if(!bars->rowCount()) return;
    difY = (*(std::max_element(barsAreas.begin(), barsAreas.end()))) ;

    unsigned int perOneMmV;
    if(difY)
    {
        perOneMmV = (hep - 2*spaceV)/difY;
    }
    else
    {
        perOneMmV = (hep - 2*spaceV)/2;
    }
    for(int i=0; i<bars->rowCount(); i++)
    {
        unsigned int begin = barsCoords.at(i).first;
        unsigned int end = barsCoords.at(i).second;
        unsigned int begCrd = nodesCoordsOnScreen.at(begin-1);
        unsigned int endCrd = nodesCoordsOnScreen.at(end-1);
        graphVW->scene()->addRect(begCrd+4, (barsAreas.at(i)/2)*perOneMmV, endCrd-begCrd,
                                  -(barsAreas.at(i))*perOneMmV, standartPen);
    }



    if(leftFix->isChecked())
    {
        for (int i=0; i < barsAreas.at(0)*perOneMmV-19; i=i+20)
        {
            graphVW->scene()->addLine(nodesCoordsOnScreen.at(0)+2, (-barsAreas.at(0)/2)*perOneMmV+i,
                                      nodesCoordsOnScreen.at(0)-18, (-barsAreas.at(0)/2)*perOneMmV+i+20);
        }
    }

    if(rightFix->isChecked())
    {
        for (int i=0; i < barsAreas.at(barsAreas.size()-1)*perOneMmV-19; i=i+20)
        {
            graphVW->scene()->addLine(nodesCoordsOnScreen.at(barsCoords.at(barsCoords.size()-1).second-1)+2,
                                      (-barsAreas.at(barsAreas.size()-1)/2)*perOneMmV+i,
                                      nodesCoordsOnScreen.at(barsCoords.at(barsCoords.size()-1).second-1)+22,
                                      (-barsAreas.at(barsAreas.size()-1)/2)*perOneMmV+i+20);
        }
    }


    //unsigned int wec = (int)( (float)difX / 0.9 );
    //unsigned int hec = (int)( (float)difY / 0.9 );
//    graphVW->fitInView(QRectF(0-spaceH, he/2, we, -(he/2) ), Qt::KeepAspectRatio);
}

