import luigi as lg
from flappy import game


speed = 10
gravity = 2000


class Player(lg.Sprite):
    # Global variables
    jump_force = 800

    def __init__(self, x, y):
        super().__init__(lg.CompoundFrame(
        {
            'blue': lg.IndexedFrame('flappy', lg.Box.tape(0, 0, 256, 128, 3, horizontal=False)),
            'red': lg.IndexedFrame('flappy', lg.Box.tape(256, 0, 256, 128, 3, horizontal=False)),
        }, 'blue'))

        self.x = x
        self.y = y

        # Y velocity
        self.vel_y = 0

    def update(self, dt):
        # Don't forget to call super
        super().update(dt)

        # TODO : 
        self.frame.get().i = 1


        if game.typed('space') or game.mouse_typed('left'):
            # Fly
            self.vel_y = Player.jump_force

            # Sound
            game.play('wing')
        else:
            # Gravity
            self.vel_y -= gravity * dt

        self.y += self.vel_y * dt
        if self.y <= 0:
            # TODO : DEATH : Anim + Change scene after
            self.dead = True
            game.play('hit')

        # # Movements
        # if game.pressed('a'):
        #     self.x -= self.speed * dt
        # if game.pressed('d'):
        #     self.x += self.speed * dt

        # # Rotation
        # alpha = .5
        # self.rot = self.rot * (1 - alpha) + alpha * self.vy * .3

        # # Kill
        # if game.pressed('k'):
        #     music.stop(True)
        #     self.dead = True

        # # Change skin
        # if game.typed('t'):
        #     self.frame.current = 'blue' if self.frame.current == 'red' else 'red'
        #     print(self.frame.current)

        # # Move the player camera to the player
        # Player.player_cam.x, Player.player_cam.y = self.x, self.y

        # # Test collisions
        # # if game.pressed('i'):
        # #     bird.y += dt * 30
        # # elif game.pressed('o'):
        # #     bird.y -= dt * 30

        # # # Other bird collision detection
        # # if self.rect().collides(bird.rect()):
        # #     game.set_clear_color(1, 0, 0)
        # # else:
        # #     game.set_clear_color(0, 1, 0)







