# The game scene factory

import luigi as lg
from flappy import game
from game_entities import Player


def create():
    print('On game')
    # game.set_cam('game')

    game.add(Player(0, 0))

