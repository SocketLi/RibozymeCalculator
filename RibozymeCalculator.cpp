#include"RibozymeCalculator.h"
using namespace std;
extern unsigned int PistolGTBackPos;
extern unsigned int HammerTCBackPos;
RibozymeCalculator::RibozymeCalculator()
{
    PistolGTBeginPos=HammerTCBeginPos=TwisterBeginPos=0;
    NeighborDH["AA"]=NeighborDH["UU"]=-6.82;
    NeighborDS["AA"]=NeighborDS["UU"]=-19.0;
    NeighborDH["AG"]=NeighborDH["CU"]=-10.48;
    NeighborDS["AG"]=NeighborDS["CU"]=-27.1;
    NeighborDH["AC"]=NeighborDH["GU"]=-11.40;
    NeighborDS["AC"]=NeighborDS["GU"]=-29.5;
    NeighborDH["AU"]=-9.38;
    NeighborDS["AU"]=-26.7;
    NeighborDH["GA"]=NeighborDH["UC"]=-12.44;
    NeighborDS["GA"]=NeighborDS["UC"]=-32.5;
    NeighborDH["GG"]=NeighborDH["CC"]=-13.39;
    NeighborDS["GG"]=NeighborDS["CC"]=-32.7;
    NeighborDH["GC"]=-14.88;
    NeighborDS["GC"]=-36.9;
    NeighborDH["CA"]=NeighborDH["UG"]=-10.44;
    NeighborDS["CA"]=NeighborDS["UG"]=-26.9;
    NeighborDH["CG"]=-10.64;
    NeighborDS["CG"]=-26.7;
    NeighborDH["UA"]=-7.69;
    NeighborDS["UA"]=-20.5;
}

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
string* RibozymeCalculator::ProcessRibozyme(const string& MatchRNA,const string& ZymeType,const smatch& SubRegexResult)
{
    string* RNAseq=new string(MatchRNA);
    if(ZymeType==TWISTER_SISTER){
        unsigned int BeginPos=SubRegexResult[0].first-MatchRNA.begin();
        RNAseq->erase(BeginPos-4,1);
        RNAseq->erase(BeginPos-1,1);
        RNAseq->erase(BeginPos+3-2,1);
        RNAseq->erase(BeginPos+4-3,1);
        RNAseq->erase(BeginPos+5-4,1);
    }
    else if(ZymeType==TWISTER){
        RNAseq->erase(TwisterBeginPos,1);
        RNAseq->erase(TwisterBeginPos+1-1,1);
        RNAseq->erase(TwisterBeginPos+2-2,1);
        RNAseq->erase(TwisterBeginPos+7-3,1);
        RNAseq->erase(TwisterBeginPos+8-4,1);
    }
    else if(ZymeType==PISTOL){
        RNAseq->erase(PistolGTBeginPos,1);
        RNAseq->erase(PistolGTBeginPos,1);
    }
    else if(ZymeType==HAMMER_HEAD){
        RNAseq->erase(HammerTCBeginPos+1,1);
    }
    return RNAseq;
}
void RibozymeCalculator::CalculateTM(const string& MatchRNA,const string& ZymeType,const smatch& SubRegexResult,vector<string> &CalculateResultItem)
{
    double GNum=0,CNum=0,InitDH=3.61,InitDS=-1.5,TerminalAUDH=3.72,TerminalAUDS=10.5;
    double SumDH=0,SumDS=0,TerminalAUNum=0;
    string* CalculateSeq=ProcessRibozyme(MatchRNA,ZymeType,SubRegexResult);
    string SlideWindow;
    for(unsigned int i=0;i<CalculateSeq->length()-1;++i){
        SlideWindow.clear();
        SlideWindow.push_back(CalculateSeq->at(i));
        SlideWindow.push_back(CalculateSeq->at(i+1));
        SumDH+=NeighborDH[SlideWindow];
        SumDS+=NeighborDS[SlideWindow];
        if(CalculateSeq->at(i)=='G'){
            GNum++;
        }
        else if(CalculateSeq->at(i)=='C'){
            CNum++;
        }
    }
    if(SlideWindow[1]=='G'){
        GNum++;
    }
    else if(SlideWindow[1]=='C'){
        CNum++;
    }
    if((CalculateSeq->at(0)=='A' || CalculateSeq->at(0)=='U') && (CalculateSeq->at(CalculateSeq->length()-1)=='A' || CalculateSeq->at(CalculateSeq->length()-1)=='U')){
        TerminalAUNum=2;
    }
    else if((CalculateSeq->at(0)=='A' || CalculateSeq->at(0)=='U') || (CalculateSeq->at(CalculateSeq->length()-1)=='A' || CalculateSeq->at(CalculateSeq->length()-1)=='U')){
        TerminalAUNum=1;
    }
    SumDH+=InitDH+TerminalAUDH*TerminalAUNum;
    SumDS+=InitDS+TerminalAUDS*TerminalAUNum;
    double OneMolTM=(SumDH*1000)/(SumDS+1.987*qLn(5*pow(10,-8)/4));
    double TM=1/OneMolTM+((4.29*(GNum+CNum)/CalculateSeq->length()-3.95)*qLn(0.05)+0.94*pow(qLn(0.05),2))*pow(10,-5);
    double TMResult=1/TM-273.15;
    CalculateResultItem.push_back(*CalculateSeq);
    if(TMResult<0){
        CalculateResultItem.push_back(to_string(0));
    }
    else{
        CalculateResultItem.push_back(to_string(TMResult));
    }
    delete CalculateSeq;
    return;
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
                                                unsigned int MatchEndPos,string& ZymeType, const smatch& SubRegexResult,vector<string> &CalculateResultItem)
{
    string tmp=Ribozyme;
    string cDNA = GenCDNA(Ribozyme);
    replace(begin(MatchRNA),end(MatchRNA),'T','U');
    CalculateResultItem.push_back(MatchRNA);
    CalculateResultItem.push_back(to_string(MatchBeginPos));
    CalculateResultItem.push_back(to_string(MatchEndPos));
    reverse(tmp.begin(),tmp.end());
    CalculateResultItem.push_back(tmp);
    CalculateResultItem.push_back(CalculateGCPercent(Ribozyme)+"%");//GC
    CalculateTM(MatchRNA,ZymeType,SubRegexResult,CalculateResultItem);//TM
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
int RibozymeCalculator::Calculate(string OldDNASeq, string TarRNA,string ZymeType,vector<vector<string>>& CalculateResult)
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
            smatch SubRegexResult;
            string Ribozyme;
            if(ZymeType==TWISTER_SISTER || ZymeType==TWISTER){
                string::const_iterator SubPatternBegin = MatchRNA.begin();
                string::const_iterator SubPatternEnd = MatchRNA.end();

                if(ZymeType==TWISTER_SISTER){
                    SubPattern="GCT[AGCT]A[AGCT]";
                }
                else if(ZymeType==TWISTER){
                    SubPattern="[AGCT]AA[AGCT]{4}GC";
                }
                if (regex_search(SubPatternBegin, SubPatternEnd, SubRegexResult, SubPattern)){
                    if(ZymeType==TWISTER_SISTER){
                        CalculateTwisterSister(MatchRNA,SubRegexResult,Ribozyme);
                        CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,SubRegexResult,CalculateResultItem);
                        CalculateResult.push_back(CalculateResultItem);
                    }
                    else if(ZymeType==TWISTER){
                        CalculateTwister(MatchRNA,SubRegexResult,Ribozyme);
                        CalculateResultItem.clear();
                        TwisterBeginPos=SubRegexResult[0].first-MatchRNA.begin();
                        CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,SubRegexResult,CalculateResultItem);
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
                                CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos+i,ZymeType,SubRegexResult,CalculateResultItem);
                                CalculateResult.push_back(CalculateResultItem);
                            }
                        }
                     }
                     iterStart = RegexResult[0].second;
                }
                else if(ZymeType==PISTOL){
                    CalculatePistol(MatchRNA,Ribozyme);
                    CalculateResultItem.clear();
                    CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,SubRegexResult,CalculateResultItem);
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
                            CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos-i+1,MatchEndPos,ZymeType,SubRegexResult,CalculateResultItem);
                            CalculateResult.push_back(CalculateResultItem);
                        }
                     iterStart=RegexResult[0].first+2;
                }
                else if(ZymeType==HAMMER_HEAD){
                    CalculateHammer(MatchRNA,Ribozyme);
                    CalculateResultItem.clear();
                    CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,SubRegexResult,CalculateResultItem);
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
                            CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos-i+1,MatchEndPos,ZymeType,SubRegexResult,CalculateResultItem);
                            CalculateResult.push_back(CalculateResultItem);
                        }
                     iterStart=RegexResult[0].first+2;
                }
      }
    } while (false);
    return iRet;
}
