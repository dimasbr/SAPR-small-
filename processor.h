#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QTableWidget>
#include <QCheckBox>
#include <vector>

double getLength(unsigned int bar, QTableWidget* bars, QTableWidget* nodes);

unsigned int getBarBeginsIn(unsigned int node, QTableWidget*bars);

unsigned int getBarEndsIn(unsigned int node, QTableWidget*bars);

std::vector<double> makeBVector(QTableWidget* bars, QTableWidget* nodes, QCheckBox* left, QCheckBox* right);

std::vector<std::vector<double> > makeAMatrix(QTableWidget* bars, QTableWidget* nodes, QCheckBox* left, QCheckBox* right);

std::vector<double> solveSystem (std::vector<std::vector<double> > a, std::vector<double> b);

double upx (double x, double l, double up0, double upl, double q, double e, double a);

std::vector<std::pair<double, double> > makeCoordsUx (std::vector<double> delta,
                                                            QTableWidget* bars, QTableWidget*nodes);

double npx (double x, double e, double a, double l, double up0, double upl, double q);

std::vector<std::pair<double, double> > makeCoordsNx (std::vector<double> delta,
                                                            QTableWidget* bars, QTableWidget*nodes);

double sigmax (double n, double a);

std::vector<std::pair<double, double> > makeCoordsSigmax (std::vector<std::pair<double, double> > n,
                                                                QTableWidget* bars, QTableWidget* nodes);

#endif // PROCESSOR_H
