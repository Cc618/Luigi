import luigi as lg
from flappy import game, width, height


class Play(lg.Sprite):
    def __init__(self):
        super().__init__(lg.Region('flappy', lg.Box(640, 128, 256, 128)))

        # TODO : Mouse pos reverse
        self.x = width / 2
        self.y = height / 2
    
        self.start_width, self.start_height = self.width, self.height

        self.scale = 1
        self.target_scale = self.scale
        self.anim_speed = .2
    
    def update(self, dt):
        super().update(dt)

        if game.typed('space'):
            game.set_scene('game')
        
        # The mouse is within the button
        if self.rect().contains(*lg.Mouse.pos):
            self.target_scale = 1.2

            # Button clicked
            if game.mouse_typed('left'):
                game.set_scene('game')
        else:
            self.target_scale = 1

        # Update scale by linearly interpolating scale and target_scale values
        self.scale = self.scale * (1 - self.anim_speed) + self.target_scale * self.anim_speed
        self.width, self.height = self.start_width * self.scale, self.start_height * self.scale        


def create():
    # TODO : Background
    game.set_clear_color(.9, .95, 1)

    game.add(Play())
    pass
