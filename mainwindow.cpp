#include "mainwindow.h"
#include<QStringList>
#include "ui_mainwindow.h"
#include"ribozyme_calculator.h"
#include<QStandardItemModel>
#include<QMenu>
#include<QAction>
#include<QModelIndex>
using namespace std;
#define DNA_SEQ 0
#define RIBOZYME_SEQ 3
#define CDNA_SEQ 6
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetUpResultView();
    CreateRightMenu();
    SetUpPictureWidget();
    flag=false;
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::SetUpPictureWidget()
{
    QPalette pal(ui->Picture->palette());//初始化绘图区
    pal.setColor(QPalette::Background, Qt::white);
    ui->Picture->setAutoFillBackground(true); //设置背景
    ui->Picture->setPalette(pal);
    ui->Picture->installEventFilter(this);
    PictureX=ui->Picture->x();
    PictureY=ui->Picture->y();
    PictureWidth=ui->Picture->width();
    PictureHeight=ui->Picture->height();
}
void MainWindow::CreateRightMenu()
{
   ui->ResultView->setContextMenuPolicy(Qt::CustomContextMenu);
   QMenu* RightClickMenu=new QMenu(this);
   QAction* CopyAction = new QAction(u8"复制",this);
   QAction* DrawAction = new QAction(u8"绘图",this);
   RightClickMenu->addAction(DrawAction);
   RightClickMenu->addAction(CopyAction);
   connect(ui->ResultView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(OnRightMenuClicked(QPoint)));
   connect(CopyAction,SIGNAL(triggered()),this,SLOT(OnCopyActionClicked()));
   connect(DrawAction,SIGNAL(triggered()),this,SLOT(OnDrawActionClicked()));
   RightMenu=RightClickMenu;
}
void MainWindow::OnRightMenuClicked(QPoint ClickPos)
{
    if(RightMenu==NULL){
        return;
    }
    QModelIndex ClickIndex = ui->ResultView->indexAt(ClickPos);
    if(!ClickIndex.isValid()){
       ui->ResultView->clearSelection();
       return;
    }
    RightMenu->exec(QCursor::pos());
}
void MainWindow::OnDrawActionClicked()
{
    flag=true;
    ui->Picture->update();
}
void MainWindow::OnCopyActionClicked()
{
    qDebug()<<"in copy\n";
}
void MainWindow::SetUpResultView()
{
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
    ui->ResultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ResultView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ResultView->show();
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->Picture && event->type()==QEvent::Paint){
        if(flag){
            DrawRibozymeImage();
            return true;
        }
        else{
            return false;
        }
    }
    return QMainWindow::eventFilter(watched, event);
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
     }
     return;
}
void MainWindow::DrawRibozymeImage()
{
    QPainter painter(ui->Picture);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::green);
    qDebug()<<"in paint\n";
    painter.drawRect(400,100,400+20,100+20);
}
