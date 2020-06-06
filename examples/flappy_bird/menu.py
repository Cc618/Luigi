import luigi as lg
from flappy import game, width, height


class Play(lg.Entity):
    def __init__(self):
        super().__init__()
    
    def update(self, dt):
        super().update(dt)

        if game.typed('space'):
            game.set_scene('game')

    # TODO : Test stop called when set_scene


def create():
    # TODO : Background
    game.set_clear_color(.9, .95, 1)

    game.add(Play())
    pass
