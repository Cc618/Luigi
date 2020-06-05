#include "audio.h"
#include "error.h"
#include "game.h"

using namespace std;
using namespace lg;
using namespace sf;

void lg::stop_audio()
{
    if (Transition::instance != nullptr)
        delete Transition::instance;

    for (auto i : lg::Sound::instances)
        delete i.second;

    for (auto i : lg::Music::instances)
    {
        i.second->stop(false);
        delete i.second;
    }

    lg::Sound::instances.clear();
    lg::Music::instances.clear();
}

// --- Sound --- //
std::unordered_map<std::string, lg::Sound*> lg::Sound::instances;
float lg::Sound::default_volume = 80;

lg::Sound::Sound(const std::string& name, const std::string& file)
    : name(name)
{
    Error::check(buf.loadFromFile(file), "Can't load sound '" + name + "' with file '" + file + "'");
    snd.setBuffer(buf);
    set_volume(default_volume);
}

void lg::Sound::play()
{
    snd.play();
}

void lg::Sound::set_volume(float val)
{
    Error::check(val >= 0 && val <= 100, "Volume must be between 0 and 100");

    snd.setVolume(val);
}

void lg::Sound::set_pos(float val)
{
    snd.setPosition(val, 0, 0);
}

// --- Music --- //
std::unordered_map<std::string, lg::Music*> lg::Music::instances;
float lg::Music::default_volume = 60;

lg::Music *lg::Music::current = nullptr;
lg::Music *lg::Music::previous = nullptr;

lg::Music::Music(const std::string& name, const std::string& file)
    : name(name)
{
    Error::check(stream.openFromFile(file), "Can't load music '" + name + "' with file '" + file + "'");
    stream.setLoop(true);
    set_volume(default_volume);
}

void lg::Music::play()
{
    previous = current;
    current = this;

    if (previous != current)
        Transition::push(current, previous);

    if (stream.getStatus() != sf::SoundSource::Playing)
        stream.play();
}

void lg::Music::stop(bool fade_out)
{
    if (fade_out)
    {
        current = nullptr;
        previous = this;
        Transition::push(current, previous);
    }
    else
        stream.stop();
}

void lg::Music::set_volume(float val)
{
    Error::check(val >= 0 && val <= 100, "Volume must be between 0 and 100");

    stream.setVolume(val);
}

void lg::Music::set_pos(float val)
{
    stream.setPosition(val, 0, 0);
}

void lg::Music::set_loop(bool loop)
{
    stream.setLoop(loop);
}

float lg::Music::get_volume() const
{
    return stream.getVolume();
}

// --- Transition --- //
// A thread which changes the volume of both musics
void fade()
{
    using namespace std::literals::chrono_literals;

    constexpr auto FADE_DT = 10ms;

    Clock clock;

    while (Transition::instance->running)
    {
        float elapsed = clock.getElapsedTime().asSeconds();

        // Transition elapsed time ratio
        float ratio = elapsed / Transition::duration;

        if (ratio >= 1)
        {
            Transition::instance->reset_volume();

            Transition::instance->running = false;
            break;
        }

        // Set volume
        if (Transition::instance->fade_in)
            Transition::instance->fade_in->set_volume(Transition::instance->fade_in_vol * ratio);

        if (Transition::instance->fade_out)
            Transition::instance->fade_out->set_volume(Transition::instance->fade_out_vol * (1 - ratio));

        // Wait
        std::this_thread::sleep_for(FADE_DT);
    }
}

Transition *Transition::instance = nullptr;

float Transition::duration = 1;

void Transition::push(lg::Music *fade_in, lg::Music *fade_out)
{
    if (instance != nullptr)
        delete instance;

    // Run new instance
    new Transition(fade_in, fade_out);
}

Transition::Transition(Music *fade_in, Music *fade_out)
    : fade_in(fade_in), fade_out(fade_out)
{
    Error::check(instance == nullptr, "Two transitions are playing");
    instance = this;

    if (fade_in != nullptr)
        fade_in_vol = fade_in->get_volume();

    if (fade_out != nullptr)
        fade_out_vol = fade_out->get_volume();

    // Start transition
    running = true;
    volume_callback = new thread(fade);
}

Transition::~Transition()
{
    stop();

    instance = nullptr;
}

void Transition::stop()
{
    if (running)
    {
        running = false;
        volume_callback->join();
        reset_volume();
    }

    if (volume_callback != nullptr)
    {
        volume_callback = nullptr;
        delete volume_callback;
    }
}

void Transition::reset_volume()
{
    if (fade_in != nullptr)
        fade_in->set_volume(fade_in_vol);

    if (fade_out != nullptr)
    {
        fade_out->set_volume(fade_out_vol);
        fade_out->stop(false);
    }
}

// --- Game --- //
void Game::play(const std::string& name)
{
    auto i = Sound::instances.find(name);

    // A sound is found
    if (i != Sound::instances.end())
        (*i).second->play();
    else
    {
        auto i = Music::instances.find(name);

        Error::check(i != Music::instances.end(), "The sound or music named '" + name + "' is not found");

        (*i).second->play();
    }
}

void Game::set_fade_duration(float seconds)
{
    Error::check(seconds >= 0, "The duration must be positive");

    // Avoid zero division
    if (seconds == 0)
        seconds += 1e-4;

    Transition::duration = seconds;
}

void Game::set_default_volume(float sounds, float musics)
{
    if (sounds >= 0)
        lg::Sound::default_volume = sounds;

    if (musics >= 0)
        lg::Music::default_volume = musics;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bind_audio(py::module &m)
{
    py::class_<lg::Sound>(m, "Sound")
        .def("play", &lg::Sound::play,
            "Plays the sound.")

        .def("set_volume", &lg::Sound::set_volume, py::arg("vol"),
            R"(
                Changes the sound volume.

                :param vol: The volume between 0 and 100.
            )")

        .def("set_pos", &lg::Sound::set_pos, py::arg("x"),
            "Changes the 1D position of the sound.")

        .doc() = R"(
            (**audio**) A SFX, can be played multiple times and at different positions.

            .. warning:: MP3 is NOT supported due to licensing issues.
                Only WAV, OGG/Vorbis and FLAC are supported.
        )"
    ;

    py::class_<lg::Music>(m, "Music")
        .def("play", &lg::Music::play,
            "Plays the music, a transition is also played.")

        .def("stop", &lg::Music::stop, py::arg("fade_out")=true,
            "Stops the music, a transition is also played if ``fade_out``.")

        .def("set_volume", &lg::Music::set_volume, py::arg("vol"),
            R"(
                Changes the sound volume.

                :param vol: The volume between 0 and 100.
            )")

        .def("set_pos", &lg::Music::set_pos, py::arg("x"),
            "Changes the 1D position of the music.")

        .def("set_loop", &lg::Music::set_loop, py::arg("loop")=false,
            "Sets whether the music is a loop.")

        .doc() = R"(
            (**audio**) A looping music, when the music is changed, a transition is played.

            .. warning:: MP3 is NOT supported due to licensing issues.
                Only WAV, OGG/Vorbis and FLAC are supported.
        )"
    ;
}
