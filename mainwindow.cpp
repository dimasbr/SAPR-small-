#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <set>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->barsTable->horizontalHeader()->resizeSection(0, 50);
    ui->barsTable->horizontalHeader()->resizeSection(1, 50);
    ui->errorWarning->setVisible(false);

    //scene.addText("blablabla");
    ui->renderArea->setScene(&scene);
//    scene.addText("blablabla");
    ui->renderArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->renderArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //ui->renderArea->scale(1.5, 1.5);
    //scene.setSceneRect(QRectF(0, 1, 0.5, 1));
    //ui->renderArea->fitInView(QRectF(-2, -2, 1, 1), Qt::KeepAspectRatio);
    draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::swapNodes(int first, int second)
{
    QTableWidgetItem* coord1 = ui->nodesTable->takeItem(first, 0);
    QTableWidgetItem* force1 = ui->nodesTable->takeItem(first, 1);
    QTableWidgetItem* coord2 = ui->nodesTable->takeItem(second, 0);
    QTableWidgetItem* force2 = ui->nodesTable->takeItem(second, 1);

    ui->nodesTable->setItem(first, 0, coord2);
    ui->nodesTable->setItem(first, 1, force2);
    ui->nodesTable->setItem(second, 0, coord1);
    ui->nodesTable->setItem(second, 1, force1);

    ui->nodesTable->setCurrentCell(second, 0);
}

void MainWindow::swapBars(int first, int second)
{
    QTableWidgetItem* begin1 = ui->barsTable->takeItem(first, 0);
    QTableWidgetItem* begin2 = ui->barsTable->takeItem(second, 0);

    QTableWidgetItem* end1 = ui->barsTable->takeItem(first, 1);
    QTableWidgetItem* end2 = ui->barsTable->takeItem(second, 1);

    QTableWidgetItem* area1 = ui->barsTable->takeItem(first, 2);
    QTableWidgetItem* area2 = ui->barsTable->takeItem(second, 2);

    QTableWidgetItem* freq1 = ui->barsTable->takeItem(first, 3);
    QTableWidgetItem* freq2 = ui->barsTable->takeItem(second, 3);

    QTableWidgetItem* maxFrc1 = ui->barsTable->takeItem(first, 4);
    QTableWidgetItem* maxFrc2 = ui->barsTable->takeItem(second, 4);

    QTableWidgetItem* frc1 = ui->barsTable->takeItem(first, 5);
    QTableWidgetItem* frc2 = ui->barsTable->takeItem(second, 5);

    ui->barsTable->setItem(first, 0, begin2);
    ui->barsTable->setItem(second, 0, begin1);

    ui->barsTable->setItem(first, 1, end2);
    ui->barsTable->setItem(second, 1, end1);

    ui->barsTable->setItem(first, 2, area2);
    ui->barsTable->setItem(second, 2, area1);

    ui->barsTable->setItem(first, 3, freq2);
    ui->barsTable->setItem(second, 3, freq1);

    ui->barsTable->setItem(first, 4, maxFrc2);
    ui->barsTable->setItem(second, 4, maxFrc1);

    ui->barsTable->setItem(first, 5, frc2);
    ui->barsTable->setItem(second, 5, frc1);

    ui->barsTable->setCurrentCell(second, 0);
}


void MainWindow::sortNodes()
{
    int i = 1;
    int j;

    while (i < ui->nodesTable->rowCount())
    {
        j=i;

        while ((j>0) && (ui->nodesTable->item(j-1, 0)->text().toDouble() > ui->nodesTable->item(j, 0)->text().toDouble()  ))
        {
            swapNodes(j, j-1);
            j--;
        }
        i++;
    }
}

void MainWindow::sortBars()
{
    int i = 1;
    int j;

    while (i < ui->barsTable->rowCount())
    {
        j=i;

        while ((j>0) && (ui->barsTable->item(j-1, 0)->text().toUInt() > ui->barsTable->item(j, 0)->text().toUInt()  ))
        {
            swapBars(j, j-1);
            j--;
        }
        i++;
    }
}


