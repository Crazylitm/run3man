//
//  Dlog.cpp
//  run3man
//
//  Created by crazywork on 14-3-2.
//
//

#include "Dlog.h"
const int MaxLogLen=1000;
static bool isOpen = true;
void Dlog::showLog(const char* pszFormat,...){
    if(isOpen == false)
        return;
    printf("litm:");
    char szBuf[MaxLogLen];
    
    va_list ap;
    va_start(ap,pszFormat);
    vsnprintf(szBuf,MaxLogLen,pszFormat,ap );
    va_end(ap);
    printf("%s",szBuf);
    printf("\n");
}