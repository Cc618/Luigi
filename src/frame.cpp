#include "frame.h"
#include "error.h"

using namespace lg;

// --- Frame --- //
Frame::Frame(const std::string& texture, const Box& rect)
    : Frame(Texture::get(texture), rect)
{}

Frame::Frame(const std::string& texture)
    : texture(Texture::get(texture))
{}

Frame::Frame(const Texture *texture, const Box& rect)
    : texture(texture), rect(rect)
{}

Frame::Frame(const Texture *texture)
    : texture(texture)
{}

// TODO : Check overflow
Mat3 *Frame::create_transform(Box box) const
{
    float inv_width = 1.f / texture->width;
    float inv_height = 1.f / texture->height;

    // Normalize to have UV coordinates
    box.x *= inv_width;
    box.width *= inv_width;
    box.y *= inv_height;
    box.height *= inv_height;

    return box.get_transform();
}

// --- Region --- //
Region::Region(const std::string& texture_name)
    : Frame(texture_name), transform(Mat3::create_id())
{
    rect.width = texture->width;
    rect.height = texture->height;
}

Region::Region(const std::string& texture_name, const Box& rect)
    : Frame(texture_name, rect), transform(create_transform(rect))
{}

Region::Region(const Region &other)
    : Frame(other.texture, other.rect), transform(new Mat3(*other.transform))
{}

Region::~Region()
{
    delete transform;
}

const Mat3 *Region::get_transform() const
{
    return transform;
}

Frame *Region::copy() const
{
    return new Region(*this);
}

// --- Indexed Frame --- //
IndexedFrame::IndexedFrame(const std::string &texture, const std::vector<Box>& regions, size_t i)
    : Frame(texture)
{
    Error::check(!regions.empty(), "At least one region must exist");

    rect = regions[0];

    // Compute transforms
    transforms.reserve(regions.size());
    for (auto region : regions)
        transforms.emplace_back(create_transform(region));

    set_i(i);
}

IndexedFrame::IndexedFrame(const IndexedFrame &other)
    : Frame(other.texture, other.rect), i(other.i)
{
    transforms.reserve(other.transforms.size());
    for (const Mat3 *transform : other.transforms)
        transforms.emplace_back(new Mat3(*transform));
}

IndexedFrame::~IndexedFrame()
{
    for (auto transform : transforms)
        delete transform;
}

const Mat3 *IndexedFrame::get_transform() const
{
    return transforms[i];
}

Frame *IndexedFrame::copy() const
{
    return new IndexedFrame(*this);
}

void IndexedFrame::set_i(size_t i)
{
    Error::check(i < transforms.size(), "i is outside of bounds");
    this->i = i;
}

size_t IndexedFrame::get_i() const
{
    return i;
}

// --- Animated Frame --- //
AnimatedFrame::AnimatedFrame(const std::string &texture, const std::vector<Box>& regions, float fps, size_t i)
    : IndexedFrame(texture, regions, i)
{
    Error::check(fps > 0, "fps must be greater than 0");

    frame_duration = 1.f / fps;
}

AnimatedFrame::AnimatedFrame(const AnimatedFrame &other)
    : IndexedFrame(other), frame_duration(other.frame_duration), time(other.time)
{}

void AnimatedFrame::update(float dt)
{
    time += dt;

    // Next frame
    if (time >= frame_duration)
    {
        time -= frame_duration;

        ++i;

        // Loop
        if (i >= transforms.size())
            i = 0;
    }
}

Frame *AnimatedFrame::copy() const
{
    return new AnimatedFrame(*this);
}

// --- Compound Frame --- //
CompoundFrame::CompoundFrame(const std::unordered_map<std::string, Frame*> frames, const std::string& first)
{
    Error::check(!frames.empty(), "No frames found");

    // Copy frames
    for (auto f : frames)
        this->frames[f.first] = f.second->copy();

    texture = (*frames.begin()).second->texture;
    rect = (*frames.begin()).second->rect;

    set_current(first);
}

CompoundFrame::CompoundFrame(const CompoundFrame &other)
    : Frame(other.texture, other.rect), current(other.current)
{
    // Copy frames
    for (auto f : other.frames)
        frames[f.first] = f.second->copy();
}

