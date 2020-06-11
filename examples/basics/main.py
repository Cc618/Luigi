import luigi as lg


# Inherit from Sprite
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
        if game.pressed('up'):
            self.y += self.speed * dt
        if game.pressed('down'):
            self.y -= self.speed * dt

        if game.pressed('space'):
            # The rotation is in radians, so lg.pi * 2 is one turn
            self.rot += lg.pi * 2 * dt

        if game.typed('M'):
            # Go to menu
            game.set_scene('menu')

        if game.typed('K'):
            # Kill
            self.dead = True


def create_game():
    '''
        When we have to create the game scene
    '''
    # Set the background blue
    game.set_clear_color(.8, .9, 1)

    # This scene contains only a player
    game.add(Player(0, 0, 1000))


def create_menu():
    # The menu is empty, just set the background white
    game.set_clear_color(1, 1, 1)


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


# Create and launch the game
game = lg.Game()
conf = lg.ScreenConfig('Luigi', 600, 400, 60)
game.run(construct, conf)
