#pragma once

// For SFX / musics

#include <string>
#include <thread>
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

    class Transition;

    // Music stream
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
    
        // TMP : Binding
        float get_volume() const;

    public:
        std::string name;

    private:
        // Used when we fade out the music
        // Current is then the music to fade in
        static Music *previous;

    private:
        sf::Music stream;
    };

    class Transition
    {
    public:
        // Starts a new transition (becomes Transition::instance) and stops the previous
        // transition if it exists
        static void push(Music *fade_in, Music *fade_out);

    public:
        static Transition *instance;

        // In seconds
        static float duration;

    public:
        // Musics can be nullptr
        // * The musics might be played
        Transition(Music *fade_in, Music *fade_out);
        virtual ~Transition();
    
    public:
        void stop();
        // Set default volume for each music
        void reset_volume();
    
    public:
        bool running = false;
        Music *fade_in;
        Music *fade_out;
        // Set only if musics are not nullptrs
        float fade_in_vol;
        float fade_out_vol;

    private:
        std::thread *volume_callback = nullptr;
    };
}
