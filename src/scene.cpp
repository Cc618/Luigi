#include "scene.h"
// TODO :
#include <iostream>
using namespace std;

Scene *Scene::current = nullptr;

Scene::Scene(const std::string &name)
    : name(name)
{
    // TMP
    layers.push_back(new Layer("main"));
}

Scene::~Scene()
{
    cout << "Scene::~Scene\n";
}

void Scene::start()
{
    for (auto layer : layers)
        layer->start();
}

void Scene::update(float dt)
{
    printf("Scene::update\n");
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

void Scene::add(Entity *e)
{
    // TODO : Find layer
    (*layers.begin())->add(e);
}

// void Scene::add(Layer *layer)
// {
//     // TODO : Insert by z index
//     layers.push_back(layer);

//     // TODO : Only if started
//     layer->start();
// }
