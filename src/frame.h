#pragma once

// Texture region and animated texture region

#include <vector>
#include <string>
#include <memory>
#include "texture.h"
#include "maths.h"

class Region
{
public:
    Region(const std::string& texture_name, const Box& rect);
    Region(const Texture *texture, const Box& rect);
    virtual ~Region();
    Region(const Region& other);

public:
    virtual void update(float dt)
    {}

    virtual Region *copy() const;

    // Returns the texture coordinates transform
    virtual Mat3 *get_transform() const;

public:
    const Texture *texture;

    Box rect;

private:
    // Create transform from rect
    void create_transform();

private:
    Mat3 *transform;
};

// Animated region
class Frame : public Region
{
public:
    Frame(const std::string& texture_name, const Box& region);
    Frame(const std::string& texture_name, const std::vector<Box>& regions, float fps);
    Frame(const Texture *texture, const Box& region);
    Frame(const Texture *texture, const std::vector<Box>& regions, float fps);
    Frame(const Frame& other);
    virtual ~Frame() = default;

public:
    virtual void update(float dt) override;

    virtual Region *copy() const override;

    virtual Mat3 *get_transform() const override;

private:
    // All transforms describing regions
    std::vector<std::unique_ptr<Mat3>> transforms;
    
    // Current region transform
    std::vector<std::unique_ptr<Mat3>>::const_iterator current;

    float time = 0;
    // 1 / fps
    float frame_duration;
};
