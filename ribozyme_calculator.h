#ifndef RIBOZYME_CALCULATOR_H
#define RIBOZYME_CALCULATOR_H
#include <QTextEdit>
#include<QComboBox>
#include<regex>
#include<string>
#include<algorithm>
#include<QTableView>
#include<QStandardItem>
#include<vector>
#include<map>
#include<QtCore/qmath.h>
#include<QtNetwork/QtNetwork>
#define SUCEESS 0
#define ERROR -1
#define TWISTER_SISTER 1
#define TWISTER 2
#define PISTOL 3
#define TWISTER_MAX_BACK_LENGTH 8
#define PISTOL_MAX_FRONT_LENGTH 10
class ribozyme_calculator
{
public:
   std::string Gen_Regex_Pattern(std::string TarRNA);
   std::string Gen_cDNA(std::string Ribozyme);
   std::string Calculate_GC_Percent(std::string Ribozyme);
   std::string Calculate_TM(std::string Ribozyme);
   int Calculate(std::string DNASeq, std::string TarRNA,std::string ZymeType,
                  std::vector<std::vector<std::string>>& CalculateResult);
 private:
   void CalculateTwisterSister(std::string& MatchRNA,std::smatch SubRegexResult,std::string& Ribozyme);
   void CalculateTwister(std::string& MatchRNA,std::smatch SubRegexResult,std::string& Ribozyme);
   void CalculatePistol(std::string& MatchRNA,std::string& Ribozyme);
   void CalculateRibozymeParas(std::string MatchRNA,std::string Ribozyme,unsigned int MatchBeginPos,unsigned int MatchEndPos
                               ,std::vector<std::string>& CalculateResultItem);
};

#endif // RIBOZYME_CALCULATOR_H
