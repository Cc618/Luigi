#include "layer.h"
// TODO :
#include <iostream>
using namespace std;

Layer::Layer(const std::string& name, int z)
    : name(name), z(z)
{}

Layer::~Layer()
{
    cout << "Layer::~Layer\n";
}

void Layer::start()
{
    for (auto entity : entities)
        entity->start();
}

void Layer::update(float dt)
{
    printf("Layer::update\n");
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

void Layer::add(Entity *e)
{
    entities.push_back(e);

    // TODO : Only if started
    e->start();
}