bool MainWindow::check()
{
    ui->errorWarning->setVisible(false);



    QTableWidgetItem* temp;
    std::set<double> coords;
    for (int i=0; i < ui->nodesTable->rowCount(); i++)
    {
        temp = ui->nodesTable->item(i, 0);
        if(!temp) return false;
        else
        {
            double k = temp->text().toDouble();
            if (coords.count(k))
            {
                ui->errorWarning->setVisible(true);
                ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
                                           font-weight:600; color:#e20000;\">ОШИБКА! Несколько узлов в одной точке!\
                                           </span></p></body></html>");
                return false;
            }
            else coords.insert(k);
        }

        if(temp->text().toDouble() < 0) temp->setText(QString::number( - temp->text().toDouble()));

        temp = ui->nodesTable->item(i, 1);
        if(!temp) return false;
    }

    sortNodes();



    for (int i=0; i < ui->barsTable->rowCount(); i++)
    {
        for (int k=0; k < ui->barsTable->columnCount(); k++)
        {
            temp = ui->barsTable->item(i, k);
            if(!temp)
                return false;
        }
    }

    sortBars();

//    if(ui->leftFix->isChecked())
//    {
//        if(ui->nodesTable->rowCount() && ui->nodesTable->item(0,1)->text().toDouble()<0)
//        {
//            ui->nodesTable->item(0, 1)->setText(QString::number(-(ui->nodesTable->item(0,1)->text().toDouble())));
//        }
//    }
//    if(ui->rightFix->isChecked())
//    {
//        int tempSize = ui->nodesTable->rowCount();
//        if(tempSize && ui->nodesTable->item(tempSize-1,1)->text().toDouble()>0)
//        {
//            ui->nodesTable->item(tempSize-1, 1)->setText(QString::number(-(ui->nodesTable->item(tempSize-1,1)->text().toDouble())));
//        }
//    }




    std::set<double> begins;
    std::set<double> ends;
    for (int i=0; i < ui->barsTable->rowCount(); i++)
    {
        QTableWidgetItem* tempBegin = ui->barsTable->item(i, 0);
        QTableWidgetItem* tempEnd = ui->barsTable->item(i, 1);

        double tmpBeg = tempBegin->text().toDouble();
        double tmpEnd = tempEnd->text().toDouble();

        if(tmpBeg > tmpEnd)
        {
            tempBegin->setText(QString::number(tmpEnd));
            tempEnd->setText(QString::number(tmpBeg));

            double tempBuffer = tmpBeg;
            tmpBeg = tmpEnd;
            tmpEnd = tempBuffer;
        }

        if (tmpBeg > ui->nodesTable->rowCount())
        {
            if(ui->nodesTable->rowCount())
            {
               tmpBeg = ui->nodesTable->rowCount()-1;
               tempBegin->setText(QString::number(tmpBeg));
            }
        }

        if (tmpEnd > ui->nodesTable->rowCount())
        {
            if(ui->nodesTable->rowCount())
            {
               tmpEnd = ui->nodesTable->rowCount()-1;
               tempEnd->setText(QString::number(tmpEnd));
            }
        }

        if(tmpBeg != (tmpEnd-1))
        {
            if(tmpEnd < ui->nodesTable->rowCount())
            {
                tempEnd->setText(QString::number(tmpBeg+1));
                tmpEnd=tmpBeg+1;
            }
            else
            {
                ui->errorWarning->setVisible(true);
                ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
                                           font-weight:600; color:#e20000;\">ОШИБКА! Стержень начинается и заканчивается в одном узле!!\
                                           </span></p></body></html>");
                return false;
            }
        }

        if(begins.count(tmpBeg))
        {
            ui->errorWarning->setVisible(true);
            ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
                                       font-weight:600; color:#e20000;\">ОШИБКА! Два стержня имеют начало в одном узле!\
                                       </span></p></body></html>");
            return false;
        }
        begins.insert(tmpBeg);

        if(ends.count(tmpEnd))
        {
            ui->errorWarning->setVisible(true);
            ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
                                       font-weight:600; color:#e20000;\">ОШИБКА! Два стержня имеют конец в одном узле!\
                                       </span></p></body></html>");
            return false;
        }
        ends.insert(tmpEnd);

//        if(tmpBeg == tmpEnd)
//        {
//            ui->errorWarning->setVisible(true);
//            ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
//                                       font-weight:600; color:#e20000;\">ОШИБКА! Стержень начинается и заканчивается в одном узле!\
//                                       </span></p></body></html>");
//            return false;
//        }



        if(!(ui->leftFix->isChecked() || ui->rightFix->isChecked()))
        {
            ui->errorWarning->setVisible(true);
            ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
                                           font-weight:600; color:#e20000;\">ОШИБКА! Мгновенный механизм!\
                                           </span></p></body></html>");
            return false;
        }

        if(i)
        {
            QTableWidgetItem* tempEndPrev = ui->barsTable->item(i-1, 1);
            double tmpEndPrev = tempEndPrev->text().toDouble();
            if(tmpBeg!=tmpEndPrev)
            {
                ui->errorWarning->setVisible(true);
                ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
                                           font-weight:600; color:#e20000;\">ОШИБКА! Конструкция не целостная!\
                                           </span></p></body></html>");
                return false;
            }
        }

    }

    if((!begins.count(1) || !ends.count(ui->nodesTable->rowCount())) && ui->nodesTable->rowCount())
    {
        ui->errorWarning->setVisible(true);
        ui->errorWarning->setText( "<html><head/><body><p><span style=\"\
                                   font-weight:600; color:#e20000;\">ОШИБКА! Свободный узел!\
                                   </span></p></body></html>");

        return false;
    }

    return true;

}



