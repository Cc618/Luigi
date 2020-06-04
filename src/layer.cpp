#include "layer.h"

using namespace std;
using namespace lg;

Layer::Layer(const std::string& name, int z)
    : name(name), z(z)
{}

void Layer::start()
{
    Entity::start();

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
            // No delete statement because python will garbage collect the entity
            (*i)->stop();
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
    
    Entity::stop();
}

void Layer::add(Entity *e)
{
    entities.push_back(e);

    if (started)
        e->start();
}
