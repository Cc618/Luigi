#include "layer.h"

Layer::Layer(const std::string& name, const std::list<Entity*>& entities, int z)
    : name(name), entities(entities), z(z)
{}

Layer::~Layer()
{
    for (auto entity : entities)
        delete entity;
}

void Layer::start()
{
    for (auto entity : entities)
        entity->start();
}

void Layer::update(float dt)
{
    for (auto entity : entities)
        entity->update(dt);
    
    // Remove dead entities
    for (auto i = entities.begin(); i != entities.end(); ++i)
        if ((*i)->dead)
        {
            (*i)->stop();
            delete *i;
            i = entities.erase(i);
        }
}

void Layer::draw()
{
    for (auto entity : entities)
        entity->draw();
}

void Layer::stop()
{
    for (auto entity : entities)
        entity->stop();
}
