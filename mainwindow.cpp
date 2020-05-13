#include "mainwindow.h"
#include<QStringList>
#include"imagepainter.h"
#include "ui_mainwindow.h"
#include"RibozymeCalculator.h"
#include<QStandardItemModel>
#include<QMenu>
#include<QAction>
#include<QModelIndex>
using namespace std;
#define DNA_SEQ 0
#define RIBOZYME_SEQ 3
#define TM_SEQ 5
#define CDNA_SEQ 7

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetUpResultView();
    CreateRightMenu();
    SetUpPictureWidget();
    HasDrawButtonClicked=flag=false;
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
    HasDrawButtonClicked=flag=true;
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
    ZymeList<<TWISTER_SISTER<<TWISTER<<PISTOL<<HAMMER_HEAD;
    ui->comboBox->addItems(ZymeList);
    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(OnBoxChange(QString)));
    ui->TarRNA->setText("NNNNNNNNNGCTNANNNNNNN");//因为默认第一个是sister
    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels = QObject::trUtf8(" 靶向RNA序列,开始位置,结束位置,核酶序列,核酶GC比例,用以计算TM值的序列,TM值估算结果,cDNA序列").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);
    ui->ResultView->setModel(model);
    ui->ResultView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
    ui->ResultView->horizontalHeader()->setSectionResizeMode(DNA_SEQ, QHeaderView::ResizeToContents);
    ui->ResultView->horizontalHeader()->setSectionResizeMode(RIBOZYME_SEQ, QHeaderView::ResizeToContents);
    ui->ResultView->horizontalHeader()->setSectionResizeMode(TM_SEQ, QHeaderView::ResizeToContents);
    ui->ResultView->horizontalHeader()->setSectionResizeMode(CDNA_SEQ, QHeaderView::ResizeToContents);  //然后设置要根据内容使用宽度的列
    ui->ResultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ResultView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ResultView->show();
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->Picture && event->type()==QEvent::Paint){
        if(HasDrawButtonClicked){
            QPainter Painter(ui->Picture);
            string MatchRNASeq,RibozymeType;
            Painter.setRenderHints(QPainter::Antialiasing, true);
            if(flag){
                QModelIndex CurIndex = ui->ResultView->selectionModel()->currentIndex();//获取当前行
                QModelIndex MatchRNASeqIndex=ui->ResultView->model()->index(CurIndex.row(),DNA_SEQ,QModelIndex());
                MatchRNASeq=MatchRNASeqIndex.data().toString().toStdString();
                LastDrawRNASeq=MatchRNASeq;
                LastDrawRibozymeType=ResultViewRibozymeType;
                RibozymeType=ResultViewRibozymeType;
            }
            else{
                MatchRNASeq=LastDrawRNASeq;
                RibozymeType=LastDrawRibozymeType;
            }
            RibozymeImagePainter* MyRibozymeImagePainter=new RibozymeImagePainter(RibozymeType);
            MyRibozymeImagePainter->DrawRibozymeImage(MatchRNASeq,ui->Picture->width(),ui->Picture->height(),&Painter);
            delete MyRibozymeImagePainter;
            flag=false;
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
     QString DNAseq=ui->DNAseq->toPlainText();
     DNAseq=DNAseq.toUpper();
     RNA_Calculator.Calculate(DNAseq.toStdString(),ui->TarRNA->toPlainText().toStdString(),
                              ui->comboBox->currentText().toStdString(),CalculateResult);
     ResultViewRibozymeType= ui->comboBox->currentText().toStdString();
     QStandardItemModel* model=(QStandardItemModel*)ui->ResultView->model();
     model->removeRows(0,model->rowCount());
     if(!CalculateResult.empty()) {
         SetResultView(CalculateResult);
     }
     return;
}
void MainWindow::OnBoxChange(const QString &text)
{
    if(text==TWISTER_SISTER){
        ui->TarRNA->setText("NNNNNNNNNGCTNANNNNNNN");
    }
    else if(text==TWISTER){
        ui->TarRNA->setText("NNNNNNAANNNNGC");
    }
    else if(text==PISTOL){
        ui->TarRNA->setText("NNNNNGTNNNN");
    }
    else if(text==HAMMER_HEAD){
        ui->TarRNA->setText("NNNNTCNNNNN");
    }
}
/*void MainWindow::on_export_2_clicked()
{

    QSqlDatabase SqlSeverDB=QSqlDatabase::addDatabase("QODBC");
    SqlSeverDB.setHostName("127.0.0.1");
    SqlSeverDB.setPort(1433);
    SqlSeverDB.setUserName("MCM1");
    SqlSeverDB.setPassword("123456789");
    SqlSeverDB.setDatabaseName("test");
    if(SqlSeverDB.open()){
        QSqlQuery query;
        QStandardItemModel* model=(QStandardItemModel*)ui->ResultView->model();
        QString InsertStr;
        vector<QString> LineData;
        for(int i=0;i<model->rowCount();++i){
            LineData.clear();
            for(int j=0;j<model->columnCount();++j){
                LineData.push_back(model->item(i,j)->text());
            }
            InsertStr=QString("insert into PistolData VALUES('%1','%2','%3','%4','%5','%6','%7','%8')").arg(LineData[0]).arg(LineData[1]).arg(LineData[2]).arg(LineData[3])
                                                                                    .arg(LineData[4]).arg(LineData[5]).arg(LineData[6]).arg(LineData[7]);
            query.prepare(InsertStr);
            if(!query.exec()){
                  qDebug()<<"query error :"<<query.lastError().databaseText();
            }
        }
        SqlSeverDB.close();
    }
    else{
        qDebug()<<SqlSeverDB.lastError().text()<<endl;
        return;
    }
}*/
