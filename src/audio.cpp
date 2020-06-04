#include "audio.h"
#include "error.h"

using namespace std;
using namespace lg;
using namespace sf;

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

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bind_audio(py::module &m)
{
    py::class_<lg::Sound>(m, "Sound")
        // TODO : rm
        .def(py::init<const string&, const string&>(), py::arg("name"), py::arg("file"))

        .def("play", &lg::Sound::play,
            "Plays the sound.")

        .def("set_volume", &lg::Sound::set_volume, py::arg("vol"),
            R"(
                Changes the sound volume.

                :param vol: The volume between 0 and 100.
            )")

        .def("set_pos", &lg::Sound::set_pos, py::arg("x"),
            "Changes the 1D position of the volume.")

        .doc() = R"(
            (**audio**) A SFX, can be played multiple times and at different positions.

            .. warning:: MP3 is NOT supported due to licensing issues.
                Only WAV, OGG/Vorbis and FLAC are supported.
        )"
    ;



    //     .doc() = R"(
    //         (**audio**) A looping music, when the music is changed, a transition is played.

    //         .. warning:: MP3 is NOT supported due to licensing issues.
    //             Only WAV, OGG/Vorbis and FLAC are supported.
    //     )"
    // ;
}
