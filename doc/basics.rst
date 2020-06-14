Basics
======

This guide explains how to get started with Luigi, we will discover how to make
a simple game with a controllable player and a menu.

.. image:: https://github.com/Cc618/Luigi/raw/master/examples/basics/doc/basics.gif

The code for this guide is `here (examples/basics) <https://github.com/Cc618/Luigi/blob/master/examples/basics>`_.

===============
The Game object
===============

In Luigi, many functions are gathered in the Game object, it is used firstly to create a window and run the game.

Let's create and configure the game :

.. code-block:: python

    import luigi as lg

    game = lg.Game()
    conf = lg.ScreenConfig('Luigi', 600, 400, 60)

These instructions tell luigi that the window will be named Luigi with a size of 600x400 pixels and 60 fps.

=================
Game construction
=================

Now, we have to load the game, this is made in the construct function.
This function serves to load textures and other resources and moreover to register scenes.

In Luigi, we use both object oriented and functional code,
functions are used to construct the game or create scenes and
objects are used with entities.

.. note:: create_game and create_scene will be explained below.

.. code-block:: python

    def construct():
        '''
            When the game is loading
        '''
        # Textures
        game.add_texture('flappy', 'res/flappy.png')

        # Scenes 
        game.add_scene('game', create_game, default_cam='game')
        game.add_scene('menu', create_menu, default_cam='game')

        # Cameras
        game.add_cam('game', height=500)

        # Set entry scene
        game.set_scene('game')

You can add also sounds and more with game.

This procedure registers two scenes named 'game' and 'menu',
we give as argument a function that loads the scene, we call this a scene factory.

Game gathers methods such as add_scene, add_texture, etc... to register and
load assets, make sure to load these assets in construct and not in a scene
or entity constructor.
To handle these assets, you just have to know the name of the target component like
in set_scene (this avoid global variables).

==============
Scene creation
==============

Let's implement create_game, we will use a Player entity, Player will be explained later.

.. code-block:: python

    def create_game():
        '''
            When we have to create the game scene
        '''
        # Set the background blue
        game.set_clear_color(.8, .9, 1)

        # This scene contains only a player
        game.add(Player(x=0, y=0, speed=1000))

This scene contains only a player over a blue background.

The game.add function adds entities to the current scene, in Luigi,
an entity is a game object and provides some methods such as start, draw, update and stop.
Update is the most important method, it is called each frame and aims to update player's behaviour.

=====================
Entity implementation
=====================

Luigi provides the Sprite class, this class is an entity with a texture and will be used in the player class :

.. code-block:: python

    # Inherits from Sprite
    class Player(lg.Sprite):
        def __init__(self, x, y, speed):
            # The sprite is a texture region from the top left corner of the 'flappy' texture
            super().__init__(lg.Region('flappy', lg.Box(0, 0, width=256, height=128)))

            # Position of the sprite
            self.x = x
            self.y = y

            # Custom attribute, the speed in pixels per second
            self.speed = speed

        def update(self, dt):
            # Don't forget to call super
            super().update(dt)

            if game.pressed('left'):
                self.x -= self.speed * dt

            if game.pressed('right'):
                self.x += self.speed * dt

            # * More code in the original file *

            if game.typed('M'):
                # Go to menu
                game.set_scene('menu')

            if game.typed('K'):
                # Kill
                self.dead = True

The most important part here is the update method, we test whether an input is pressed and we move
the player by setting the x attribute.
When M is typed, the scene changes to the menu scene with set_scene.
We can also remove an entity by setting the dead attribute to True.

Now, everything is created except create_menu, which is just a function that changes the background color.

We just have to launch the game with this instruction :

.. code-block:: python

    game.run(construct, conf)

You should have a player that moves with the arrow keys and rotates with space.

.. image:: https://github.com/Cc618/Luigi/raw/master/examples/basics/doc/basics.gif

===============
Game management
===============

Here is a little summary to help you to manage your game :

1. Configure the game.
2. Construct the game (load / register) resources.
3. Create scenes.
4. Create entities.

In your code, these instructions should be executed in the reverse order,
that is to say that entities have to be either created or imported from another python file before scenes.
The `Flappy Bird <https://github.com/Cc618/Luigi/blob/master/examples/flappy_bird>`_ example uses multiple source files to manage the game.
