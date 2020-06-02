#include "scene.h"
#include <algorithm>
#include "error.h"
#include "camera.h"

using namespace std;

// Compares 2 layers by their z index
static bool layer_cmp(Layer* a, Layer *b)
{
    return a->z < b->z;
}

// --- Scene --- //
Scene *Scene::current = nullptr;

list<Scene*> Scene::instances;

// TMP
// Scene *Scene::create(const std::string& name, const std::string& default_cam)
// {
//     auto scn = new Scene(name, default_cam);

//     instances.push_back(scn);

//     return scn;
// }

Scene *Scene::find(const std::string& name)
{
    auto scn = find_if(instances.begin(), instances.end(), [name](Scene* s) { return s->name == name; });

    Error::check(scn != instances.end(), "No scene with name '" + name + "' found");

    return *scn;
}

Scene::Scene(const std::string &name, const std::string& default_cam)
    : name(name), layers(layer_cmp), default_cam(default_cam)
{
    // Create default main layer
    layers.insert(selected_layer = new Layer("main"));
}

void Scene::start()
{
    Entity::start();

    // Try to retrieve the default camera
    auto i = Camera::instances.find(default_cam);
    Error::check(i != Camera::instances.end(), "The default camera named '" + default_cam + "' from the '" + name + "' scene cannot be found");
    Camera::main = (*i).second;
    Camera::main->update_ratio();

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

// --- SceneFactory --- //
unordered_map<std::string, SceneFactory*> SceneFactory::instances;

SceneFactory::SceneFactory(const std::string& name, const std::function<void ()>& factory, const std::string& default_cam)
    : name(name), factory(factory), default_cam(default_cam)
{
    auto i = instances.find(name);
    Error::check(i == instances.end(), "A scene with the same name already exists");

    instances[name] = this;
}

Scene *SceneFactory::spawn() const
{
    Scene *scn = new Scene(name, default_cam);
    Scene::current = scn;
    
    factory();

    scn->start();

    return scn;
}
