#include"RibozymeCalculator.h"
using namespace std;
extern unsigned int PistolGTBackPos;
extern unsigned int HammerTCBackPos;
string RibozymeCalculator::GenRegexPattern(string OldTarRNA)
{
    string RegexPattern,TarRNA;
    for(auto it=OldTarRNA.begin();it!=OldTarRNA.end();++it)
    {
       if(*it=='A' || *it=='G' || *it=='C' || *it=='T' || *it=='N')
       {
           TarRNA.push_back(*it);
       }
    }
    int count = 0;
    for (unsigned int i = 0; i < TarRNA.length(); i++)
    {
        if (TarRNA[i] == 'N')
        {
            count++;
            continue;
        }
        else if (TarRNA[i] == 'A' || TarRNA[i] == 'G' || TarRNA[i] == 'C' || TarRNA[i] == 'T')
        {
            if (count)
            {
                string TempPattern = "[AGCT]{" + to_string(count) +"}" + TarRNA[i];
                RegexPattern += TempPattern;
                count = 0;
            }
            else
            {
                RegexPattern += TarRNA[i];
            }
        }
        else
        {
            qDebug() << "Invalid Target RNA" << endl;
            return "";
        }
    }
    if (count)
    {
        string TempPattern = "[AGCT]{" + to_string(count) +"}";
        RegexPattern += TempPattern;
    }
    return RegexPattern;
}
string RibozymeCalculator:: GenCDNA(string Ribozyme)
{
    string cDNA;
    for (auto it = Ribozyme.begin(); it != Ribozyme.end(); ++it){
        cDNA.push_back(GenDNAByRNA(*it));
    }
    string T7Template = "CGCTATAGTGAGTCGTATTACC";
    cDNA += T7Template;
    return cDNA;
}
string RibozymeCalculator::CalculateGCPercent(string Ribozyme)
{
    double count=0;
    for(unsigned int i=0;i<Ribozyme.length();++i)
    {
        if(Ribozyme[i]=='G' || Ribozyme[i]=='C')
        {
            count++;
        }
    }
    return to_string(count/double(Ribozyme.length())*100);
}
string RibozymeCalculator::CalculateTM(string Ribozyme,string ZymeType)
{
    double ANum=0,GNum=0,CNum=0,TNum=0;
    for(auto it=Ribozyme.begin();it!=Ribozyme.end();++it){
        switch (*it) {
        case 'A':
            ANum++;
            break;
        case 'G':
            GNum++;
            break;
        case 'C':
            CNum++;
            break;
        case 'T':
            TNum++;
            break;
        default:
            break;
        }
    }
    if(ZymeType==PISTOL){
        GNum--;
        TNum--;
    }
    double Mole=(GNum+CNum)/(Ribozyme.length()-2);
    //79.8 + 18.5*log10([Na+]) + (58.4 * (yG+zC)/(wA+xT+yG+zC)) + (11.8 * ((yG+zC)/(wA+xT+yG+zC))2) - (820/(wA+xT+yG+zC))
    return to_string(79.8+18.5*qLn(0.05)/qLn(10)+(58.4*Mole)
                     +11.8*pow(Mole,2)-820/(Ribozyme.length()-2));
}
void RibozymeCalculator::CalculateTwisterSister(string& MatchRNA, smatch SubRegexResult, string& Ribozyme)
{
    Ribozyme.clear();
    for (auto it = MatchRNA.begin(); it != MatchRNA.end(); ++it)
    {
        if (it == SubRegexResult[0].first - 4)
        {
            string tmp = "CAGUCCCGUGCAAGCCGGGAC";
            reverse(tmp.begin(), tmp.end());
            Ribozyme += tmp;
        }
        else if (it == SubRegexResult[0].first || it == SubRegexResult[0].first + 4
            || it == SubRegexResult[0].first + 5)
        {
            continue;
        }
        else if (it == SubRegexResult[0].first + 3)
        {
            Ribozyme += "AC";
        }
        else
        {
            Ribozyme.push_back(GenRNAByDNA(*it));
        }
    }
}
void RibozymeCalculator::CalculateRibozymeParas(string& MatchRNA, string& Ribozyme, unsigned int MatchBeginPos,
                                                unsigned int MatchEndPos,string& ZymeType, vector<string> &CalculateResultItem)
{
    string tmp=Ribozyme;
    string cDNA = GenCDNA(Ribozyme);
    CalculateResultItem.push_back(MatchRNA);
    CalculateResultItem.push_back(to_string(MatchBeginPos));
    CalculateResultItem.push_back(to_string(MatchEndPos));
    reverse(tmp.begin(),tmp.end());
    CalculateResultItem.push_back(tmp);
    CalculateResultItem.push_back(CalculateGCPercent(Ribozyme)+"%");//GC
    CalculateResultItem.push_back(CalculateTM(MatchRNA,ZymeType));//TM
    CalculateResultItem.push_back(cDNA);
}
void RibozymeCalculator::CalculatePistol(string &MatchRNA, string &Ribozyme)
{
   Ribozyme.clear();
   for(auto it=MatchRNA.begin();it!=MatchRNA.end();++it){
       if(it==MatchRNA.begin()+PistolGTBeginPos){
           Ribozyme+="GCGAAUCCCGAAUU";
       }
       else if(it==MatchRNA.begin()+PistolGTBeginPos+1){
           continue;
       }
       else{
           Ribozyme.push_back(GenRNAByDNA(*it));
       }
  }
  Ribozyme+="GAUAAAUUGCACCGGGAUUGGUGC";
}
void RibozymeCalculator::CalculateHammer(string &MatchRNA, string &Ribozyme)
{
    Ribozyme.clear();
    for(auto it=MatchRNA.begin();it!=MatchRNA.end();++it){
        if(it==MatchRNA.begin()+HammerTCBeginPos+1){
            Ribozyme+="AAGCAGGUCAGAGUAGUC";
        }
        else{
            Ribozyme.push_back(GenRNAByDNA(*it));
        }
   }
   Ribozyme+="UAAAAUAUCUGAGGACCAACAGAA";
}
void RibozymeCalculator::CalculateTwister(string& MatchRNA,smatch SubRegexResult,string& Ribozyme)
{
    Ribozyme.clear();
    for (auto it = MatchRNA.begin(); it != MatchRNA.end(); ++it)
    {
        if (it == SubRegexResult[0].first)
        {
            Ribozyme += "GGGAG";
        }
        else if (it == SubRegexResult[0].first+7)
        {
            Ribozyme += "AAAUAGGCCCGAACCCUGGCC";
        }
        else if (it == SubRegexResult[0].first+1 || it == SubRegexResult[0].first + 2
            || it == SubRegexResult[0].first+8)
        {
            continue;
        }
        else
        {
            Ribozyme.push_back(GenRNAByDNA(*it));
        }
    }
}
int RibozymeCalculator::Calculate(string OldDNASeq, string TarRNA,string ZymeType,std::vector<std::vector<std::string>>& CalculateResult)
{
    string DNASeq;
    CalculateResult.clear();
    int iRet=SUCEESS;
    do
    {
       if(OldDNASeq.empty() || TarRNA.empty()) {
         iRet=ERROR;
         break;
       }
       for(auto it=OldDNASeq.begin();it!=OldDNASeq.end();++it){
         if(*it=='A' || *it=='G' || *it=='C' || *it=='T'){
           DNASeq.push_back(*it);
         }
       }
        string RegexPattern = GenRegexPattern(TarRNA);
        if (RegexPattern.empty()){
            iRet=ERROR;
            break;
        }
        if(ZymeType==PISTOL){
            PistolGTBeginPos=TarRNA.find("GT");
            PistolGTBackPos=TarRNA.length()-PistolGTBeginPos-1;
        }
        else if(ZymeType==HAMMER_HEAD){
            HammerTCBeginPos=TarRNA.find("TC");
            HammerTCBackPos=TarRNA.length()-HammerTCBeginPos-1;
        }
        regex Pattern(RegexPattern);
        string::const_iterator iterStart = DNASeq.begin();
        string::const_iterator iterEnd = DNASeq.end();
        smatch RegexResult;
        string MatchRNA;
        while (regex_search(iterStart, iterEnd, RegexResult, Pattern)) {
            MatchRNA = RegexResult[0];
            vector<string> CalculateResultItem;
            unsigned int MatchBeginPos=RegexResult[0].first-DNASeq.begin();
            unsigned int MatchEndPos=RegexResult[0].second-DNASeq.begin();
            regex SubPattern;
            if(ZymeType==TWISTER_SISTER){
                   SubPattern="GCT[AGCT]A[AGCT]"; //  ("[A,G,C,T]AA[A,G,C,T]{4,4}GC");
            }
            else if(ZymeType==TWISTER){
                SubPattern="[AGCT]AA[AGCT]{4}GC";
            }
            else if(ZymeType==PISTOL){
                SubPattern="GT";
            }
            else if(ZymeType==HAMMER_HEAD){
                SubPattern="TC";
            }
            string::const_iterator SubPatternBegin = MatchRNA.begin();
            string::const_iterator SubPatternEnd = MatchRNA.end();
            smatch SubRegexResult;
            if (regex_search(SubPatternBegin, SubPatternEnd, SubRegexResult, SubPattern))
            {
                string Ribozyme;
                if(ZymeType==TWISTER_SISTER){
                    CalculateTwisterSister(MatchRNA,SubRegexResult,Ribozyme);
                    CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,CalculateResultItem);
                    CalculateResult.push_back(CalculateResultItem);
                }
                else if(ZymeType==TWISTER){
                    CalculateTwister(MatchRNA,SubRegexResult,Ribozyme);
                    CalculateResultItem.clear();
                    CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,CalculateResultItem);
                    CalculateResult.push_back(CalculateResultItem);
                    for(unsigned int i=1;i<=TWISTER_MAX_BACK_LENGTH;++i){
                            if(MatchEndPos+i<DNASeq.length()){ //防止向后越界
                                MatchRNA.push_back(DNASeq[MatchEndPos+i-1]);
                                Ribozyme.push_back(GenRNAByDNA(MatchRNA.back()));
                            }
                            else{
                                break;
                            }
                            CalculateResultItem.clear();
                            CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos+i,ZymeType,CalculateResultItem);
                            CalculateResult.push_back(CalculateResultItem);
                        }                
                }
                else if(ZymeType==PISTOL){
                    CalculatePistol(MatchRNA,Ribozyme);
                    CalculateResultItem.clear();
                    CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,CalculateResultItem);
                    CalculateResult.push_back(CalculateResultItem);
                    for(int i=1;i<=PISTOL_MAX_FRONT_LENGTH;++i){
                            if((int)MatchBeginPos-i>=0){  //防止向前越界
                               MatchRNA.insert(0,1,DNASeq[MatchBeginPos-i]);
                               Ribozyme.insert(0,1,GenRNAByDNA(MatchRNA.front()));
                            }
                            else{
                               break;
                            }
                            CalculateResultItem.clear();
                            CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos-i+1,MatchEndPos,ZymeType,CalculateResultItem);
                            CalculateResult.push_back(CalculateResultItem);
                        }
                }
                else if(ZymeType==HAMMER_HEAD){
                    CalculateHammer(MatchRNA,Ribozyme);
                    CalculateResultItem.clear();
                    CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,CalculateResultItem);
                    CalculateResult.push_back(CalculateResultItem);
                    for(int i=1;i<=HAMMER_MAX_FRONT_LENGTH;++i){
                            if((int)MatchBeginPos-i>=0){  //防止向前越界
                               MatchRNA.insert(0,1,DNASeq[MatchBeginPos-i]);
                               Ribozyme.insert(0,1,GenRNAByDNA(MatchRNA.front()));
                            }
                            else{
                               break;
                            }
                            CalculateResultItem.clear();
                            CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos-i+1,MatchEndPos,ZymeType,CalculateResultItem);
                            CalculateResult.push_back(CalculateResultItem);
                        }
                }
            if(ZymeType!=PISTOL && ZymeType!=HAMMER_HEAD){//更新搜索起始位置,搜索剩下的字符串
                iterStart = RegexResult[0].second;
            }
            else{
                iterStart=RegexResult[0].first+2;
            }
        }
      }
    } while (false);
    return iRet;
}
