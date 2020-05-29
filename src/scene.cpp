#include "scene.h"
#include "error.h"
#include <algorithm>
// TODO :
#include <iostream>

using namespace std;

Scene *Scene::current = nullptr;

list<Scene*> Scene::instances;

Scene *Scene::create(const std::string& name)
{
    auto scn = new Scene(name);

    instances.push_back(scn);

    return scn;
}

Scene *Scene::find(const std::string& name)
{
    auto scn = find_if(instances.begin(), instances.end(), [name](Scene* s) { return s->name == name; });

    Error::check(scn != instances.end(), "No scene with name '" + name + "' found");

    return *scn;
}

Scene::Scene(const std::string &name)
    : name(name)
{
    // TMP
    layers.push_back(selected_layer = new Layer("main"));
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
    Error::check(selected_layer != nullptr, "Can't add without layers, create a layer before creating entities");
    selected_layer->add(e);
}

void Scene::set_layer(const std::string& name, bool create, int z)
{
    if (create)
    {
        cout << "LAYER\n";
        selected_layer = new Layer(name, z);
        layers.push_back(selected_layer);
    }
    else
    {
        auto result = find_if(layers.begin(), layers.end(), [name](const Layer* l) { return l->name == name; });

        Error::check(result != layers.end(), "Could not find layer with name '" + name +
            "' in scene '" + this->name + "', add create=True to create a new layer");

        selected_layer = *result;
    }
}

// void Scene::add(Layer *layer)
// {
//     // TODO : Insert by z index
//     layers.push_back(layer);

//     // TODO : Only if started
//     layer->start();
// }
