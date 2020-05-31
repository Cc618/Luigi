#include "frame.h"
#include "error.h"

Frame::Frame(const std::string& texture_name, Box region)
    : Frame(Texture::get(texture_name), region)
{}

Frame::Frame(const std::string& texture_name, const std::vector<Box>& regions, float fps)
    : Frame(Texture::get(texture_name), regions, fps)
{}

Frame::Frame(const Texture *texture, Box region)
    : Frame(texture, { region }, .001f)
{}

Frame::Frame(const Texture *texture, const std::vector<Box>& regions, float fps)
    : texture(texture)
{
    Error::check(!regions.empty(), "Regions cannot be empty");
    Error::check(fps > 0, "FPS must be > 0");

    first_region = regions[0];

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
    : texture(other.texture), frame_duration(other.frame_duration), first_region(other.first_region)
{
    transforms.reserve(other.transforms.size());
    for (auto transform : other.transforms)
        transforms.emplace_back(new Mat3(*transform));

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
