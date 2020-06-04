#include "frame.h"
#include "error.h"

using namespace lg;

// --- Region --- //
Region *Region::create(const std::string& texture_name)
{
    auto t = Texture::get(texture_name);

    auto r = new Region(t, Box(0, 0, t->width, t->height));

    return r;
}

Region::Region(const std::string& texture_name, const Box& rect)
    : Region(Texture::get(texture_name), rect)
{}

Region::Region(const Texture *texture, const Box& rect)
    : texture(texture), rect(rect)
{
    create_transform();
}

Region::~Region()
{
    delete transform;
}

Region::Region(const Region& other)
    : texture(other.texture), rect(other.rect), transform(new Mat3(*other.transform))
{}

Region *Region::copy() const
{
    return new Region(*this);
}

Mat3 *Region::get_transform() const
{
    return new Mat3(*transform);
}

void Region::create_transform()
{
    Box box(rect);
    float inv_width = 1.f / texture->width;
    float inv_height = 1.f / texture->height;

    // Normalize to have UV coordinates
    box.x *= inv_width;
    box.width *= inv_width;
    box.y *= inv_height;
    box.height *= inv_height;

    transform = box.get_transform();
}

// --- Frame --- //
Frame *Frame::tape(const std::string& texture_name, Box first, int count, float fps, bool horizontal)
{
    // TMP : Binding return !!!

    auto tex = Texture::get(texture_name);
    std::vector<Box> regions;
    regions.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        regions.emplace_back(first);

        if (horizontal)
            first.x += first.width;
        else
            first.y += first.height;
    }

    return new Frame(tex, regions, fps);
}

Frame::Frame(const std::string& texture_name, const Box& region)
    : Frame(Texture::get(texture_name), region)
{}

Frame::Frame(const std::string& texture_name, const std::vector<Box>& regions, float fps)
    : Frame(Texture::get(texture_name), regions, fps)
{}

Frame::Frame(const Texture *texture, const Box& region)
    : Frame(texture, { region }, .001f)
{}

Frame::Frame(const Texture *texture, const std::vector<Box>& regions, float fps)
    : Region(texture, Box())
{
    Error::check(!regions.empty(), "Regions cannot be empty");
    Error::check(fps > 0, "FPS must be > 0");

    // rect describes the first region here
    rect = regions[0];

    frame_duration = 1 / fps;

    // Inverse texture dimensions
    float inv_width = 1.f / texture->width;
    float inv_height = 1.f / texture->height;

    transforms.reserve(regions.size());

    // Normalize to have UV coordinates
    for (auto region : regions)
    {
        // Copy
        Box box(region);
        box.x *= inv_width;
        box.width *= inv_width;
        box.y *= inv_height;
        box.height *= inv_height;

        // Check region inside
        Error::check(
            box.x >= 0 && box.x + box.width <= 1 &&
            box.y >= 0 && box.y + box.height <= 1,
            "Region outside of texture"
        );

        transforms.emplace_back(box.get_transform());
    }

    current = transforms.begin();
}

Frame::Frame(const Frame& other)
    : Region(other), frame_duration(other.frame_duration)
{
    transforms.reserve(other.transforms.size());
    for (auto i = 0; i < other.transforms.size(); ++i)
        transforms.emplace_back(new Mat3(*other.transforms[i]));

    current = transforms.begin();
}

void Frame::update(float dt)
{
    time += dt;

    // Next frame
    if (time >= frame_duration)
    {
        time -= frame_duration;

        ++current;

        // Loop
        if (current == transforms.end())
            current = transforms.begin();
    }
}

Region *Frame::copy() const
{
    return new Frame(*this);
}

Mat3 *Frame::get_transform() const
{
    return new Mat3(**current);
}

// --- Bindings --- //
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

void bind_frame(py::module &m)
{
    py::class_<Region>(m, "Region")
        // TODO : Other constructors
        .def(py::init<const string&, const Box&>(), py::arg("texture_name"), py::arg("rect"))

        .def_static("create", &Region::create, py::arg("texture_name"),
            "Creates a region that contains the whole texture.")

        .doc() = "(**frame**) Describes an (axis aligned) rectangle in a texture."
    ;

    py::class_<Frame, Region>(m, "Frame")
        // TODO : Other constructors
        .def(py::init<const string&, const std::vector<Box>&, float>(), py::arg("texture_name"), py::arg("regions"), py::arg("fps"))

        .def_static("tape", &Frame::tape, py::arg("texture_name"), py::arg("first"), py::arg("count"), py::arg("fps"), py::arg("horizontal")=true,
            R"(
                Creates a frame from ``count`` regions placed at the same x / y level.

                :param first: The first region, if horizontal, this is the left most region, otherwise
                    this is the top most region.
                
                :param count: Number of regions.

                :param horizontal: True if the regions share the same x, False if they share the same y level.
            )")

        .doc() = "(**frame**) An animated texture region."
    ;
}