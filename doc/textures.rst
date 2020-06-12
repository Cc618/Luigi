Textures
========

This guide explains how texture and frames work together in Luigi.

First of all, let's explain what is a texture and a frame :

- A texture is an image loaded on the GPU, we don't use directly textures,
  we use texture handlers which describe a texture.

- A frame is a texture region, it can be animated and more.
  We use frames inside a sprite to create animations or just to load less images
  since loading many images can be expensive.

.. note:: This guide contains code samples of the `basics example <https://github.com/Cc618/Luigi/blob/master/examples/basics>`_.

=======
Loading
=======

To use textures, we need to load an image and associate it with a name.
We load everything in the construct method.

.. code-block:: python

    def construct():
        game.add_texture('flappy', 'res/flappy.png')

        # Other assets to load...

Now, we just have to remember the name of the texture.

=====================
Frames implementation
=====================

Let's see a simple implementation of frames, we will use a simple texture region.

Here is the texture :

.. image:: https://github.com/Cc618/Luigi/raw/master/examples/basics/res/flappy.png

We want to retrieve the top left blue bird, the dimensions of the blue bird region are 256 pixels
for the width and 128 pixels for the height.
Moreover, the x and y coordinates are computed from the top left corner of the texture so
the position is (0, 0).

Let's use the Region class which describes a non animated texture region.

.. code-block:: python

    class Player(lg.Sprite):
        def __init__(self, x, y, speed):
            # The sprite is a texture region from the top left corner of the 'flappy' texture
            super().__init__(lg.Region('flappy', lg.Box(x=0, y=0, width=256, height=128)))

.. note:: Arguments' name are explicitly written only for readibility and can be omitted.

We now have a cool blue bird :

.. image:: https://github.com/Cc618/Luigi/raw/master/examples/basics/doc/game.png

===========
Frame types
===========

*****
Frame
*****

Luigi provides subclasses of the Frame class, this class is abstract and can't be used directly.

It contains the copy() and get_transform() methods used more often on the C++ side of Luigi.
Attributes are more useful, rect describes the hitbox of the region and texture it's image.

******
Region
******

A Region is a simple texture region, an AABB that contains the sprite's texture.

*************
Indexed Frame
*************

Indexed Frame gathers multiple regions and one texture, we can change the texture region with the
i property which is the index of the current region.

**************
Animated Frame
**************

This frame is used to change at a target frame rate several regions.
It inherits from IndexedFrame and takes in addition the FPS argument.

**************
Compound Frame
**************

This one is useful to make complex animations.
We can change a frame with a name, the frame can be any subclass of Frame.
For example, in the `Flappy Bird example <https://github.com/Cc618/Luigi/blob/master/examples/flappy_bird>`_
we use it to make multiple skins, a blue and red one.

=====
Utils
=====

When using complex or just long animations, we have many boilerplate code since we have to provide
a list of Box objects for an animation, the code can be easier to read with Box.tape.
This static method creates a list of Box objects with the same dimensions.

To illustrate, the player of the `Flappy Bird example <https://github.com/Cc618/Luigi/blob/master/examples/flappy_bird>`_
contains multiple calls to Box.tape :

.. code-block:: python
    
    # In Player.__init__
    lg.CompoundFrame(
        {
            'blue': lg.IndexedFrame('flappy', lg.Box.tape(0, 0, bird_tex_width, bird_tex_height, 3, horizontal=False)),
            'red': lg.IndexedFrame('flappy', lg.Box.tape(bird_tex_width, 0, bird_tex_width, bird_tex_height, 3, horizontal=False)),
        })

Box.tape takes as arguments in addition to the coordinates and dimensions the number of frames and the horizontality of the tape.
