# The game scene factory

import luigi as lg
from flappy import game, width, height
from game_entities import Player, Image, PipeManager


def create():
    # game.set_cam('game')

    # Player
    game.add(Player(width / 3, height / 2, skin='blue'))

    # Manager
    game.add(PipeManager())

    # Next entities will be over player and pipes
    game.add_layer('overlay', z=10)

    # Ground
    # TODO : Scale change in texture
    ground_height = 256
    game.add(Image(width / 2, -ground_height, lg.Region('flappy', lg.Box(0, 1792, 512, ground_height)), scale=1.5))

