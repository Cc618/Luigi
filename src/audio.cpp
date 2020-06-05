#include "audio.h"
#include "error.h"
#include "game.h"

using namespace std;
using namespace lg;
using namespace sf;

void lg::stop_audio()
{
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

lg::Sound::Sound(const std::string& name, const std::string& file)
    : name(name)
{
    Error::check(buf.loadFromFile(file), "Can't load sound '" + name + "' with file '" + file + "'");
    snd.setBuffer(buf);
}

void lg::Sound::play()
{
    snd.play();
}

void lg::Sound::set_volume(float val)
{
    snd.setVolume(val);
}

void lg::Sound::set_pos(float val)
{
    snd.setPosition(val, 0, 0);
}

// --- Music --- //
std::unordered_map<std::string, lg::Music*> lg::Music::instances;

lg::Music *lg::Music::current = nullptr;
lg::Music *lg::Music::previous = nullptr;

lg::Music::Music(const std::string& name, const std::string& file)
    : name(name)
{
    Error::check(stream.openFromFile(file), "Can't load music '" + name + "' with file '" + file + "'");
    stream.setLoop(true);
}

void lg::Music::play()
{
    previous = current;
    current = this;

    // TODO : Fade out
    if (previous != nullptr)
        previous->stop();

    // TODO : Fade in 
    stream.play();
}

void lg::Music::stop(bool fade_out)
{
    // TODO : fade_out

    stream.stop();
}

void lg::Music::set_volume(float val)
{
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
