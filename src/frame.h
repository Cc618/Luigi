#pragma once

// Texture region and animated texture region

#include <vector>
#include <string>
#include <memory>
#include "texture.h"
#include "maths.h"

namespace lg
{
    // Base class for all objects describing a texture region / animation...
    class Frame
    {
    public:
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
        const Texture *texture;
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




















    /*
    class Region
    {
    public:
        // Creates a region that contains the whole texture
        static Region *create(const std::string& texture_name);

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
        // Creates a frame with 'count' regions with the same shape of 'first'
        static Frame *tape(const std::string& texture_name, Box first, int count, float fps, bool horizontal=true);

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
    */
}
