#pragma once

// Texture region and animated texture region

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "texture.h"
#include "maths.h"

namespace lg
{
    // Base class for all objects describing a texture region / animation...
    class Frame
    {
    public:
        Frame() = default;
        Frame(const std::string& texture, const Box &rect);
        explicit Frame(const std::string& texture);
        Frame(const Texture *texture, const Box &rect);
        Frame(const Texture *texture);
        virtual ~Frame() = default;

    public:
        virtual void update(float dt)
        {}

        // Returns the texture coordinates transform
        virtual const Mat3 *get_transform() const = 0;

        virtual Frame *copy() const = 0;

    public:
        const Texture *texture = nullptr;
        // Describes the first rect around the texture
        Box rect;
    
    protected:
        // Creates the transform describing this rect
        Mat3 *create_transform(Box rect) const;
    };

    // A simple texture region
    class Region : public Frame
    {
    public:
        // Takes the whole texture
        explicit Region(const std::string &texture_name);
        Region(const std::string &texture_name, const Box &rect);
        Region(const Region &other);
        virtual ~Region();

    public:
        // Returns the texture coordinates transform
        virtual const Mat3 *get_transform() const override;
    
        virtual Frame *copy() const override;

    private:
        Mat3 *transform;
    };

    // Like a region but can be changed with an index
    class IndexedFrame : public Frame
    {
    public:
        IndexedFrame(const std::string &texture, const std::vector<Box>& regions, size_t i=0);
        IndexedFrame(const IndexedFrame &other);
        virtual ~IndexedFrame();

    public:
        // Returns the texture coordinates transform
        virtual const Mat3 *get_transform() const override;
    
        virtual Frame *copy() const override;

    public:
        void set_i(size_t i);
        size_t get_i() const;

    protected:
        std::vector<Mat3*> transforms;
        size_t i;
    };

    // An animated texture region
    class AnimatedFrame : public IndexedFrame
    {
    public:
        AnimatedFrame(const std::string &texture, const std::vector<Box>& regions, float fps, size_t i=0);
        AnimatedFrame(const AnimatedFrame &other);
        virtual ~AnimatedFrame() = default;

    public:
        virtual void update(float dt) override;

        virtual Frame *copy() const override;
    
    private:
        float time = 0;
        float frame_duration;
    };

    // Gathers multiple frames
    class CompoundFrame : public Frame
    {
    public:
        // Frames are not copied
        CompoundFrame(const std::unordered_map<std::string, Frame*> frames, const std::string& first);
        CompoundFrame(const CompoundFrame &other);
        virtual ~CompoundFrame() = default;
    
    public:
        virtual void update(float dt) override;

        virtual const Mat3 *get_transform() const override;

        virtual Frame *copy() const override;

    public:
        void set_current(const std::string &name);

        std::string get_current() const
        { return current_str; }

        Frame *get() const
        { return current; }

    private:
        Frame *current;
        std::string current_str;
        std::unordered_map<std::string, Frame*> frames;
    };
}
