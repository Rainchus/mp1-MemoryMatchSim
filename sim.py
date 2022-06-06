from collections import Counter
from dataclasses import dataclass
from ctypes import c_uint32, c_uint8, c_int32

SUB_SEED_START = 0x19971204

@dataclass
class TileValue:
    name: str
    emoji: str

@dataclass
class Gamestate:
    tiles: list[int]
    main_seed: c_uint32
    rand_byte: c_uint8
    sub_seed: c_uint32

    def get_random_byte(self):
        self.main_seed = c_int32(self.main_seed.value * 0x41C64E6D + 0x3039)
        return c_uint8((self.main_seed.value + 1) >> 16)

    def seed_initial_subseed(self):
        return c_uint32(SUB_SEED_START * self.rand_byte.value + self.rand_byte.value)

    def __init__(self, main_seed: int):
        self.tiles = [
            0, 3, 6,
            1, 4, 7,
            2, 5, 8
        ]
        self.main_seed = c_uint32(main_seed)
        self.rand_byte = self.get_random_byte()
        self.sub_seed = self.seed_initial_subseed()

    def rand_in_range(self, range: int):
        self.sub_seed = c_uint32(self.sub_seed.value * SUB_SEED_START)
        self.sub_seed = c_uint32(c_uint32(self.sub_seed.value + 0x19760831).value >> 16)
        if range == 0:
            return self.sub_seed.value
        else:
            return self.sub_seed.value % range

    def shuffle_tiles(self):
        shuffle_amount = self.rand_in_range(0x200)
        for i in range(shuffle_amount):
            s1 = self.rand_in_range(9)

            if self.rand_in_range(0) & 1:
                s2 = s1 + 1
            else:
                s2 = s1 - 1
            s2 %= 9

            self.tiles[s1], self.tiles[s2] = self.tiles[s2], self.tiles[s1]

    def print_results(self):
        print_order: list[TileValue] = [None] * 9
        for i in range(9):
            print_order[self.tiles[i]] = symbol_order[i % 5]

        for i in range(9):
            print(print_order[i].emoji, end=" ")
            if (i + 1) % 3 == 0:
                print()

flower = TileValue("flower", "🌹")
shell = TileValue("shell", "🐢")
oneup = TileValue("oneup", "1️⃣")
mushroom = TileValue("mushroom", "🍄")
bowser = TileValue("bowser", "👹")

symbol_order: list[TileValue] = [
    flower, shell, oneup, mushroom, bowser,
    flower, shell, oneup, mushroom
]

tile_configs = Counter()

def hashable_tiles(tiles: list[int]):
    print_order = [0] * 9
    for i in range(9):
        print_order[tiles[i]] = i % 5

    return tuple(print_order)

def run_game(start_seed: int):
    state = Gamestate(start_seed)
    state.shuffle_tiles()
    #state.print_results()
    tile_configs[hashable_tiles(state.tiles)] += 1

for i in range(0x1000):
    run_game(i)

run_game(0x40000)
