#pragma once

// A screen with multiple layers of entities

#include <string>
#include <set>
#include <unordered_map>
#include <functional>
#include "entity.h"
#include "layer.h"

namespace lg
{
    class Scene : public Entity
    {
    public:
        static Scene *current;

    public:
        Scene(const std::string& name, const std::string& default_cam);
        virtual ~Scene() = default;

    public:
        virtual void start() override;
        virtual void update(float dt) override;
        virtual void draw() override;
        virtual void stop() override;

    public:
        void add(Entity *e);

        // Like Game::set_layer
        void set_layer(const std::string& name);

        // Like Game::add_layer
        void add_layer(const std::string& name, int z=0);

    public:
        std::string name;
        std::string default_cam;
        Layer *selected_layer;
        // A set is used to order layers by the z index
        std::set<Layer*, bool (*)(Layer*, Layer*)> layers;
    };

    // These factories are never deleted
    struct SceneFactory
    {
    public:
        static std::unordered_map<std::string, SceneFactory*> instances;

    public:
        SceneFactory(const std::string& name, const std::function<void ()>& factory, const std::string& default_cam);

    public:
        // Creates a new scene
        Scene *spawn() const;

    public:
        std::string name;
        std::string default_cam;

        // This is not really a factory but it sets up the scene
        std::function<void ()> factory;
    };
}
