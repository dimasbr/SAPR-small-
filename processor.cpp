#include "processor.h"

unsigned int getLength(unsigned int bar, QTableWidget* bars, QTableWidget*nodes)
{
    unsigned int begin = bars->item(bar-1, 0)->text().toUInt();
    unsigned int end = bars->item(bar-1, 1)->text().toUInt();

    unsigned int coordBegin = nodes->item(begin-1, 0)->text().toUInt();
    unsigned int coordEnd = nodes->item(end-1, 0)->text().toUInt();

    return coordEnd-coordBegin;
}

unsigned int getBarBeginsIn(unsigned int node, QTableWidget*bars)
{
    for(unsigned int i = 0; i<bars->rowCount(); i++)
    {
        if (bars->item(i, 0)->text().toUInt() == node) return i+1;
    }
}

unsigned int getBarEndsIn(unsigned int node, QTableWidget*bars)
{
    for(unsigned int i = 0; i<bars->rowCount(); i++)
    {
        if (bars->item(i, 1)->text().toUInt() == node) return i+1;
    }
}

std::vector<double> makeBVector(QTableWidget* bars, QTableWidget*nodes, QCheckBox* left, QCheckBox* right)
{
    std::vector<double> res;
    res.resize(nodes->rowCount());

    for(unsigned int i=1; i<nodes->rowCount()-1; i++)
    {
        unsigned int bar0 = getBarEndsIn(i+1, bars);
        unsigned int barL = getBarBeginsIn(i+1, bars);

        double q0 = bars->item(bar0-1, 5)->text().toDouble();
        double qL = bars->item(barL-1, 5)->text().toDouble();

        unsigned int  l0 = getLength(bar0, bars, nodes);
        unsigned int  lL = getLength(barL, bars, nodes);

        double force = nodes->item(i, 1)->text().toDouble();

        res.at(i) = force + (q0/2)*l0 + (qL/2)*lL;
    }

    if(left->isChecked()) res.at(0) = 0;
    else
    {
        double q = bars->item(0, 5)->text().toDouble();
        unsigned int l = getLength(1, bars, nodes);
        double force = nodes->item(0, 1)->text().toDouble();
        res.at(0) = force + (q/2)*l;
    }
    if(right->isChecked()) res.at(res.size()-1) = 0;
    else
    {
        double q = bars->item(bars->rowCount()-1, 5)->text().toDouble();
        unsigned int l = getLength(bars->rowCount(), bars, nodes);
        double force = nodes->item(nodes->rowCount()-1, 1)->text().toDouble();
        res.at(res.size()-1) = force + (q/2)*l;
    }

    return res;
}

std::vector<std::vector<double> > makeAMatrix(QTableWidget* bars, QTableWidget* nodes, QCheckBox* left, QCheckBox* right)
{
    std::vector<std::vector<double> > res (bars->rowCount()+1, std::vector<double>(bars->rowCount()+1, 0));

    for(unsigned int i=0; i<bars->rowCount(); i++)
    {
        double e = bars->item(i, 3)->text().toDouble();
        unsigned int l = getLength(i+1, bars, nodes);
        double a = bars->item(i, 2)->text().toDouble();

        double element = e*a/l;

        res.at(i).at(i) += element;
        res.at(i).at(i+1) -= element;
        res.at(i+1).at(i) -= element;
        res.at(i+1).at(i+1) += element;
    }

    if(left->isChecked())
    {
        res.at(0).at(0) = 1;
        for(unsigned int i = 1; i < res.size(); i++)
        {
            res.at(0).at(i) = 0;
            res.at(i).at(0) = 0;
        }
    }
    if(right->isChecked())
    {
        int k = res.size()-1;
        res.at(k).at(k) = 1;
        for(unsigned int i = 0; i < res.size()-1; i++)
        {
            res.at(k).at(i) = 0;
            res.at(k).at(0) = 0;
        }
    }

    return res;
}

std::vector<double> solveSystem (std::vector<std::vector<double> > a, std::vector<double> b)
{
    for (unsigned int i = 0; i < a.size(); i++)
    {
        double temp = a.at(i).at(i);
        for (unsigned int k = 0; k < a.size(); k++)
        {
            a.at(i).at(k) = a.at(i).at(k)/temp;
        }
        b.at(i) = b.at(i)/temp;

        for (unsigned int k = 0; k < a.size(); k++)
        {
            if(k != i)
            {
                double temp2 = a.at(k).at(i);
                for(unsigned int y = 0; y < a.size(); y++)
                {
                    a.at(k).at(y) = a.at(k).at(y) - a.at(i).at(y)*temp2;
                }
                b.at(k) = b.at(k) - b.at(i)*temp2;
            }
        }
    }

    return b;
}

