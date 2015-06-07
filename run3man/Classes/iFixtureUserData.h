/**
    Storage fixture user data.
    use for b2Fixture user data.
 */
class iFixtureUserData
{
public:
    typedef uint BodyType;
    typedef uint FixtureType;
    
    static const BodyType   BT_Avata = 0x000;  // no any use...
    static const FixtureType FT_None = 0x000;
    
    static const BodyType BT_Map  = 0x1000;
    static const FixtureType FT_STATIC_OBJ = 0x1F01;
    static const FixtureType FT_DYNAMIC_OBJ = 0x1F02;

    //
    static const BodyType BT_Role = 0x2000;
    static const BodyType BT_Bullet = 0x2100;
    
    static const FixtureType FT_BODY = 0x2F01;
    static const FixtureType FT_FOOT = 0x2F02;

    
public:
    iFixtureUserData(BodyType body_type, FixtureType fixture_type):
        mBodyType(body_type), mFixtureType(fixture_type){
    }
    
    virtual ~iFixtureUserData() {
        
    }

    inline BodyType getBodyType() { return mBodyType; }
    inline FixtureType getFixtureType() { return mFixtureType; }
    
protected:
    BodyType mBodyType;
    FixtureType mFixtureType;
};

/**
 Block object.
 specify a block area in physics engine. */
class StaticBlockObject : public iFixtureUserData {
public:
    StaticBlockObject():
    iFixtureUserData(BT_Map, FT_STATIC_OBJ),
    fixture(NULL),
    half_block(false)
    {
        
    }
    enum ShapeType {
        ST_POLYGON = 0,
        ST_CIRCLE = 1,
        ST_EDGE = 2
    };
    
    ShapeType shape;        // The shape type.
    
    float density;
    float friction;
    float restitution;
    
    b2Fixture *fixture;
    
    bool half_block;
};

typedef std::vector<StaticBlockObject *> StaticBlockList;
