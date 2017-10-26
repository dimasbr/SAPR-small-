#include "saveload.h"

void saveProj(QTableWidget* nodes, QTableWidget* bars, QCheckBox* leftFix, QCheckBox* rightFix, std::string fileName)
{
    std::ofstream file;
    //fileName+=".ban";
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

void loadProj(QTableWidget* nodes, QTableWidget* bars, QCheckBox* leftFix, QCheckBox* rightFix, std::string fileName)
{
    std::ifstream file;
    file.open(fileName.c_str());

    nodes->setRowCount(0);
    bars->setRowCount(0);

    std::string temp;

    file>>temp;
    if (temp == "1")
    {
        leftFix->setChecked(true);
    }
    else leftFix->setChecked(false);

    file>>temp;
    if (temp == "1")
    {
        rightFix->setChecked(true);
    }
    else rightFix->setChecked(false);

    std::string tempValue;
    while(std::getline(file, temp))
    {
        std::stringstream tempStream(temp);
        tempStream>>tempValue;
        if(tempValue == "n")
        {
            nodes->insertRow(nodes->rowCount());
            tempValue="";

            QTableWidgetItem* tempItemCoord = new QTableWidgetItem;
            tempStream>>tempValue;
            tempItemCoord->setText(QString::fromStdString(tempValue));
            nodes->setItem(nodes->rowCount()-1, 0, tempItemCoord);
            tempValue="";

            QTableWidgetItem* tempItemFrc = new QTableWidgetItem;
            tempStream>>tempValue;
            tempItemFrc->setText(QString::fromStdString(tempValue));
            nodes->setItem(nodes->rowCount()-1, 1, tempItemFrc);
        }
        else if(tempValue == "b")
        {
            bars->insertRow(bars->rowCount());
            tempValue="";

            for (int i=0; i<bars->columnCount(); i++)
            {
                QTableWidgetItem* tempItem = new QTableWidgetItem;
                tempStream>>tempValue;
                tempItem->setText(QString::fromStdString(tempValue));
                bars->setItem(bars->rowCount()-1, i, tempItem);
                tempValue="";
            }
        }
    }
}

