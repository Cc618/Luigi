# The game scene factory

import luigi as lg
from flappy import game, width, height
from game_entities import Player, Image, PipeManager, ground_y, ScoreManager


def create():
    # game.set_cam('game')

    # Player
    game.add(Player(width / 3, height / 2, skin='blue'))

    # Manager
    game.add(PipeManager())

    # Next entities will be over player and pipes
    game.add_layer('overlay', z=10)

    # Ground
    ground_height = 256
    game.add(Image(width / 2, ground_y - ground_height / 2, lg.Region('flappy', lg.Box(0, 1792, width, ground_height))))

    game.add_layer('background', z=-100)
    game.add(Image(width / 2, height / 2, lg.Region('flappy', lg.Box(1536, 1280, width, height))))
    # TODO : Parallax game.add(Image(width / 2, height / 2, lg.Region('flappy', lg.Box(1536, 1280, 512, 768))))

    game.add_layer('ui', z=100)
    game.add(ScoreManager())
