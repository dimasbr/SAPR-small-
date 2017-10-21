#include "draw.h"

void draw(QTableWidget* nodes, QTableWidget* bars, QSlider* zoom, QGraphicsView* graphVW)
{
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


    unsigned int difX;
    unsigned int difY;

    if(!nodes->rowCount()) return;
    if(!bars->rowCount()) return;
    difX = nodesCoords[0] - nodesCoords[nodesCoords.size()-1];
    difY = (*(std::max_element(barsAreas.begin(), barsAreas.end()))) / 2;



    unsigned int we = (int)( (float)difX / 0.9 );
    unsigned int he = (int)( (float)difY / 0.9 );

    unsigned int spaceH = (we-difX)/2;
    unsigned int spaceV = (he-difY)/2;

    graphVW->scene()->addLine(0, he/2, we, he/2);
    graphVW->fitInView(QRectF());
}

