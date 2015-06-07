//
//  Role.cpp
//  run3man
//
//  Created by crazywork on 14-3-2.
//
//

#include "Role.h"
#define PTM_RATIO 1

Role::Role():xSpeed(5),ySpeed(30),constXspend(xSpeed),constYspeed(ySpeed),acceslate(0.94) {
    
    m_particleBatchNode = CCParticleBatchNode::create("goldeffect.png");
    
    CC_SAFE_RETAIN(m_particleBatchNode);
}
Role::~Role(){
    CC_SAFE_RELEASE(m_particleBatchNode);
}
bool Role::init(){

    return true;
}
void Role::extraInit(){
    CCTMXObjectGroup* group = getGameMap()->objectGroupNamed( "Role" );
    
    CCDictionary*   dic = group->objectNamed( "player" );
    
    float x = dic->valueForKey( "x" )->floatValue();
    float y = dic->valueForKey( "y" )->floatValue();
    
    /////////////////////////////
    
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("runman0.png","runman0.plist","runman.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("runman1.png","runman1.plist","runman.ExportJson");
    //CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("runman2.png","runman2.plist","runman.ExportJson");
    //CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("runmanman3.png","runman3.plist","runman.ExportJson");
	mSprite = CCArmature::create("runman");
	mSprite->getAnimation()->playByIndex(0);
	
    //mSprite = CCSprite::create("Icon-72.png");
    mSprite->setAnchorPoint(ccp(0.5f,0.0f));
    mSprite->setPosition(ccp(x,y));
    Dlog::showLog("x=%f,y=%f",x,y);
	getGameMap()->addChild(mSprite,ROLE_ZORDER);
    
    getGameMap()->addChild(m_particleBatchNode,GOLD_PARTICLE_BATCH_NODE_TAG);
    setCurrentRoleState(ROLE_RUN);
    
    setMapPointByRole();
    
    //add litm 2014125 把之前自己计算小人跑步的碰撞运算函数替换成由box2d引擎来完成
    //this->schedule(schedule_selector( Role::role_update), 0.016f);
    
    initBox2d();
    
    this->schedule(schedule_selector(Role::camera_update), 0.016f);
}

void Role::setCurrentRolePoint(CCPoint point){
    mSprite->setPosition(point);
}
CCPoint Role::getCurrentRolePoint(){
    return mSprite->getPosition();
}
Role* Role::CreateWithTMX(CCTMXTiledMap* _map){
    Role* role = new Role();
    if (role && role->init()) {
        
        role->setGameMap(_map);
        role->extraInit();
        
        role->autorelease() ;
        
        return role;
    }
    
    CC_SAFE_DELETE(role);
    return NULL;
}
void Role::role_update(float dt ){
    
    role_logic();
}

void Role::runlogic(){
    
    
    /*
     
     1、先判断向右方向的跑动的碰撞
     2、再判断向下方向的判断的碰撞
     
     
     */
    
    if (isCollWhitCollArea(COLL_RIGHT)) {
        //如果与右侧碰撞了，说明人物碰到墙了，状态转换成stand by
        
        this->setCurrentRoleState(ROLE_STAND_BY);
        Dlog::showLog("跳转成撞墙状态");
        return;
        
    }
    if (!isCollWhitCollArea(COLL_BOTTOM)) {
        //如果不与地面接触则转换成下落状态
        isCollWhitCollArea(COLL_BOTTOM);
        this->setCurrentRoleState(ROLE_JUMP_DOWN);
        Dlog::showLog("跳转成下落状态");
        return;
    }
    

    
   // Dlog::showLog("runlogic");
    CCPoint preRolePoint = getCurrentRolePoint();
    
    CCPoint targetRolePoint =ccp(preRolePoint.x+xSpeed,preRolePoint.y);
    
    this->setCurrentRolePoint(targetRolePoint);
    
}
/**
 修复碰撞
 **/
void Role::fixColl(COLL_STATE _collState,CCRect box){
    /**
     1、算出相交的矩形
     2、根据方向将人物拉回到应有的位置上
     **/
    CCRect roleBox = mSprite->boundingBox() ;
    
    float maxX = roleBox.getMaxX() > box.getMaxX() ? box.getMaxX() : roleBox.getMaxX();
    float minX = roleBox.getMinX() > box.getMinX() ? roleBox.getMinX() : box.getMinX();
    
    float maxY = roleBox.getMaxY() > box.getMaxY() ? box.getMaxY() : roleBox.getMaxY();
    float minY = roleBox.getMinY() > box.getMinY() ? roleBox.getMinY() :box.getMaxY();
    
    float width = maxX - minX;
    float height = maxY - minY;
    
    //CCRect inBox = CCRectMake(minX, maxY, width, height);
    
    if (_collState == COLL_BOTTOM) {
        mSprite->setPositionY(mSprite->getPositionY() + height);
        return;
    }
    
    if (_collState == COLL_TOP) {
        mSprite->setPositionY(mSprite->getPositionY() - height);
        return;
    }
    
    if (_collState == COLL_LEFT) {
        mSprite->setPositionX(mSprite->getPositionX() + width);
        return;
    }
    
    if (_collState == COLL_RIGHT) {
        mSprite->setPositionX(mSprite->getPositionX() - width);
        return;
    }
    
    
}
void Role::jumpDownLogic(){
 /*
  1、判断与地面的碰撞顺序
  2、判断与X方向
  */
    if (isCollWhitCollArea(COLL_BOTTOM)) {
        ySpeed = constYspeed;
        
        setCurrentRoleState(ROLE_RUN);
        return;
    }
    
    CCPoint roleCurrentPoint = getCurrentRolePoint();
    
    ySpeed +=acceslate;
    
    if (ySpeed >=15) {
        ySpeed = 15;
    }
    CCPoint rlt = ccp(xSpeed,-ySpeed);
    
    this->setCurrentRolePoint(roleCurrentPoint + rlt);
    
    
    
}
/*
 跳起状态
 */
void Role::jumpUpLogic(){
    
    if (ySpeed <= 0.0) {
        ySpeed = 0.0;
    }
    if (ySpeed == 0.0) {
        //转为下落状态
        setCurrentRoleState(ROLE_JUMP_DOWN);
        Dlog::showLog("人物转为下落状态");
        return;
    }
    CCPoint roleCurrentPoint = getCurrentRolePoint();
    
    float xValue = xSpeed + roleCurrentPoint.x;
    
    ySpeed -= acceslate;
    
    float yValue = ySpeed + roleCurrentPoint.y;
    
    this->setCurrentRolePoint(ccp(xValue,yValue));
    
}

void Role::setMapPointByRole(){
    
    CCSize visableSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSize mapTiledNum = getGameMap()->getMapSize();
    CCSize tiledSize = getGameMap()->getTileSize();
    
    CCSize mapSize = CCSizeMake(mapTiledNum.width*tiledSize.width, mapTiledNum.height*tiledSize.height);
    
    CCPoint rolePoint = mSprite->getPosition();
    
    float x = rolePoint.x > visableSize.width/4 ? rolePoint.x : visableSize.width/4;
    
    x = x < mapSize.width - visableSize.width/2 ? x: mapSize.width - visableSize.width/2;
    
    float y = rolePoint.y > visableSize.height/4 ? rolePoint.y : visableSize.height/4;
    y = y < mapSize.height /2 ? y : mapSize.height/2;
    
    CCPoint desPoint = ccp(x,y);//目标点
    
    //CCPoint centerPoint = ccp(visableSize.width/5 ,visableSize.height/4);
    
    CCPoint centerPoint = ccp(visableSize.width/4 ,visableSize.height/4);
    
    CCPoint rtlPoint = ccpSub(centerPoint, desPoint);
    
    getGameMap()->setPosition(rtlPoint);
    
    
    
}

void Role::collistionWithGold(float dt){
    for (unsigned int i=0;i<goldVector.size(); i++) {
        GOLD_STRUCT gs = goldVector[i];
        if (gs.goldSprint != NULL &&
            mSprite->boundingBox().intersectsRect(gs.goldSprint->boundingBox())) {
            
            //播放金币碰撞的粒子效果
            CCPoint point = gs.goldSprint->getPosition();
            //CCQuadParticleSystems* mParticle = CCQuadParticleSystems::create("goldeffect.plist");
            
            CCParticleSystemQuad* mParticle = CCParticleSystemQuad::create("goldeffect.plist");
            
            mParticle->setPosition(point);
            mParticle->setAutoRemoveOnFinish(true);//如果不设置的话，如果不是设置。lizi
            
                Dlog::showLog("与金币碰撞了");
                gs.goldSprint->removeFromParentAndCleanup(true);
                goldVector.erase(goldVector.begin() + i);
            
            m_particleBatchNode->addChild(mParticle);
            
        }
    }
}
void Role::onEnter(){
    CCNode::onEnter();
    this->schedule(schedule_selector(Role::collistionWithGold), 0.016f);
    
}
bool Role::CreateWorld(){
    /*注释掉：是因为需要建立更复杂的刚体碰撞方法，通过TMS读入
    CCSize screedSize = CCDirector::sharedDirector()->getVisibleSize();
    b2Vec2 gravity;
    gravity.Set(0.0f, 5.0f);
    m_world = new b2World(gravity);
    
    b2BodyDef grouddef;
    
    grouddef.position.Set(0, 0);
    
    b2Body *groundboy  = m_world->CreateBody(&grouddef);
    
    //b2PolygonShape groundshare;
    b2EdgeShape groundshape;
    groundshape.Set(
                    b2Vec2(0, 87/PT_RATIO),
                    b2Vec2(screedSize.width/PT_RATIO,87/PT_RATIO)
                    );
    
    
    groundboy->CreateFixture(&groundshape,0);
    
    
    this->schedule(schedule_selector(Role::tick), 0.016f);
    
    */
    
    //initBox2d();
    
    b2BodyDef body_def;
    body_def.type = b2_staticBody;
    body_def.position.SetZero();
    m_qiangpao_body = m_world->CreateBody(&body_def);
    
    // 找出阻挡区域所在的层
    CCTMXObjectGroup* group = getGameMap()->objectGroupNamed("col");//Group->object->polyline
    
    CCArray* array = group->getObjects();
    CCDictionary* dict;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(array, pObj)
    {
        dict = (CCDictionary*)pObj;
        if (!dict)
            continue;
        
        b2FixtureDef fixture_def;
        
        StaticBlockObject *sb_obj = new StaticBlockObject();
        
        sb_obj->density = 1.0f;
        sb_obj->friction = 0.2f;
        sb_obj->restitution = 0.f;
        
        // 读取所有形状的起始点
        float x = ((CCString*)dict->objectForKey("x"))->floatValue();
        float y = ((CCString*)dict->objectForKey("y"))->floatValue();
        
        b2Shape* shape = NULL;
        
        //多边形
        CCObject *polygon = dict->objectForKey("polygonPoints");
        if (polygon) {
            CCArray *polygon_points = (CCArray*)polygon;
            
            std::vector<b2Vec2> points;
            
            // 必须将所有读取的定点逆向，因为翻转y之后，三角形定点的顺序已经逆序了，构造b2PolygonShape会crash
            int c =polygon_points->count();
            points.resize(c);
            c--;
            
            CCDictionary* pt_dict;
            CCObject* obj = NULL;
            CCARRAY_FOREACH(polygon_points, obj)
            {
                pt_dict = (CCDictionary*)obj;
                
                if (!pt_dict) {
                    continue;
                }
                
                // 相对于起始点的偏移
                float offx = ((CCString*)pt_dict->objectForKey("x"))->floatValue();
                float offy = ((CCString*)pt_dict->objectForKey("y"))->floatValue();
                
                points[c--] = (b2Vec2((x + offx) / PTM_RATIO, (y-offy) / PTM_RATIO));
            }
            
            b2PolygonShape *ps = new b2PolygonShape();
            ps->Set(&points[0], points.size());
            fixture_def.shape = ps;
            
            shape = ps;
            
            sb_obj->shape = StaticBlockObject::ST_POLYGON;
        } else if (polygon = dict->objectForKey("polylinePoints")){
            CCArray *polyline_points = (CCArray*)polygon;
            
            std::vector<b2Vec2> points;
            
            CCDictionary* pt_dict;
            CCObject* obj = NULL;
            CCARRAY_FOREACH(polyline_points, obj)
            {
                pt_dict = (CCDictionary*)obj;
                
                if (!pt_dict) {
                    continue;
                }
                
                float offx = ((CCString*)pt_dict->objectForKey("x"))->floatValue();
                float offy = ((CCString*)pt_dict->objectForKey("y"))->floatValue();
                points.push_back(b2Vec2((x + offx) / PTM_RATIO, (y-offy) / PTM_RATIO));
            }
            
            b2ChainShape *ps = new b2ChainShape();
            ps->CreateChain(&points[0], points.size());
            fixture_def.shape = ps;
            
            shape = ps;
            
            sb_obj->shape = StaticBlockObject::ST_POLYGON;
        } else if (dict->objectForKey("ellipse")) {
            float width = ((CCString*)dict->objectForKey("width"))->floatValue();
            float height = ((CCString*)dict->objectForKey("height"))->floatValue();
            
            b2CircleShape *ps = new b2CircleShape;
            ps->m_p.Set((x+width/2) / PTM_RATIO, ((y+height/2)) / PTM_RATIO);
            ps->m_radius = width/2/PTM_RATIO;
            fixture_def.shape = ps;
            
            shape = ps;
            
            sb_obj->shape = StaticBlockObject::ST_CIRCLE;
        } else {
            float width = ((CCString*)dict->objectForKey("width"))->floatValue();
            float height = ((CCString*)dict->objectForKey("height"))->floatValue();
            
            b2PolygonShape *ps = new b2PolygonShape;
            ps->SetAsBox(width/2/PTM_RATIO, height/2/PTM_RATIO, b2Vec2((x+width/2)/PTM_RATIO, (y+height/2)/PTM_RATIO), 0);
            fixture_def.shape = ps;
            
            shape = ps;
            
            sb_obj->shape = StaticBlockObject::ST_POLYGON;
            
        }
        
        fixture_def.density = sb_obj->density;
        fixture_def.friction = sb_obj->friction;
        fixture_def.restitution = sb_obj->restitution;
        
        b2Fixture *fixture = m_qiangpao_body->CreateFixture(&fixture_def);
        sb_obj->fixture = fixture;
        
        if (shape) {
            delete shape;
            shape = NULL;
        }
        
        // Storage the Static block object.
        m_StaticBlockList.push_back(sb_obj);
    }
    addBodyForSprite();//将动态的任务sprite初始化到box2d世界
    this->schedule(schedule_selector(Role::update));
    return true;

}
//暂时无用
void Role::tick(float dt){
    //让世界向前走dt这么多时间
    m_world->Step(dt, 8, 6);
    //更新cocos2d世界的界面
    for (b2Body *b = m_world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetUserData()) {
            //userdata 表示每个刚体可以存放的一些私有数据  ccsprite self
            CCArmature *_sprite = (CCArmature *)b->GetUserData();
            
        }
    }
}

