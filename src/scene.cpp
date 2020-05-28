#include "scene.h"

Scene *Scene::current = nullptr;

Scene::Scene(const std::string &name, const std::list<Layer*>& layers)
    : name(name), layers(layers)
{}

Scene::~Scene()
{
    for (auto layer : layers)
        delete layer;
}

void Scene::start()
{
    for (auto layer : layers)
        layer->start();
}

void Scene::update(float dt)
{
    for (auto layer : layers)
        layer->update(dt);
}

void Scene::draw()
{
    for (auto layer : layers)
        layer->draw();
}

void Scene::stop()
{
    for (auto layer : layers)
        layer->stop();
}