void CompoundFrame::update(float dt)
{
    current->update(dt);
}

const Mat3 *CompoundFrame::get_transform() const
{
    return current->get_transform();
}

Frame *CompoundFrame::copy() const
{
    return new CompoundFrame(*this);
}

void CompoundFrame::set_current(const std::string &name)
{
    auto i = frames.find(name);
    Error::check(i != frames.end(), "No frame named '" + name + "' found");

    current = (*i).second;
    current_str = name;
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

void bind_frame(py::module &m)
{
    py::class_<Frame>(m, "Frame")
        .def("get_transform", &Frame::get_transform,
            "Returns the texture coordinates transform for the current region.")

        .def("copy", &Frame::copy,
            "Clones itself.")

        .def_readonly("texture", &Frame::texture)

        .def_readonly("rect", &Frame::rect)

        .doc() = "(**frame**) Base class for texture regions."
    ;

    py::class_<Region, Frame>(m, "Region")
        .def(py::init<const string&>(), py::arg("texture_name"))
        .def(py::init<const string&, const Box&>(), py::arg("texture_name"), py::arg("rect"))

        // From Frame
        .def("get_transform", &Frame::get_transform,
            "Returns the texture coordinates transform for the current region.")

        .def("copy", &Frame::copy,
            "Clones itself.")

        .def_readonly("texture", &Frame::texture)

        .def_readonly("rect", &Frame::rect)

        .doc() = R"(
            (**frame**) Describes an (axis aligned) rectangle in a texture.

            The constructor can take only the texture name to create a region over
            the entire texture.
        )"
    ;

    py::class_<IndexedFrame, Frame>(m, "IndexedFrame")
        .def(py::init<const std::string&, const std::vector<Box>&, size_t>(), py::arg("texture_name"), py::arg("regions"), py::arg("i")=0)

        // From Frame
        .def("get_transform", &Frame::get_transform,
            "Returns the texture coordinates transform for the current region.")

        .def("copy", &Frame::copy,
            "Clones itself.")

        .def_readonly("texture", &Frame::texture)

        .def_readonly("rect", &Frame::rect)

        .def_property("i", &IndexedFrame::get_i, &IndexedFrame::set_i)

        .doc() = R"(
            (**frame**) Like :class:`Region` but the region can be changed with the index ``i``.
        )"
    ;

    py::class_<AnimatedFrame, IndexedFrame>(m, "AnimatedFrame")
        .def(py::init<const std::string&, const std::vector<Box>&, float, size_t>(), py::arg("texture_name"), py::arg("regions"), py::arg("fps"), py::arg("i")=0)

        // From Frame
        .def("get_transform", &Frame::get_transform,
            "Returns the texture coordinates transform for the current region.")

        .def("copy", &Frame::copy,
            "Clones itself.")

        .def_readonly("texture", &Frame::texture)

        .def_readonly("rect", &Frame::rect)

        // From IndexedFrame
        .def_property("i", &IndexedFrame::get_i, &IndexedFrame::set_i)

        .doc() = R"(
            (**frame**) An animated texture region.
        )"
    ;

    py::class_<CompoundFrame, Frame>(m, "CompoundFrame")
        .def(py::init<const std::unordered_map<std::string, Frame*>, const std::string&>(),
            py::arg("frames"), py::arg("first"), py::keep_alive<1, 2>(),
            R"(
                :param frames: A dict of frames.
                :param first: The name of the first frame.
            )")

        // From Frame
        .def("get_transform", &Frame::get_transform,
            "Returns the texture coordinates transform for the current region.")

        .def("copy", &Frame::copy,
            "Clones itself.")

        .def_readonly("texture", &Frame::texture)

        .def_readonly("rect", &Frame::rect)

        .def_property("current", &CompoundFrame::get_current, &CompoundFrame::set_current,
            "Changes the current frame by its name.")

        .def("get", &CompoundFrame::get, py::return_value_policy::reference,
            "Returns the current frame.")

        .doc() = R"(
            (**frame**) Gathers multiple frames, they can be changed with the ``current`` property.

            .. note:: Useful to make animations with instances of :class:`AnimatedFrame`.
        )"
    ;
}