//用户判读与山体碰撞的逻辑函数
bool Role::isCollWhitCollArea(COLL_STATE _collState){
    
//    <objectgroup name="col" width="1000" height="200">
//    <object name="qiang1" x="1" y="6337" width="3200" height="67"/>
//    <object name="qiang2" x="3205" y="6340" width="429" height="52"/>
//    <object name="qiang3" x="3639" y="6339" width="1677" height="82"/>
//    <object name="qiang4" x="5314" y="6339">
//    <polygon points="0,0 82,-128 87,4"/>
//    </object>
//    <object name="qiang5" x="5404" y="6242" width="1380" height="91"/>
//    </objectgroup>
    
    bool bFlag = false;
    
    CCTMXObjectGroup* collGroup = getGameMap()->objectGroupNamed("col");
    
    CCObject *obj = NULL;

////#define CCARRAY_FOREACH(__array__, __object__)
//if ((__array__) && (__array__)->data->num > 0)
//for(CCObject** __arr__ = (__array__)->data->arr, **__end__ = (__array__)->data->arr + (__array__)->data->num-1;
//__arr__ <= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);
//__arr__++)
    
 //   CCARRAY_FOREACH(collGroup->getObjects(), obj){
    
    if ((collGroup->getObjects()) && (collGroup->getObjects())->data->num > 0)
        for(CCObject** __arr__ = (collGroup->getObjects())->data->arr,
            **__end__ = (collGroup->getObjects())->data->arr + (collGroup->getObjects())->data->num-1;
            
            __arr__ <= __end__ && (((obj) = *__arr__) != NULL/* || true*/);
            __arr__++)
        {
            
        CCDictionary *dic = (CCDictionary*)obj;
        
        int  itype = dic->valueForKey( "type" )->intValue();
            CCRect box;
        //判断碰撞的是立方的墙体，还是多边形墙体
        if (itype == 1) {
            float x = dic->valueForKey( "x" )->floatValue() ;
            float y = dic->valueForKey( "y" )->floatValue() ;
        
            float width = dic->valueForKey( "width")->floatValue();
            float height = dic->valueForKey( "height")->floatValue()    ;
        
           box = CCRectMake(x, y, width, height);
        }
        else if ( itype == 2){
            float x = dic->valueForKey( "x" )->floatValue() ;
            float y = dic->valueForKey( "y" )->floatValue() ;
            Dlog::showLog("x=%f,y=%f\n",x,y);
        }
        else{
            continue;
        }
        bool bFlag = false;
        switch (_collState) {
            case COLL_BOTTOM:
                bFlag = isCollistionWithBottom(box);
                break;
            case COLL_TOP:
                bFlag = isCollistionWithTop(box);
                break;
                
            case COLL_LEFT:
                bFlag = isCollistionWithLeft(box);
                break;
                
            case COLL_RIGHT:
                bFlag = isCollistionWithRight(box);
                break;
            default:
                break;
        }
        if(bFlag == true){
         //如果已经碰撞了，我们才碰撞修复
            fixColl(_collState, box);
            
            return bFlag;
        }
        
    }
    
    return bFlag;
}
bool Role::isCollistionWithBottom(CCRect box){
    CCRect roleBox = mSprite->boundingBox();
    
    CCPoint rolePoint = ccp(roleBox.getMidX(),roleBox.getMinY());
    
    //Dlog::showLog("rolePoint.x=%f,y=%f----box.x=%f,y=%f",rolePoint.x,rolePoint.y,box.getMinX(),box.getMaxY());
    return box.containsPoint(rolePoint);
    
}

