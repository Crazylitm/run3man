//
//  GameFatherLayer.cpp
//  run3man
//
//  Created by crazywork on 14-3-2.
//
//

#include "GameFatherLayer.h"
const int GOLD_BATCH_NODE_TAG = 1000;
GameFatherLayer::GameFatherLayer(){
    CC_SAFE_RETAIN(m_GameMaps);
    _pGoldBatchNode = CCSpriteBatchNode::create("jinbi.png");
    CC_SAFE_RETAIN(_pGoldBatchNode);
}
GameFatherLayer::~GameFatherLayer(){
    CC_SAFE_RELEASE(m_GameMaps);
    CC_SAFE_RELEASE(_pGoldBatchNode);
}
bool GameFatherLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    initUI();
    initMap();
    initRole();
    initComponets();
    this->setTouchEnabled(true);
    return true;
    
}
void GameFatherLayer::initUI(){
    
}

void GameFatherLayer::dispatchTouchRoleMove(){
    if (mRole->getCurrentRoleState() == ROLE_RUN) {
        
        mRole->setCurrentRoleState(ROLE_JUMP_UP);
        
        return;
    }
}
void GameFatherLayer::initRole(){
    mRole = Role::CreateWithTMX(m_GameMaps);
    this->addChild(mRole);
}
void GameFatherLayer::initMap(){
    m_GameMaps = CCTMXTiledMap::create("paoku.tmx");
    this->addChild(m_GameMaps,1);
    
}
void GameFatherLayer::initComponets(){
    
    std::vector<GOLD_STRUCT> goldVector;
    
    m_GameMaps->addChild(_pGoldBatchNode,GOLD_BATCH_NODE_TAG);
    
    CCTMXObjectGroup* _pGroup = m_GameMaps->objectGroupNamed("items");
    
    CCObject* obj=NULL;
    
    //遍历所有items的对象层，获取gold对象的位置，然后将金币加入到游戏场景中。
    CCARRAY_FOREACH(_pGroup->getObjects(), obj){
        CCDictionary* dic = (CCDictionary*)obj;
        
        if(dic->valueForKey("name")->compare("gold") ==0){
            Dlog::showLog("加载gold");
            float x = dic->valueForKey("x")->floatValue();
            float y = dic->valueForKey("y")->floatValue();
            CCSprite* _gold = CCSprite::create("jinbi.png");
            _gold->setScale(0.3);
            _pGoldBatchNode->addChild(_gold);
            _gold->setPosition(ccp(x,y));
            
            GOLD_STRUCT _gs;
            _gs.goldSprint = _gold;
            
            //todo 以后根据配置文件或者plist文件加载不同的金币数目
            _gs.goldValues = 100;
            
            goldVector.push_back(_gs);
        }
    }
    mRole->setGoldVector(goldVector);
}
CCScene* GameFatherLayer::scene(){
    CCScene *sc = CCScene::create();
    sc->addChild(GameFatherLayer::create());
    
    
    return  sc;
}

bool GameFatherLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    dispatchTouchRoleMove();
    return true;
}
void GameFatherLayer::onEnter(){
    CCLayer::onEnter();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    
    
}
void GameFatherLayer::onExit(){
 
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCLayer::onExit();
}

