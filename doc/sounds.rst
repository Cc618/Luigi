Sounds
======

This page cover the audio functionalities of Luigi.

Firstly, sounds and musics are not the same in Luigi.
In addition to how the memory is handled for a sound or a music,
we use sounds for punctual SFX. This means that we play it
with a Python instruction.

A music is different, indeed, a music is always played (in loop)
and can be only changed, moreover, a transition is played between two musics.
We can remove transitions by setting their duration time to 0.

=======
Loading
=======

.. warning:: MP3 is NOT supported due to licensing issues. Only WAV, OGG/Vorbis and FLAC are supported.

Like for textures, we load sounds in the construct method and we associate each sound or music by
a name. Musics and sounds are different but you can't name a sound with the same name of a music.

.. code-block:: python

    def construct():
        # SFX
        game.add_sound('wing', 'res/wing.ogg')
        game.add_sound('point', 'res/point.ogg')

        # Musics
        game.add_music('menu', 'res/menu.ogg')
        game.add_music('game', 'res/game.ogg')

Remember to always load resources inside construct and not in a scene factory or anywhere else.

.. note:: add_sound or add_music return the created sound and music objects to be able to configure them.

=======
Playing
=======

To play a sound or a music, we use the method play of Game.
This method is the same for sounds and musics.

.. code-block:: python

    def create_game():
        # Change the music
        game.play('game')

        # Play a sound effect
        game.play('wing')

=============
Configuration
=============

To configure music transition duration, we can use set_fade_duration :

.. code-block:: python

    # In seconds
    game.set_fade_duration(1.2)

Music and Sound classes have two methods useful, set_volume and set_pos,
set_volume changes the volume of the audio component with a value between 0 and 100.

set_pos will change the x position of the sound, this means that the sound will be
louder in the right / left speaker if x != 0.

A last useful function within game, set_default_volume is used to change the default volume
of a music / SFX :

.. code-block:: python

    # Changes only default volume for sounds
    game.set_default_volume(sounds=50)