bool Role::isCollistionWithTop(CCRect box){
    CCRect roleBox = mSprite->boundingBox();
    
    CCPoint rolePoint = ccp(roleBox.getMidX(),roleBox.getMaxY());
    //Dlog::showLog("rolePoint.x=%f,y=%f----box.x=%f,y=%f",rolePoint.x,rolePoint.y,box.getMinX(),box.getMaxY());
    
    return box.containsPoint(rolePoint);
    
}

bool Role::isCollistionWithRight(CCRect box){
    CCRect roleBox = mSprite->boundingBox();
    
    CCPoint rolePoint = ccp(roleBox.getMaxX(),roleBox.getMidY());
    //Dlog::showLog("rolePoint.x=%f,y=%f----box.x=%f,y=%f",rolePoint.x,rolePoint.y,box.getMinX(),box.getMaxY());
    
    return box.containsPoint(rolePoint);
    
}

bool Role::isCollistionWithLeft(CCRect box){
    //获取任务左边框中间点
    
    CCRect roleBox = mSprite->boundingBox();
    
    CCPoint rolePoint = ccp(roleBox.getMinX(),roleBox.getMidY());
    //Dlog::showLog("rolePoint.x=%f,y=%f----box.x=%f,y=%f",rolePoint.x,rolePoint.y,box.getMinX(),box.getMaxY());
    
    return box.containsPoint(rolePoint);
    
}
void Role::camera_update(float dt){
    setMapPointByRole();
}

