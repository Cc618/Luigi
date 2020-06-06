import luigi as lg


width, height = 512, 768
game = lg.Game()


def construct():
    # We import scenes here since we need the game variable in all scenes
    import game_scene
    import menu

    # Sounds
    game.add_sound('wing', 'res/wing.ogg')
    game.add_sound('point', 'res/point.ogg')
    game.add_sound('hit', 'res/hit.ogg')

    # Textures
    game.add_texture('flappy', 'res/flappy.png')

    # Scenes
    game.add_scene('menu', menu.create, default_cam='menu')
    game.add_scene('game', game_scene.create, default_cam='game')

    # Cameras
    game.add_cam('game', height=height) \
        .set(x=width / 2, y=height / 2)
    
    game.add_cam('menu', height=height) \
        .set(x=width / 2, y=height / 2)

    # Set entry scene
    game.set_scene('menu')


def run():
    conf = lg.ScreenConfig('Flappy Bird', width, height, 60, resizable=False)

    game.run(construct, conf)
