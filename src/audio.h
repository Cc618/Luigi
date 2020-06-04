#pragma once

// For SFX / musics

#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>

namespace lg
{
    // SFX
    class Sound
    {
    public:
        static std::unordered_map<std::string, Sound*> instances;

    public:
        Sound(const std::string& name, const std::string &file);
    
    public:
        void play();
        void set_volume(float vol);
        void set_pos(float x);
    
    public:
        std::string name;

    private:
        sf::SoundBuffer buf;
        sf::Sound snd;
    };
}