double upx (double x, unsigned int l, double up0, double upl, double q, double e, double a)
{
    return (1-x/l)*up0 + (x/l)*upl + (q*l*l)/(2*e*a) * x/l * (1-x/l);
}

std::vector<std::pair<double, double> > makeCoordsUx (std::vector<double> delta,
                                                            QTableWidget* bars, QTableWidget*nodes)
{
    std::vector<std::pair<double, double> > res;
    std::pair <double, double> temp;

    unsigned int l;
    double up0;
    double upl;
    double q;
    double e;
    double a;

    for (unsigned int i = 0; i < bars->rowCount(); i++)
    {
        up0 = delta.at(i);
        upl = delta.at(i+1);
        q = bars->item(i, 5)->text().toDouble();
        e = bars->item(i, 3)->text().toDouble();
        a = bars->item(i, 2)->text().toDouble();
        l = getLength(i+1, bars, nodes);

        for(double x = 0; x < l; x+=0.1)
        {
            temp.first = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toDouble() + x;
            temp.second = upx(x, l, up0, upl, q, e, a);
            res.push_back(temp);
        }
        temp.first = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toDouble() + l;
        temp.second = upx(l, l, up0, upl, q, e, a);
        res.push_back(temp);

//        temp.first = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toUInt();
//        temp.second = upx(0, l, up0, upl, q, e, a);
//        res.push_back(temp);

//        temp.first = nodes->item(bars->item(i, 1)->text().toInt()-1, 0)->text().toUInt();
//        temp.second = upx(l, l, up0, upl, q, e, a);
//        res.push_back(temp);
    }

    return res;
}

double npx (double x, double e, double a, unsigned int l, double up0, double upl, double q)
{
    return (e*a)/l * (upl-up0) + (q*l)/2 * (1 - (2*x)/l);
}

std::vector<std::pair<double, double> > makeCoordsNx (std::vector<double> delta,
                                                            QTableWidget* bars, QTableWidget*nodes)
{
    std::vector<std::pair<double, double> > res;
    std::pair <double, double> temp;

    unsigned int l;
    double up0;
    double upl;
    double q;
    double e;
    double a;

    for (unsigned int i = 0; i < bars->rowCount(); i++)
    {
        up0 = delta.at(i);
        upl = delta.at(i+1);
        q = bars->item(i, 5)->text().toDouble();
        e = bars->item(i, 3)->text().toDouble();
        a = bars->item(i, 2)->text().toDouble();
        l = getLength(i+1, bars, nodes);

        for(double x = 0; x < l; x+=0.1)
        {
            temp.first = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toDouble() + x;
            temp.second = npx(x, e, a, l, up0, upl, q);
            res.push_back(temp);
        }
        temp.first = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toDouble() + l;
        temp.second = npx(l, e, a, l, up0, upl, q);
        res.push_back(temp);

//        temp.first = nodes->item(bars->item(i, 0)->text().toInt()-1, 0)->text().toUInt();
//        temp.second = npx(0, e, a, l, up0, upl, q);
//        res.push_back(temp);

//        temp.first = nodes->item(bars->item(i, 1)->text().toInt()-1, 0)->text().toUInt();
//        temp.second = npx(l, e, a, l, up0, upl, q);
//        res.push_back(temp);
    }

    return res;
}

double sigmax (double n, double a)
{
    return n/a;
}


std::vector<std::pair<double, double> > makeCoordsSigmax (std::vector<std::pair<double, double> > n,
                                                                QTableWidget* bars, QTableWidget* nodes)
{
    std::vector<std::pair<double, double> > res;
    std::pair<double, double> temp;

//    unsigned int l;
    double a;
    unsigned int barTemp;
    unsigned int tempCoord;

    for (unsigned int i = 0; i < n.size(); i++)
    {
        for(unsigned int x = 0; x < nodes->rowCount(); x++)
        {
            tempCoord = nodes->item(x, 0)->text().toUInt();
            if(n.at(i).first >= tempCoord)
            {
                barTemp = getBarBeginsIn(x+1, bars);
                break;
            }
        }
        a = bars->item(barTemp-1, 2)->text().toDouble();
        temp.first = n.at(i).first;
        temp.second = sigmax(n.at(i).second, a);
        res.push_back(temp);
    }

    return res;
}
