import luigi as lg


width, height = 512, 256 * 3
game = lg.Game()

def construct():
    # We import scenes here since we need the game variable in all scenes
    import game_scene

    # Textures
    game.add_texture('flappy', 'res/flappy.png')

    # Scenes
    game.add_scene('game', game_scene.create, default_cam='game')

    # Cameras
    game.add_cam('game', height=100)

    # Set entry scene
    # TODO : Menu
    game.set_scene('game')

    print('OK')


def run():
    conf = lg.ScreenConfig('Flappy Bird', width, height, 60, resizable=False)

    game.run(construct, conf)