void Role::role_logic(){
    
    switch (getCurrentRoleState()) {
        case ROLE_INVAILD_STATE:
           // Dlog::showLog("ROLE_INVAILD_STATE");
            break;
        case ROLE_RUN:
           // Dlog::showLog("ROLE_RUN");
            runlogic();
            break;
        case ROLE_JUMP_DOWN:
           // Dlog::showLog("ROLE_JUMP_DOWN");
            jumpDownLogic();
            break;
            
        case ROLE_JUMP_UP:
           // Dlog::showLog("ROLE_JUMP_UP");
            jumpUpLogic();
            break;
        case ROLE_STAND_BY:
            //Dlog::showLog("stand by");
            break;
        default:
            break;
    }
}

void Role::draw()
{
    CCNode::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    m_world->DrawDebugData();
    
    kmGLPopMatrix();
    
    CHECK_GL_ERROR_DEBUG();
}

void Role::update()
{
    //b2Vec2 vel;
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetUserData() != NULL) {
            //这边要进行xspeed的碰撞和运动监测
            if (b->GetUserData() == mSprite) {
                mSprite->setPositionX(b->GetPosition().x/PTM_RATIO);
                mSprite->setPositionY(b->GetPosition().y/PTM_RATIO);
            }
            //role_update(1);
            //vel =   b->GetLinearVelocity();
            //vel.Normalize();
            //vel.Set(10, 0);
        }
    }
    
    uint32 flags = 0;
    flags += 1            * b2Draw::e_shapeBit;
    flags += 1            * b2Draw::e_jointBit;
    flags += 1            * b2Draw::e_aabbBit;
    flags += 1            * b2Draw::e_pairBit;
    flags += 0            * b2Draw::e_centerOfMassBit;
    m_debugDraw.SetFlags(flags);
    Float32 dt = 1.0f / 60.0f;
    
    m_world->Step(dt, 8, 8);
    
    m_world->DrawDebugData();
    
    this->dealCollisions();
}

