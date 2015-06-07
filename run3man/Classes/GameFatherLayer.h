//
//  GameFatherLayer.h
//  run3man
//
//  Created by crazywork on 14-3-2.
//
//

#ifndef __run3man__GameFatherLayer__
#define __run3man__GameFatherLayer__

#include <iostream>
#include "Headers.h"
#include "Role.h"

class Role;

class GameFatherLayer :public CCLayer {
    
public:
    GameFatherLayer();
    ~GameFatherLayer();
    CREATE_FUNC(GameFatherLayer);
    virtual bool init();
    void initUI();
    static CCScene* scene();
    void initMap();
    void initRole();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    
    void dispatchTouchRoleMove();
    
    void initComponets();//初始化地图中的原件或者道具
private:
    CCTMXTiledMap* m_GameMaps;
    Role* mRole;
    CCSpriteBatchNode* _pGoldBatchNode;//金币的批处理渲染；
};
#endif /* defined(__run3man__GameFatherLayer__) */
