/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2022 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: https://www.qcustomplot.com/                         **
**             Date: 06.11.22                                             **
**          Version: 2.1.1                                                **
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ResultCollector* r,QWidget *parent)
    :QMainWindow(parent)
    ,rc(r)
    ,ui(new Ui::MainWindow)
    ,mPlot(0)
  //mTag1(0)
  //mTag2(0)
{
  ui->setupUi(this);
  
  mPlot = new QCustomPlot(this);
  setCentralWidget(mPlot);
  
  //配置图表具有两个右轴:
  mPlot->yAxis->setTickLabels(false);
  mPlot->yAxis->setRange(0,1);
  mPlot->yAxis2->setVisible(true);
  mGraph0 = mPlot->addGraph();
  mGraph1 = mPlot->addGraph();
  mGraph2 = mPlot->addGraph();
  mGraph3 = mPlot->addGraph();
  mGraph4 = mPlot->addGraph();
  mGraph5 = mPlot->addGraph();
  mGraph0->setPen(QPen(QColor(255, 165, 0)));
  mGraph1->setPen(QPen(QColor(0,255,0)));
  mGraph2->setPen(QPen(QColor(255, 0, 0)));
  mGraph3->setPen(QPen(QColor(0, 0, 255)));
  mGraph4->setPen(QPen(QColor(255, 255, 0)));
  mGraph5->setPen(QPen(QColor(128, 0, 128)));
  connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
  mDataTimer.start(40);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::timerSlot()
{
  // 计算并添加每个图表的新数据点:
 mPlot->yAxis->setRange(0, 1);
  mPlot->yAxis2->setRange(0,1);
  double num_0=rc->crazing_num/rc->total_num;
  double num_1=rc->inclusion_num/rc->total_num;
  double num_2=rc->patches_num/rc->total_num;
  double num_3=rc->pitted_surface_num/rc->total_num;
  double num_4=rc->rolled_scale_num/rc->total_num;
  double num_5=rc->scratches_num.load()/rc->total_num.load();
      mGraph0->addData(rc->total_num,num_0);
      mGraph1->addData(rc->total_num,num_1);
      mGraph2->addData(rc->total_num,num_2);
      mGraph3->addData(rc->total_num,num_3);
      mGraph4->addData(rc->total_num,num_4);
      mGraph5->addData(rc->total_num,num_5);
  std::cout << "Graph0 Data: " << mGraph0->dataCount() << " points" << std::endl;
  for (int i = 0; i < mGraph0->dataCount(); ++i) {
      std::cout << "(" << mGraph0->data()->at(i)->key << ", " << mGraph0->data()->at(i)->value << ")" << std::endl;
  }
  mPlot->xAxis->setRange(rc->total_num, 5, Qt::AlignRight);
  
  mPlot->replot();
}
