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
#define TWISTER_SISTER "Twister sister"
#define TWISTER "Twister"
#define PISTOL "Pistol"

inline char GenRNAMatchBase(char Base){
    switch (Base) {
    case 'A':
        return 'U';
    case 'G':
        return 'C';
    case 'C':
        return 'G';
    case 'U':
        return 'A';
    default:
        DEBUG_WARN("ERROR BASE\n");
        return 0;
    }
}
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
    QModelIndex CurIndex = ui->ResultView->selectionModel()->currentIndex();//获取当前行
    QModelIndex MatchRNASeqIndex=ui->ResultView->model()->index(CurIndex.row(),DNA_SEQ,QModelIndex());
    QModelIndex RibozymeSeqIndex=ui->ResultView->model()->index(CurIndex.row(),RIBOZYME_SEQ,QModelIndex());
    MatchRNASeq=MatchRNASeqIndex.data().toString().toStdString();
    RibozymeSeq=RibozymeSeqIndex.data().toString().toStdString();
    qDebug()<<MatchRNASeq.c_str()<<" "<<RibozymeSeq.c_str();
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
    ZymeList<<TWISTER_SISTER<<TWISTER<<PISTOL;
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
     ResultViewRibozymeType= ui->comboBox->currentText().toStdString();
     QStandardItemModel* model=(QStandardItemModel*)ui->ResultView->model();
     model->removeRows(0,model->rowCount());
     if(!CalculateResult.empty()) {
         SetResultView(CalculateResult);
     }
     return;
}
void MainWindow::DrawRibozymeImage()
{
    if(MatchRNASeq.empty() || RibozymeSeq.empty()){
        return;
    }
    if(ResultViewRibozymeType==TWISTER_SISTER){
       DrawTwisterSister();
    }
    else if(ResultViewRibozymeType==TWISTER){
       DrawTwister();
    }
    else if(ResultViewRibozymeType==PISTOL){
        DrawPistol();
    }
}
void MainWindow::DrawBase(QPainter *Painter, unsigned int x, unsigned int y, char Base)
{
    if(Painter==NULL){
        return;
    }
    if(Base=='T'){
        Base='U';
    }
    switch (Base) {
     case 'A':
        Painter->setPen(Qt::red);
        break;
     case 'C':
        Painter->setPen(Qt::yellow);
        break;
     case 'G':
        Painter->setPen(Qt::green);
        break;
    case  'U':
       Painter->setPen(Qt::gray);
       break;
    default:
        break;
    }
    Painter->drawText(x,y,QString(Base));
}
void MainWindow::DrawBasePair(QPainter *Painter, unsigned int BaseX, unsigned int BaseY, char Base,
                              unsigned int PairX, unsigned int PairY)
{
    char MatchBase;
    DrawBase(Painter,BaseX,BaseY,Base);
    MatchBase=GenRNAMatchBase(Base);
    if(MatchBase){
        DrawBase(Painter,PairX,PairY,MatchBase);
    }
    else{
        DEBUG_WARN("Invalid Base,fail to draw\n");
        return;
    }
   return;
}
void MainWindow::DrawTwisterSister()
{
   QPainter Painter(ui->Picture);
   QFont PictureFont;
   PictureFont.setFamily("Microsoft YaHei");
   PictureFont.setPointSize(PICTURE_FONT_SIZE);
   Painter.setFont(PictureFont);
   smatch RegexResult;
   regex RegexPartern("GCT[A,G,C,T]A[A,G,C,T]");
   string::const_iterator MatchRNASeqBeg = MatchRNASeq.begin();
   string::const_iterator MatchRNASeqEnd=MatchRNASeq.end();
   if(regex_search(MatchRNASeqBeg,MatchRNASeqEnd,RegexResult,RegexPartern)){
       int i=0;
       for(auto it=MatchRNASeq.begin();it!=MatchRNASeq.end();++it){
             if (it <= RegexResult[0].first - 4){
                 DrawBase(&Painter,TWISTER_SISTER_BEGIN_X-16*i,TWISTER_SISTER_BEGIN_Y+16*i,*it);
                 i++;
             }
       }
   }
   DrawBase(&Painter,PictureWidth/3,PictureHeight-10,RibozymeSeq[0]);
   Painter.drawText(PictureWidth/3,PictureHeight-28,QString(RibozymeSeq[0]));
}

void MainWindow::DrawTwister()
{

}
void MainWindow::DrawPistol()
{

}