void MainWindow::on_addNode_clicked()
{
    ui->nodesTable->insertRow(ui->nodesTable->rowCount());

    QTableWidgetItem* tempCoord = new QTableWidgetItem;
    QTableWidgetItem* tempForce = new QTableWidgetItem;

    tempCoord->setText("0");
    ui->nodesTable->setItem(ui->nodesTable->rowCount()-1, 0, tempCoord);

    tempForce->setText("0.0");
    ui->nodesTable->setItem(ui->nodesTable->rowCount()-1, 1, tempForce);
}

void MainWindow::on_deleteNode_clicked()
{
    ui->nodesTable->removeRow(ui->nodesTable->currentRow());

//    unsigned int temp;
//    for (int i=0; i < ui->barsTable->rowCount(); i++)
//    {
//        temp = ui->barsTable->item(i, 0)->text().toUInt();
//        if (temp > ui->nodesTable->rowCount())
//        {
//            ui->barsTable->item(i, 0)->setText(QString::number(ui->nodesTable->rowCount()));
//        }

//        temp = ui->barsTable->item(i, 1)->text().toUInt();
//        if (temp > ui->nodesTable->rowCount())
//        {
//            int blabla=ui->nodesTable->rowCount();
//            QTableWidgetItem* bla = ui->barsTable->item(i, 1);
//            /*ui->barsTable->item(i, 1)*/bla->setText(QString::number(ui->nodesTable->rowCount()));
//        }
//    }
    if (check())
    {
        draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
    }
}

