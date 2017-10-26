#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <QTableWidget>
#include <QCheckBox>

void saveProj(QTableWidget* nodes, QTableWidget* bars, QCheckBox* leftFix, QCheckBox* rightFix, std::string fileName);
void loadProj(QTableWidget* nodes, QTableWidget* bars, QCheckBox* leftFix, QCheckBox* rightFix, std::string fileName);

#endif // SAVELOAD_H
