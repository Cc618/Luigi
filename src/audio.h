#pragma once

// For SFX / musics

#include <string>
#include <SFML/Audio.hpp>

namespace lg
{
    // SFX
    class Sound
    {
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
