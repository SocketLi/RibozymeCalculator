#include"RibozymeCalculator.h"
using namespace std;
extern unsigned int PistolGTBackPos;
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
                string TempPattern = "[A,G,C,T]{" + to_string(count) +"}" + TarRNA[i];
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
        string TempPattern = "[A,G,C,T]{" + to_string(count) +"}";
        RegexPattern += TempPattern;
    }
    return RegexPattern;
}
string RibozymeCalculator:: GenCDNA(string Ribozyme)
{
    string cDNA;
    for (auto it = Ribozyme.begin(); it != Ribozyme.end(); ++it)
    {
        switch (*it)
        {
        case 'A':
            cDNA += 'T';
            break;
        case 'C':
            cDNA += 'G';
            break;
        case 'G':
            cDNA += 'C';
            break;
        case 'U':
            cDNA += 'A';
            break;
        default:
        {
            qDebug() << "unkown char" << endl;
            return "";
            break;
        }
        }
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
            switch (*it)
            {
            case 'A':
                Ribozyme += 'U';
                break;
            case 'C':
                Ribozyme += 'G';
                break;
            case 'G':
                Ribozyme += 'C';
                break;
            case 'T':
                Ribozyme += 'A';
                break;
            default:
            {
                qDebug() << "unkown char" << endl;
                break;
            }
        }
      }
    }
}
void RibozymeCalculator::CalculateRibozymeParas(string& MatchRNA, string& Ribozyme, unsigned int MatchBeginPos,
                                                unsigned int MatchEndPos,string& ZymeType, vector<string> &CalculateResultItem)
{
    string cDNA = GenCDNA(Ribozyme);
    CalculateResultItem.push_back(MatchRNA);
    CalculateResultItem.push_back(to_string(MatchBeginPos));
    CalculateResultItem.push_back(to_string(MatchEndPos));
    reverse(Ribozyme.begin(),Ribozyme.end());
    CalculateResultItem.push_back(Ribozyme);
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
           switch (*it)
           {
           case 'A':
               Ribozyme += 'U';
               break;
           case 'C':
               Ribozyme += 'G';
               break;
           case 'G':
               Ribozyme += 'C';
               break;
           case 'T':
               Ribozyme += 'A';
               break;
           default:
           {
               qDebug() << "unkown char" << endl;
               break;
           }
       }
   }
  }
  Ribozyme+="GAUAAAUUGCACCGGGAUUGGUGC";
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
            switch (*it)
            {
            case 'A':
                Ribozyme += 'U';
                break;
            case 'C':
                Ribozyme += 'G';
                break;
            case 'G':
                Ribozyme += 'C';
                break;
            case 'T':
                Ribozyme += 'A';
                break;
            default:
            {
                qDebug() << "unkown char" << endl;
                break;
            }
        }
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
        if(ZymeType=="Pistol"){
            unsigned int pos=TarRNA.find("GT");
            if(pos!=TarRNA.npos){
                PistolGTBeginPos=pos;
                PistolGTBackPos=TarRNA.length()-pos-1;
            }
            else{
                DEBUG_WARN("Invalid Pistol mark");
                iRet=ERROR;
                break;
            }
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
            if(ZymeType=="Twister sister"){
                   SubPattern="GCT[A,G,C,T]A[A,G,C,T]"; //  ("[A,G,C,T]AA[A,G,C,T]{4,4}GC");
            }
            else if(ZymeType=="Twister"){
                SubPattern="[A,G,C,T]AA[A,G,C,T]{4,4}GC";
            }
            else if(ZymeType=="Pistol"){
                SubPattern="GT";
            }
            string::const_iterator SubPatternBegin = MatchRNA.begin();
            string::const_iterator SubPatternEnd = MatchRNA.end();
            smatch SubRegexResult;
            if (regex_search(SubPatternBegin, SubPatternEnd, SubRegexResult, SubPattern))
            {
                string Ribozyme;
                if(ZymeType=="Twister sister"){
                    CalculateTwisterSister(MatchRNA,SubRegexResult,Ribozyme);
                    CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos,ZymeType,CalculateResultItem);
                    CalculateResult.push_back(CalculateResultItem);
                }
                else if(ZymeType=="Twister"){
                    for(unsigned int i=0;i<=TWISTER_MAX_BACK_LENGTH;++i){
                        if(i){
                            if(MatchEndPos+i<DNASeq.length()){ //防止向后越界
                                MatchRNA.push_back(DNASeq[MatchEndPos+i-1]);
                            }
                            else{
                                break;
                            }
                        }
                        else{
                            CalculateTwister(MatchRNA,SubRegexResult,Ribozyme);
                        }
                        CalculateResultItem.clear();
                   if(i) {
                     switch(MatchRNA.back()){
                        case 'A':
                           Ribozyme.push_back('U');
                           break;
                        case 'G':
                           Ribozyme.push_back('C');
                           break;
                        case 'C':
                           Ribozyme.push_back('G');
                           break;
                        case 'T':
                           Ribozyme.push_back('A');
                           break;
                        default:
                            break;
                        }
                     }
                        CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos+1,MatchEndPos+i,ZymeType,CalculateResultItem);
                        CalculateResult.push_back(CalculateResultItem);
                    }
                }
                else if(ZymeType=="Pistol"){
                    for(int i=0;i<=PISTOL_MAX_FRONT_LENGTH;++i){
                        if(i){
                            if((int)MatchBeginPos-i>=0){  //防止向前越界
                               MatchRNA.insert(0,1,DNASeq[MatchBeginPos-i]);
                            }
                            else{
                               break;
                            }
                        }
                        else{
                             CalculatePistol(MatchRNA,Ribozyme);
                        }
                        CalculateResultItem.clear();
                    if(i) {
                     switch(MatchRNA.front()){
                        case 'A':
                           Ribozyme.insert(0,1,'U');
                           break;
                        case 'G':
                           Ribozyme.insert(0,1,'C');
                           break;
                        case 'C':
                           Ribozyme.insert(0,1,'G');
                           break;
                        case 'T':
                           Ribozyme.insert(0,1,'A');
                           break;
                        default:
                            break;
                        }
                     }
                     CalculateRibozymeParas(MatchRNA,Ribozyme,MatchBeginPos-i+1,MatchEndPos,ZymeType,CalculateResultItem);
                     CalculateResult.push_back(CalculateResultItem);
                  }
                }
            }
            if(ZymeType!="Pistol"){
                iterStart = RegexResult[0].second;
            }
            else{
                iterStart=RegexResult[0].first+2;
            }                                  //更新搜索起始位置,搜索剩下的字符串
        }
    } while (false);
    return iRet;
}
