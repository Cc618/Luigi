#include "scene.h"
#include "error.h"
#include <algorithm>

using namespace std;

// Compares 2 layers by their z index
static bool layer_cmp(Layer* a, Layer *b)
{
    return a->z < b->z;
}

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
    : name(name), layers(layer_cmp)
{
    // Create default main layer
    layers.insert(selected_layer = new Layer("main"));
}

void Scene::start()
{
    Entity::start();

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

    Entity::stop();
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
        Error::check(!started, "Can't create layers when a scene has started");

        selected_layer = new Layer(name, z);
        layers.insert(selected_layer);
    }
    else
    {
        auto result = find_if(layers.begin(), layers.end(), [name](const Layer* l) { return l->name == name; });

        Error::check(result != layers.end(), "Could not find layer with name '" + name +
            "' in scene '" + this->name + "', add create=True to create a new layer");

        selected_layer = *result;
    }
}