bool Role::initBox2d()
{
    CCSize screedSize = CCDirector::sharedDirector()->getVisibleSize();
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    m_world = new b2World(gravity);
    
    m_world->SetAllowSleeping(true);
    m_world->SetContinuousPhysics(true);
    m_world->SetContactListener(this);
    m_world->SetDebugDraw(&m_debugDraw);
    
    CreateWorld();
    
    //m_world->SetWarmStarting(true);
    //m_world->SetContinuousPhysics(true);
    //m_world->SetSubStepping(false);
    
}
void Role::dealCollisions()
{
    
}

void Role::addBodyForSprite()
{
    b2BodyDef spriteBodyDef;
    b2Vec2 vel;
    spriteBodyDef.type = b2_dynamicBody;
    spriteBodyDef.position.Set(mSprite->getPosition().x / PTM_RATIO, mSprite->getPosition().y / PTM_RATIO  );
    spriteBodyDef.userData = mSprite;
   // spriteBodyDef.linearDamping = 10.0f;

    m_spriteBody = m_world->CreateBody(&spriteBodyDef);
    
    vel.Set(100000, 0);
    m_spriteBody->SetLinearVelocity(vel);
    
    b2PolygonShape spriteShape;
    CCSize size = mSprite->getContentSize();
    
    spriteShape.SetAsBox(size.width / PTM_RATIO /2, size.height / PTM_RATIO / 2);
    
    b2FixtureDef spriteShapeDef;
    spriteShapeDef.shape = &spriteShape;
    spriteShapeDef.density = 1.0f;
    
    m_spriteBody->CreateFixture(&spriteShapeDef);
    //sprinteBody->CreateFixture(&sprinteBodyDef);
    
}

void Role::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    
}