void MainWindow::on_addBar_clicked()
{
    ui->barsTable->insertRow(ui->barsTable->rowCount());

    QTableWidgetItem* tempBegin = new QTableWidgetItem;
    QTableWidgetItem* tempEnd = new QTableWidgetItem;
    QTableWidgetItem* tempArea = new QTableWidgetItem;
    QTableWidgetItem* tempFrequency = new QTableWidgetItem;
    QTableWidgetItem* tempMaxForce = new QTableWidgetItem;
    QTableWidgetItem* tempForce = new QTableWidgetItem;

    if(ui->barsTable->rowCount()==1)
    {

        if (ui->nodesTable->rowCount()==0)
        {
            ui->nodesTable->insertRow(0);
            ui->nodesTable->insertRow(1);

            QTableWidgetItem* tempCoord1 = new QTableWidgetItem;
            QTableWidgetItem* tempForce1 = new QTableWidgetItem;
            QTableWidgetItem* tempCoord2 = new QTableWidgetItem;
            QTableWidgetItem* tempForce2 = new QTableWidgetItem;

            tempCoord1->setText("0");
            tempCoord2->setText("0");
            ui->nodesTable->setItem(0, 0, tempCoord1);
            ui->nodesTable->setItem(1, 0, tempCoord2);

            tempForce1->setText("0.0");
            tempForce2->setText("0.0");
            ui->nodesTable->setItem(0, 1, tempForce1);
            ui->nodesTable->setItem(1, 1, tempForce2);
        }

        if (ui->nodesTable->rowCount()==1)
        {
            ui->nodesTable->insertRow(1);

            QTableWidgetItem* tempCoord1 = new QTableWidgetItem;
            QTableWidgetItem* tempForce1 = new QTableWidgetItem;

            tempCoord1->setText("0");
            ui->nodesTable->setItem(1, 0, tempCoord1);

            tempForce1->setText("0.0");
            ui->nodesTable->setItem(1, 1, tempForce1);
        }


        tempBegin->setText("1");
        ui->barsTable->setItem(0, 0, tempBegin);

        tempEnd->setText("2");
        ui->barsTable->setItem(0, 1, tempEnd);

        tempArea->setText("1.0");
        ui->barsTable->setItem(0, 2, tempArea);

        tempFrequency->setText("1.0");
        ui->barsTable->setItem(0, 3, tempFrequency);

        tempMaxForce->setText("1.0");
        ui->barsTable->setItem(0, 4, tempMaxForce);

        tempForce->setText("0.0");
        ui->barsTable->setItem(0, 5, tempForce);
    }

    else
    {
        tempBegin->setText(ui->barsTable->item(ui->barsTable->rowCount()-2, 1)->text());
        ui->barsTable->setItem(ui->barsTable->rowCount()-1, 0, tempBegin);

        int tempEndNumber = ui->barsTable->item(ui->barsTable->rowCount()-2, 1)->text().toInt();
        tempEndNumber++;

        if(tempEndNumber > ui->nodesTable->rowCount())
        {
            ui->nodesTable->insertRow(ui->nodesTable->rowCount());

            QTableWidgetItem* tempCoordNode = new QTableWidgetItem;
            QTableWidgetItem* tempForceNode = new QTableWidgetItem;

            tempCoordNode->setText("0");
            ui->nodesTable->setItem(ui->nodesTable->rowCount()-1, 0, tempCoordNode);

            tempForceNode->setText("0.0");
            ui->nodesTable->setItem(ui->nodesTable->rowCount()-1, 1, tempForceNode);
        }

        tempEnd->setText(QString::number(tempEndNumber));
        ui->barsTable->setItem(ui->barsTable->rowCount()-1, 1, tempEnd);

        tempArea->setText(ui->barsTable->item(ui->barsTable->rowCount()-2, 2)->text());
        ui->barsTable->setItem(ui->barsTable->rowCount()-1, 2, tempArea);

        tempFrequency->setText("1.0");
        ui->barsTable->setItem(ui->barsTable->rowCount()-1, 3, tempFrequency);

        tempMaxForce->setText("1.0");
        ui->barsTable->setItem(ui->barsTable->rowCount()-1, 4, tempMaxForce);

        tempForce->setText("0.0");
        ui->barsTable->setItem(ui->barsTable->rowCount()-1, 5, tempForce);
    }

}

void MainWindow::on_deleteBar_clicked()
{
    ui->barsTable->removeRow(ui->barsTable->currentRow());
    if (check())
    {
        draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
    }
}

void MainWindow::on_nodesTable_cellChanged(int row, int column)
{
    if (column==0)
    {
       double temp=0;
       try
       {
           temp=ui->nodesTable->item(row, column)->text().toDouble();
       }
       catch(...)
       {

       }
       ui->nodesTable->item(row, column)->setText(QString::number(temp));
    }
    if (column==1)
    {
        double temp=0;
        try
        {
            temp=ui->nodesTable->item(row, column)->text().toDouble();
        }
        catch(...)
        {

        }
        ui->nodesTable->item(row, column)->setText(QString::number(temp));
    }

    if (check())
    {
        draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
    }
}

