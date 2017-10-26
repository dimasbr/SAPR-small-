#include "saveload.h"

void saveProj(QTableWidget* nodes, QTableWidget* bars, QCheckBox* leftFix, QCheckBox* rightFix, std::string fileName)
{
    std::ofstream file;
    file.open(fileName.c_str());

    std::string temp = "";

    if(leftFix->isChecked()) temp+="1 ";
    else temp+="0 ";
    if(rightFix->isChecked()) temp+="1\n";
    else temp+="0\n";

    file<<temp;

    for (int i=0; i < nodes->rowCount(); i++)
    {
        temp="n ";
        temp+=nodes->item(i, 0)->text().toStdString();
        temp+=" ";
        temp+=nodes->item(i, 1)->text().toStdString();
        temp+="\n";
        file<<temp;
    }

    for (int i=0; i < bars->rowCount(); i++)
    {
        temp="b ";
        for(int k=0; k < bars->columnCount(); k++)
        {
            temp+=bars->item(i, k)->text().toStdString();
            temp+=" ";
        }
        temp+="\n";
        file<<temp;
    }

    file.close();
}

void loadProj(QTableWidget* nodes, QTableWidget* bars, QCheckBox* leftFix, QCheckBox* rightFix, std::string fileName){}

