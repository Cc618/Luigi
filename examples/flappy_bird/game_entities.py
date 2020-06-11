from random import randint
import luigi as lg
from flappy import game, width, height
import settings


# Game
speed = 150
gravity = 2000
ground_y = 128

# Bird
bird_width, bird_height = 90, 70
bird_tex_width, bird_tex_height = 256, 128
bird_scale = .5

# Pipes
pipe_width, pipe_height = 128, 896

# Random height range
pipe_range = height * .3

# Gap between two pipe pairs for x
# Gap between two pipes in a pair for y
pipe_gap_x, pipe_gap_y = 384, 256


class Player(lg.Sprite):
    # Global variables
    jump_force = 600
    death_jump_force = 1200

    # To access the current player from outside of the Player class
    instance = None

    def __init__(self, x, y, on_menu=False):
        super().__init__(lg.CompoundFrame(
        {
            'blue': lg.IndexedFrame('flappy', lg.Box.tape(0, 0, bird_tex_width, bird_tex_height, 3, horizontal=False)),
            'red': lg.IndexedFrame('flappy', lg.Box.tape(bird_tex_width, 0, bird_tex_width, bird_tex_height, 3, horizontal=False)),
        }, settings.skin))

        self.x = x
        self.y = y
        self.on_menu = on_menu
        self.scale(bird_scale)

        # Y velocity
        self.vel_y = 0

        # False for the death animation
        self.controllable = True

        # Singleton
        Player.instance = self

    def update(self, dt):
        # Don't forget to call super
        super().update(dt)

        # Update frame index
        # 0 = wings up, 1 = wings mid, 2 = wings down
        self.frame.get().i = 0 if self.vel_y < -100 else 1 if self.vel_y < 100 else 2

        # If we click and we can fly
        if self.controllable and (game.typed('space') and not self.on_menu or \
                (game.mouse_typed('left') and not (self.on_menu and lg.Mouse.pos[1] < height / 2))):
            # Change skin
            if self.on_menu:
                settings.skin = 'blue' if settings.skin == 'red' else 'red'
                self.frame.current = settings.skin

            # Fly
            self.vel_y = Player.jump_force

            # Sound
            game.play('wing')
        else:
            # Gravity
            self.vel_y -= gravity * dt

        # Update rotation (using a linear interpolation)
        alpha = .5
        self.rot = self.rot * (1 - alpha) + alpha * self.vel_y * 1e-3

        # Update position
        self.y += self.vel_y * dt

        if self.on_menu:
            if self.y <= height * 2 / 3:
                self.y = height * 2 / 3
                self.vel_y = 0
                self.rot = 0

            return

        # Check collisions
        if (self.y - bird_height / 2 * bird_scale <= ground_y or self.y + bird_height / 2 * bird_scale >= height) and self.controllable:
            # Touches ground / top
            self.hit()
        elif self.y - bird_height * bird_scale / 2 <= 0 and not self.controllable:
            # Outside screen
            game.set_scene('menu')

    def hit(self):
        '''
            When the bird hits the ground or a pipe
        '''
        self.controllable = False
        self.vel_y = Player.death_jump_force
        game.play('hit')

        PipeManager.instance.enabled = False

    def hitbox(self):
        '''
            We use a custom hitbox instead of rect to have a smaller region
        '''
        x = self.x - bird_width / 2
        y = self.y - bird_height / 2

        return lg.Box(x, y, bird_width, bird_height)


class Image(lg.Sprite):
    '''
        A simple image.
    '''
    def __init__(self, x, y, frame, scale=1):
        super().__init__(frame)

        self.x = x
        self.y = y
        self.scale(scale)


class Trigger(lg.Entity):
    '''
        The trigger to count score.
    '''
    def __init__(self, x):
        super().__init__()
        self.x = x

    def update(self, dt):
        super().update(dt)

        # Kill if bird is dead
        if not PipeManager.instance.enabled:
            self.dead = True
            return

        self.x -= speed * dt

        if self.x <= Player.instance.x:
            ScoreManager.instance.inc_score()
            self.dead = True


class PipeManager(lg.Entity):
    '''
        Used to spawn / remove pipes
    '''
    instance = None

    def __init__(self):
        super().__init__()

        # All spawned pipes
        self.pipes = []

        # How much distance the bird has done
        self.dist = 0

        # When we exceed this distance, a new pipe pair spawns
        self.next_dist = 0

        self.enabled = True

        # Singleton
        PipeManager.instance = self

    def update(self, dt):
        super().update(dt)

        # Don't move / spawn / check collisions if disabled
        if not self.enabled:
            return

        self.dist += speed * dt

        for pipe in self.pipes:
            if pipe.x <= -pipe_width:
                # Destroy
                pipe.dead = True
            else:
                # Move
                pipe.x -= dt * speed

                # Test bird collision
                if pipe.rect().collides(Player.instance.hitbox()):
                    Player.instance.hit()

        # Spawn a new pair of pipes
        if self.next_dist <= self.dist:
            self.next_dist += pipe_gap_x
            self.spawn()

    def spawn(self):
        '''
            Spawns a pair of pipe
        '''
        game.set_layer('main')
        x = width + pipe_width
        y = (height + ground_y) / 2 + randint(-pipe_range // 2, pipe_range // 2)

        # Top
        pipe_top = Image(x, y + pipe_height / 2 + pipe_gap_y / 2, lg.Region('flappy', lg.Box(512, 128, pipe_width, pipe_height)))
        pipe_top.rot = lg.pi
        game.add(pipe_top)
        self.pipes.append(pipe_top)

        # Bottom
        pipe_bottom = Image(x, y - pipe_height / 2 - pipe_gap_y / 2, lg.Region('flappy', lg.Box(512, 128, pipe_width, pipe_height)))
        game.add(pipe_bottom)
        self.pipes.append(pipe_bottom)

        # Score trigger
        trigger = Trigger(x)
        game.add(trigger)


class ScoreManager(lg.Entity):
    '''
        Displays the score.
    '''
    instance = None

    def __init__(self):
        super().__init__()

        self.score = 0

        ScoreManager.instance = self

    def start(self):
        super().start()

        x, y = width / 2, height * (5 / 6)
        w = 100

        self.digits = [
            Image(x - w, y, lg.IndexedFrame('flappy', lg.Box.tape(512, 0, 128, 128, 10))),
            Image(x, y, lg.IndexedFrame('flappy', lg.Box.tape(512, 0, 128, 128, 10))),
            Image(x + w, y, lg.IndexedFrame('flappy', lg.Box.tape(512, 0, 128, 128, 10))),
        ]

        game.set_layer('ui')
        for d in self.digits:
            game.add(d)

    def inc_score(self):
        game.play('point')
        self.score += 1

        # Update digits
        digits = [ord(c) - ord('0') for c in f'{self.score:03d}']

        self.digits[0].frame.i = digits[0]
        self.digits[1].frame.i = digits[1]
        self.digits[2].frame.i = digits[2]
