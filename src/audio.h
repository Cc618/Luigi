#pragma once

// For SFX / musics

#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>

namespace lg
{
    // Flushes sounds and musics
    void stop_audio();

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

    // Music stream
    // TODO : Transition duration
    class Music
    {
    public:
        static std::unordered_map<std::string, Music*> instances;
        static Music *current;

    public:
        Music(const std::string& name, const std::string& file);
    
    public:
        void play();
        void stop(bool fade_out=true);
        void set_volume(float vol);
        void set_pos(float x);
        void set_loop(bool loop=false);
    
    public:
        std::string name;

    private:
        // Used when we fade out the music
        // Current is then the music to fade in
        static Music *previous;

    private:
        sf::Music stream;
    };
}
