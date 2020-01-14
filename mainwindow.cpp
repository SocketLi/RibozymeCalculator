#include "mainwindow.h"
#include<QStringList>
#include "ui_mainwindow.h"
#include"ribozyme_calculator.h"
#include<QStandardItemModel>
#include"mywidget.h"
using namespace std;
#define DNA_SEQ 0
#define RIBOZYME_SEQ 3
#define CDNA_SEQ 6
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->clear();
    QStringList ZymeList;
    ZymeList<<"Twister sister"<<"Twister"<<"Pistol";
    ui->comboBox->addItems(ZymeList);
    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels = QObject::trUtf8(" 匹配序列(DNA),开始位置,结束位置,核酶序列,GC比例,TM值,cDNA序列").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);
    ui->ResultView->setModel(model);
    ui->ResultView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    ui->ResultView->horizontalHeader()->setSectionResizeMode(DNA_SEQ, QHeaderView::ResizeToContents);
    ui->ResultView->horizontalHeader()->setSectionResizeMode(RIBOZYME_SEQ, QHeaderView::ResizeToContents);
    ui->ResultView->horizontalHeader()->setSectionResizeMode(CDNA_SEQ, QHeaderView::ResizeToContents);  //然后设置要根据内容使用宽度的列

    QPalette pal(ui->Picture->palette());//初始化绘图区
    pal.setColor(QPalette::Background, Qt::white);
    ui->Picture->setAutoFillBackground(true); //设置背景
    ui->Picture->setPalette(pal);
    DrawArea=ui->Picture;
    DrawArea->SetPictureParas(ui->Picture->x(),ui->Picture->y(),ui->Picture->width(),ui->Picture->height());
    DrawArea->SetDrawClicked(false);
    ui->ResultView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::SetResultView(vector<vector<string>> CalculateResult)
{
    QStandardItemModel* model=(QStandardItemModel*)ui->ResultView->model();
    for(unsigned int i=0;i<CalculateResult.size();++i)
    {
        for(unsigned int j=0;j<CalculateResult[i].size();++j)
        {
            QStandardItem *item=new QStandardItem(QString::fromStdString(CalculateResult[i][j]));
            model->setItem(i,j,item);
        }
    }
     ui->ResultView->show();
}
void MainWindow::on_calculate_clicked()
{
     vector<vector<string>> CalculateResult;
     RNA_Calculator.Calculate(ui->DNAseq->toPlainText().toStdString(),ui->TarRNA->toPlainText().toStdString(),
                              ui->comboBox->currentText().toStdString(),CalculateResult);
     QStandardItemModel* model=(QStandardItemModel*)ui->ResultView->model();
     model->removeRows(0,model->rowCount());
     if(!CalculateResult.empty()) {
         SetResultView(CalculateResult);
         DrawArea->SetDrawClicked(true);
         qDebug()<<"in click"<<endl;
        //
     }
     return;
}
