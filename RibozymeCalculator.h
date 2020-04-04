#ifndef RIBOZYME_CALCULATOR_H
#define RIBOZYME_CALCULATOR_H
#include <QTextEdit>
#include<QComboBox>
#include<algorithm>
#include<QTableView>
#include<QStandardItem>
#include<vector>
#include<map>
#include<QtCore/qmath.h>
#include<QtNetwork/QtNetwork>
#include<stdio.h>
#include"common.h"
#define SUCEESS 0
#define ERROR -1
#define TWISTER_MAX_BACK_LENGTH 8
#define PISTOL_MAX_FRONT_LENGTH 10
#define HAMMER_MAX_FRONT_LENGTH 5
using namespace std;

class RibozymeCalculator
{
public:
    RibozymeCalculator():PistolGTBeginPos(0){}
    string GenRegexPattern(string TarRNA);
    string GenCDNA(string Ribozyme);
    string CalculateGCPercent(string Ribozyme);
    string CalculateTM(string Ribozyme,string ZymeType);
    int Calculate(string DNASeq, string TarRNA,string ZymeType,
                 vector< vector< string>>& CalculateResult);
 private:
   void CalculateTwisterSister(string& MatchRNA,smatch SubRegexResult,string& Ribozyme);
   void CalculateTwister(string& MatchRNA, smatch SubRegexResult,string& Ribozyme);
   void CalculatePistol(string& MatchRNA,string& Ribozyme);
   void CalculateHammer(string& MatchRNA,string& Ribozyme);
   void CalculateRibozymeParas(string& MatchRNA, string& Ribozyme, unsigned int MatchBeginPos,
                               unsigned int MatchEndPos,string& ZymeType, vector<string> &CalculateResultItem);
   unsigned int PistolGTBeginPos,HammerTCBeginPos;
};

#endif // RIBOZYME_CALCULATOR_H