void MainWindow::on_barsTable_cellChanged(int row, int column)
{
    if (column==0 || column==1)
    {
        unsigned int temp=1;
        try
        {
            temp=ui->barsTable->item(row, column)->text().toUInt();
        }
        catch(...)
        {

        }

        if(temp == 0)
        {
            ui->barsTable->item(row, column)->setText(QString::number(1));
            temp=1;
        }

        if (ui->nodesTable->rowCount() >= temp)
        {
            ui->barsTable->item(row, column)->setText(QString::number(temp));
        }
        else
        {
           ui->barsTable->item(row, column)->setText(QString::number(ui->nodesTable->rowCount()));
        }

    }
    if (column==2)
    {
        double temp=1;
        try
        {
            temp=ui->barsTable->item(row, column)->text().toDouble();
        }
        catch(...)
        {

        }
        if(temp > 0)
        {
            ui->barsTable->item(row, column)->setText(QString::number(temp));
        }
        else
        {
            ui->barsTable->item(row, column)->setText("1");
        }
    }
    if (column==3)
    {
        double temp=1;
        try
        {
            temp=ui->barsTable->item(row, column)->text().toDouble();
        }
        catch(...)
        {

        }
        if(temp > 0)
        {
            ui->barsTable->item(row, column)->setText(QString::number(temp));
        }
        else
        {
            ui->barsTable->item(row, column)->setText("1");
        }
    }
    if (column==4)
    {
        double temp=1;
        try
        {
            temp=ui->barsTable->item(row, column)->text().toDouble();
        }
        catch(...)
        {

        }
        if(temp > 0)
        {
            ui->barsTable->item(row, column)->setText(QString::number(temp));
        }
        else
        {
            ui->barsTable->item(row, column)->setText("1");
        }
    }
    if (column==5)
    {
        double temp=1;
        try
        {
            temp=ui->barsTable->item(row, column)->text().toDouble();
        }
        catch(...)
        {

        }
        ui->barsTable->item(row, column)->setText(QString::number(temp));
    }

    if (check())
    {
        draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
    }

}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    ui->renderArea->resetMatrix();
    ui->renderArea->scale(value, value);
    draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
//    ui->renderArea->update();
}

void MainWindow::on_rightFix_stateChanged(int arg1)
{
    if (check())
    {
        draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
    }
}

void MainWindow::on_leftFix_stateChanged(int arg1)
{
    if (check())
    {
        draw(ui->nodesTable, ui->barsTable, ui->renderArea, ui->leftFix, ui->rightFix);
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString qFileName = QFileDialog::getSaveFileName(0, QString("Сохранить проект"),
                                                        QString(""),
                                                     QString("*.ban"));
    if(!qFileName.endsWith(".ban"))
        qFileName+=".ban";
    std::string fileName = qFileName.toStdString();
    saveProj(ui->nodesTable, ui->barsTable, ui->leftFix, ui->rightFix, fileName);
}

void MainWindow::on_actionLoad_triggered()
{
    std::string fileName = QFileDialog::getOpenFileName(0, QString("Открыть проект"),
                                                        QString(""), QString("*.ban")).toStdString();
    if(fileName == "") return;
    loadProj(ui->nodesTable, ui->barsTable, ui->leftFix, ui->rightFix, fileName);
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox helpText;
    helpText.setText("<html><head/><body><p>Программа написана.</p></body></html>");
    helpText.exec();
}

void MainWindow::on_processor_clicked()
{
    if (ui->nodesTable->rowCount() < 2 || ui->barsTable->rowCount() < 1 || !check()) return;

    postprocessor = new Postprocessor(this, ui->barsTable, ui->nodesTable,
                                      ui->leftFix, ui->rightFix);
    postprocessor->show();
}
