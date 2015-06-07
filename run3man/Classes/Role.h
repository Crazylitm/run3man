//
//  Role.h
//  run3man
//
//  Created by crazywork on 14-3-2.
//
//

#ifndef __run3man__Role__
#define __run3man__Role__

#include <iostream>
#include "Headers.h"

#include <Box2D/Box2D.h>
#include "iFixtureUserData.h"
#include "b2WorldCallbacks.h"
#include "GLES-Render.h"
enum ROLE_STATE {
    ROLE_INVAILD_STATE = 0,
    ROLE_RUN,
    ROLE_JUMP_UP,
    ROLE_JUMP_DOWN,
    ROLE_STAND_BY
    };
//用户判断碰撞方向的枚举
enum COLL_STATE {
    COLL_INVAILD_STATE = 0,
    COLL_TOP,
    COLL_BOTTOM,
    COLL_LEFT,
    COLL_RIGHT
    };

class Role :public CCNode,public b2ContactListener{
    
    
public:
    Role();
    ~Role();
    
    CCParticleBatchNode* m_particleBatchNode;
    
    virtual bool init();
    static Role* CreateWithTMX(CCTMXTiledMap* _map);
    CC_SYNTHESIZE(CCTMXTiledMap*, gameMap, GameMap);
    
    CC_PROPERTY(CCPoint, currentRolePoint, CurrentRolePoint);
    
    CC_SYNTHESIZE(ROLE_STATE, currentRoleState, CurrentRoleState);
    
    void role_update(float dt );
    
    void role_logic();
    
    CC_SYNTHESIZE(float, xSpeed, XSpeed);
    
    CC_SYNTHESIZE(float, ySpeed, YSpeed);
    
    CC_SYNTHESIZE(float, constXspend, ConstXspeed);
    
    CC_SYNTHESIZE(float, constYspeed, ConstYspeed);
    
    CC_SYNTHESIZE(std::vector<GOLD_STRUCT>, goldVector, GoldVector);//创建一个金币的vector；
    
    virtual void onEnter();
    
    void collistionWithGold(float dt);//判断余金币碰撞的逻辑
public:
    bool CreateWorld();
    void tick(float dt);
    
    virtual void BeginContact(b2Contact* contact) { B2_NOT_USED(contact); }
    virtual void EndContact(b2Contact* contact) { B2_NOT_USED(contact); }
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse)
    {
        B2_NOT_USED(contact);
        B2_NOT_USED(impulse);
    }
public:
    void draw();
    void update();
    bool initBox2d();
    void dealCollisions();
    void addBodyForSprite();
    GLESDebugDraw m_debugDraw;
private:
    CCArmature* mSprite;
    CCSprite* mSprite1;
    void extraInit();
    
    void runlogic();
    
    void jumpUpLogic();
    
    void jumpDownLogic();
    
    void fixColl(COLL_STATE _collState,CCRect box);
private:
    void setMapPointByRole();//根据人物的位置点设置镜头
    
    void camera_update(float dt);
    
    
private:
    //碰撞区检测函数
    bool isCollWhitCollArea(COLL_STATE _collState);
    
    bool isCollistionWithLeft(CCRect box);
    bool isCollistionWithRight(CCRect box);
    bool isCollistionWithTop(CCRect box);
    bool isCollistionWithBottom(CCRect box);
    
private:
    float acceslate;
    
private:
    //定义一个世界b2world
    b2World* m_world;
    b2Body*  m_qiangpao_body;
    b2Body*  m_spriteBody;
    StaticBlockList m_StaticBlockList;
};
#endif /* defined(__run3man__Role__) */
