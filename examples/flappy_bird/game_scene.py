# The game scene factory

import luigi as lg
from flappy import game, width, height
from game_entities import Player


def create():
    # game.set_cam('game')

    game.add(Player(width / 3, height / 2))

