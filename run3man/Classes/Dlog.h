//
//  Dlog.h
//  run3man
//
//  Created by crazywork on 14-3-2.
//
//

#ifndef __run3man__Dlog__
#define __run3man__Dlog__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class Dlog {
    
    
public:

    static void showLog(const char* pszFormat,...);
};
#endif /* defined(__run3man__Dlog__) */
