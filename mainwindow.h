#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPainter>
#include <QMainWindow>
#include "ribozyme_calculator.h"
#include "ui_mainwindow.h"
#include<QTableView>
#include<QPoint>
#define PICTURE_FONT_SIZE 16
#define PICTURE_BASE_SPACE 2
#define TWISTER_SISTER_BEGIN_X ui->Picture->width()-50
#define TWISTER_SISTER_BEGIN_Y 50
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void SetResultView(std::vector<std::vector<std::string>> CalculateResult);
    ~MainWindow();
private slots:
     void on_calculate_clicked();
     void OnRightMenuClicked(QPoint ClickPos);
     void OnCopyActionClicked();
     void OnDrawActionClicked();
protected:
     bool eventFilter(QObject *watched, QEvent *event);
     void DrawRibozymeImage();
     void DrawTwisterSister();
     void DrawTwister();
     void DrawPistol();
private:
     void SetUpPictureWidget();
     void SetUpResultView();
     void CreateRightMenu();
     void DrawBase(QPainter *Painter,unsigned int x,unsigned int y,char Base);
     void DrawBasePair(QPainter *Painter,unsigned int BaseX,unsigned int BaseY,char Base,
                       unsigned int PairX,unsigned int PairY);
     Ui::MainWindow *ui;
     ribozyme_calculator RNA_Calculator;
     QMenu* RightMenu;
     std::string RibozymeSeq,MatchRNASeq,ResultViewRibozymeType;
     unsigned int PictureWidth,PictureHeight;
     bool flag;
};
#endif // MAINWINDOW_H
