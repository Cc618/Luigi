#pragma once

// A animatable texture region

#include <vector>
#include <string>
#include "texture.h"
#include "maths.h"

// TODO : Static frame ?

class Frame
{
public:
    Frame(const std::string& texture_name, Box region);
    Frame(const std::string& texture_name, const std::vector<Box>& regions, float fps);
    Frame(const Texture *texture, Box region);
    Frame(const Texture *texture, const std::vector<Box>& regions, float fps);
    Frame(const Frame& other);

public:
    void update(float dt);

    inline Mat3 *get_transform() const
    {
        return new Mat3(**current);
    }

public:
    const Texture *texture;

    Box first_region;

private:
    // TMP : Use shared pointers or delete
    // All transforms describing regions
    std::vector<Mat3*> transforms;
    
    // Current region transform
    std::vector<Mat3*>::const_iterator current;

    float time = 0;
    // 1 / fps
    float frame_duration;
};
