#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QTableWidget>
#include <QCheckBox>
#include <vector>

unsigned int getLength(unsigned int bar, QTableWidget* bars, QTableWidget* nodes);

unsigned int getBarBeginsIn(unsigned int node, QTableWidget*bars);

unsigned int getBarEndsIn(unsigned int node, QTableWidget*bars);

std::vector<double> makeBVector(QTableWidget* bars, QTableWidget* nodes, QCheckBox* left, QCheckBox* right);

std::vector<std::vector<double> > makeAMatrix(QTableWidget* bars, QTableWidget* nodes, QCheckBox* left, QCheckBox* right);

std::vector<double> solveSystem (std::vector<std::vector<double> > a, std::vector<double> b);

double upx (double x, unsigned int l, double up0, double upl, double q, double e, double a);

std::vector<std::pair<unsigned int, double> > makeCoordsUx (std::vector<double> delta,
                                                            QTableWidget* bars, QTableWidget*nodes);

double npx (double x, double e, double a, unsigned int l, double up0, double upl, double q);

std::vector<std::pair<unsigned int, double> > makeCoordsNx (std::vector<double> delta,
                                                            QTableWidget* bars, QTableWidget*nodes);

double sigmax (double n, double a);

std::vector<std::pair<unsigned int, double> > makeCoordsSigmax (std::vector<std::pair<unsigned int, double> > n,
                                                                QTableWidget* bars);

#endif // PROCESSOR_H
