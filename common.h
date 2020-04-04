#ifndef COMMON_H
#define COMMON_H
#include<string>
#include<regex>
#include<qDebug>
#define TWISTER_SISTER "Twister sister"
#define TWISTER "Twister"
#define PISTOL "Pistol"
#define HAMMER_HEAD "HammerHead"
#define DEBUG_WARN(x) qDebug()<<__FILE__<<" "<<__FUNCTION__<<" "<<__LINE__<<":"<<x

inline char GenRNAMatchBase(char Base){  //RNA->RNA
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

inline char GenDNAMatchBase(char Base){ //DNA->DNA
    switch (Base) {
    case 'A':
        return 'T';
    case 'G':
        return 'C';
    case 'C':
        return 'G';
    case 'T':
        return 'A';
    default:
        DEBUG_WARN("ERROR BASE\n");
        return 0;
    }
}
inline char GenRNAByDNA(char Base)      //DNA->RNA
{
    switch (Base) {
    case 'A':
        return 'U';
    case 'G':
        return 'C';
    case 'C':
        return 'G';
    case 'T':
        return 'A';
    default:
        DEBUG_WARN("ERROR BASE\n");
        return 0;
    }
}
inline char GenDNAByRNA(char Base)
{
    switch (Base) {
    case 'A':
        return 'T';
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
#endif // COMMON_H
