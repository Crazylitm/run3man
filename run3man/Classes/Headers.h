//
//  Headers.h
//  run3man
//
//  Created by crazywork on 14-3-2.
//
//

#ifndef run3man_Headers_h
#define run3man_Headers_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Dlog.h"
#include <vector>
#include <string>

#include <Box2D/Box2D.h>
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;

const int ROLE_ZORDER = 10;
const int GOLD_PARTICLE_BATCH_NODE_TAG = 1001;


//创建一个金币的结构体
struct GOLD_STRUCT{
    CCSprite* goldSprint;//金币的精灵
    int goldValues;//金币的分值
};

/*
inline CCPoint operator+ (CCPoint p1,CCPoint p2){
    
    return ccp(p1.x+p2.x,p1.y+p2.y);
};

inline CCPoint operator- (CCPoint p1,CCPoint p2){
    
    return ccp(p1.x-p2.x,p1.y-p2.y);
}*/
#endif
