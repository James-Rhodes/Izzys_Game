#include "ECS.h"

void ECS::RegisterEntity(Entity &entity)
{
    if (m_entityMap.find(entity.id) != m_entityMap.end())
    {
        throw std::invalid_argument("The entity you tried to register already exists or the id has already been used");
        return;
    }

    m_entityMap[entity.id] = std::make_unique<EntityContainer>(&entity);
}

void ECS::RegisterEntityAsDrawable(const std::string &id)
{
    EntityContainer *entContainer = m_entityMap[id].get();
    entContainer->isDrawable = true;

    // Add to drawable entities
    m_drawableEntities.push_back(entContainer->entityPointer);

    // Set the iterator in the EntityContainer so that it can be easily removed later. it is the iterator one before the end
    entContainer->drawableIterator = --m_drawableEntities.end();
}

void ECS::RegisterEntityAsScreenSpaceDrawable(const std::string &id)
{
    EntityContainer *entContainer = m_entityMap[id].get();
    entContainer->isScreenSpaceDrawable = true;

    // Add to screen space drawable entities
    m_screenSpaceDrawableEntities.push_back(entContainer->entityPointer);

    // Set the iterator in the EntityContainer so that it can be easily removed later. it is the iterator one before the end
    entContainer->screenSpaceDrawableIterator = --m_screenSpaceDrawableEntities.end();
}

void ECS::RegisterEntityAsPhysicsObject(const std::string &id, b2Body *body)
{
    EntityContainer *entContainer = m_entityMap[id].get();
    entContainer->isPhysicsObject = true;

    entContainer->entityPointer->physBody = body;
    // Add to physicsObject entities
    m_physicsEntities.push_back(entContainer->entityPointer);

    // Set the iterator in the EntityContainer so that it can be easily removed later. it is the iterator one before the end
    entContainer->physicsObjectIterator = --m_physicsEntities.end();
}

void ECS::RegisterEntityAsPhysicsObject(const std::string &id, CirclePhysicsObjectConfig config)
{
    b2BodyDef bodyDef;
    bodyDef.type = config.isDynamic ? b2_dynamicBody : b2_kinematicBody;
    bodyDef.position.Set(config.pos.x, config.pos.y);
    bodyDef.fixedRotation = !config.isRollable;
    bodyDef.userData.pointer = (uintptr_t)m_entityMap[id]->entityPointer;

    b2Body *body = physManager->CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = config.radius;

    b2FixtureDef fixture;
    fixture.shape = &circle;
    fixture.density = config.density;
    fixture.friction = config.friction;
    fixture.restitution = config.restitution;
    fixture.isSensor = config.isSensor;

    body->CreateFixture(&fixture);

    RegisterEntityAsPhysicsObject(id, body);
}

void ECS::RegisterEntityAsPhysicsObject(const std::string &id, RectanglePhysicsObjectConfig config)
{
    b2BodyDef bodyDef;

    bodyDef.type = config.isDynamic ? b2_dynamicBody : b2_kinematicBody;
    bodyDef.position.Set(config.pos.x, config.pos.y);
    bodyDef.fixedRotation = !config.isRollable;
    bodyDef.userData.pointer = (uintptr_t)m_entityMap[id]->entityPointer;

    b2Body *body = physManager->CreateBody(&bodyDef);

    b2PolygonShape rect;
    rect.SetAsBox(config.width / 2, config.height / 2);

    b2FixtureDef fixture;
    fixture.shape = &rect;
    fixture.density = config.density;
    fixture.friction = config.friction;
    fixture.restitution = config.restitution;
    fixture.isSensor = config.isSensor;

    body->CreateFixture(&fixture);

    RegisterEntityAsPhysicsObject(id, body);
}

std::list<Entity *> &ECS::GetAllDrawableObjects()
{
    return m_drawableEntities;
};

std::list<Entity *> &ECS::GetAllScreenSpaceDrawableObjects()
{
    return m_screenSpaceDrawableEntities;
}

std::list<Entity *> &ECS::GetAllPhysicsObjects()
{
    return m_physicsEntities;
};

void ECS::SetPhysicsManager(b2World *_physManager)
{
    physManager = _physManager;
}

b2World *ECS::GetPhysicsManager()
{
    return physManager;
}

void ECS::RemoveEntity(const std::string &id)
{
    EntityContainer *entContainer = m_entityMap[id].get();

    entContainer->toBeDeleted = true;
}

void ECS::PermanentlyDeleteEntity(const std::string id)
{
    EntityContainer *entContainer = m_entityMap[id].get();

    Entity *entPointer = entContainer->entityPointer;

    if (entContainer->isDrawable)
    {
        m_drawableEntities.erase(entContainer->drawableIterator);
    }

    if (entContainer->isScreenSpaceDrawable)
    {
        m_screenSpaceDrawableEntities.erase(entContainer->screenSpaceDrawableIterator);
    }

    if (entContainer->isPhysicsObject)
    {
        physManager->DestroyBody(entContainer->entityPointer->physBody);
        m_physicsEntities.erase(entContainer->physicsObjectIterator);
    }

    delete entPointer;

    m_entityMap.erase(id);
}

void ECS::SetSpriteSheet(Texture2D _spriteSheet)
{
    spriteSheet = _spriteSheet;
}
Texture2D ECS::GetSpriteSheet()
{
    return spriteSheet;
};

void ECS::SetCamera(Camera2D *cam)
{
    camera = cam;
}

Camera2D *ECS::GetCamera()
{
    return camera;
}

void ECS::SetFont(Font *_font)
{
    font = _font;
}

Font *ECS::GetFont()
{
    return font;
}
