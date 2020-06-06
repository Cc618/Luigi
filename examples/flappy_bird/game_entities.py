from random import randint
import luigi as lg
from flappy import game, width, height, bottom


# Game
speed = 150
gravity = 2000

# Bird
# TODO : Bird width = 140
bird_width, bird_height = 256, 128
bird_scale = .5

# Pipes
pipe_width, pipe_height = 128, 896

# Random height range
pipe_range = 256

# Gap between two pipe pairs for x
# Gap between two pipes in a pair for y
pipe_gap_x, pipe_gap_y = 384, 256


class Player(lg.Sprite):
    # Global variables
    jump_force = 600
    death_jump_force = 1200

    # To access the current player from outside of the Player class
    instance = None

    def __init__(self, x, y, skin):
        '''
            :param skin: Either red or blue, the color of the bird.
        '''
        super().__init__(lg.CompoundFrame(
        {
            'blue': lg.IndexedFrame('flappy', lg.Box.tape(0, 0, bird_width, bird_height, 3, horizontal=False)),
            'red': lg.IndexedFrame('flappy', lg.Box.tape(bird_width, 0, bird_width, bird_height, 3, horizontal=False)),
        }, skin))

        self.x = x
        self.y = y
        self.scale(bird_scale)

        # Y velocity
        self.vel_y = 0

        # False for the death animation
        self.controllable = True

        self.score = 0

        # Singleton
        Player.instance = self

    def update(self, dt):
        # Don't forget to call super
        super().update(dt)

        # Update frame index
        # 0 = wings up, 1 = wings mid, 2 = wings down
        self.frame.get().i = 0 if self.vel_y < -100 else 1 if self.vel_y < 100 else 2

        if self.controllable and (game.typed('space') or game.mouse_typed('left')):
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
        if self.y <= -bird_height / 2 * bird_scale and self.controllable:
            self.hit()
        elif self.y <= bottom - bird_height * bird_scale / 2 and not self.controllable:
            game.set_scene('menu')

    def hit(self):
        '''
            When the bird hits the ground or a pipe
        '''
        self.controllable = False
        self.vel_y = Player.death_jump_force
        game.play('hit')

        PipeManager.instance.enabled = False

    def inc_score(self):
        '''
            Increments the score
        '''
        game.play('point')
        self.score += 1

        print(f'Score : {self.score:03d}')


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
            Player.instance.inc_score()
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
                if pipe.rect().collides(Player.instance.rect()):
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
        y = height / 2 + randint(-pipe_range // 2, pipe_range // 2)

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
