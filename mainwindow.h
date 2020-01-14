#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPainter>
#include <QMainWindow>
#include "ribozyme_calculator.h"
#include "ui_mainwindow.h"
#include<QTableView>
#include<QPoint>
enum Ribozymes {
  TWISTER_SISTER=1,
  TWISTER,
  PISTOL
};
typedef int RibozymeType;
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
private:
     void SetUpPictureWidget();
     void SetUpResultView();
     void CreateRightMenu();
     void DrawRibozymeImage();
     Ui::MainWindow *ui;
     ribozyme_calculator RNA_Calculator;
     QMenu* RightMenu;
     std::string RNA53,RNA35;
     RibozymeType ImageRibozymeType;
     unsigned int PictureX,PictureY,PictureWidth,PictureHeight;
     bool flag;
};

#endif // MAINWINDOW_H
