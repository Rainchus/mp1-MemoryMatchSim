from collections import Counter
from dataclasses import dataclass
from ctypes import c_int32, c_uint32, c_uint8

SUB_SEED_START = 0x19971204

@dataclass
class TileValue:
    name: str
    emoji: str

@dataclass
class Gamestate:
    tiles: list[int]
    main_seed: c_uint32
    sub_seed: c_uint32

    def __init__(self, main_seed: int):
        self.tiles = [0, 3, 6, 1, 4, 7, 2, 5, 8]
        self.main_seed = c_uint32(main_seed)
        self.sub_seed = c_uint32(SUB_SEED_START)

flower = TileValue("flower", "🌹")
shell = TileValue("shell", "🐢")
oneup = TileValue("oneup", "1️⃣")
mushroom = TileValue("mushroom", "🍄")
bowser = TileValue("bowser", "👹")

symbol_order: list[TileValue] = [
    flower, shell, oneup, mushroom, bowser, flower, shell, oneup, mushroom
]

main_seed_vals = Counter()
sub_seed_vals = Counter()
tile_configs = Counter()

def get_random_byte(state: Gamestate):
    state.main_seed = c_int32(state.main_seed.value * 0x41C64E6D + 0x3039)
    main_seed_vals[state.main_seed.value] += 1
    return c_uint8((state.main_seed.value + 1) >> 16)

def seed_initial_subseed(state:Gamestate, rand_byte: c_uint8):
    state.sub_seed = c_uint32(state.sub_seed.value * rand_byte.value)
    state.sub_seed = c_uint32(state.sub_seed.value + rand_byte.value)

def getRandInRange(state:Gamestate, range: int):
    state.sub_seed = c_uint32(state.sub_seed.value * SUB_SEED_START)
    state.sub_seed = c_uint32(c_uint32(state.sub_seed.value + 0x19760831).value >> 16)
    sub_seed_vals[state.sub_seed.value] += 1
    if range == 0:
        return state.sub_seed.value
    else:
        return state.sub_seed.value % range

def hashable_tiles(tiles: list[int]):
    return tuple((t % 5) for t in tiles)

def shuffle_tiles(state: Gamestate):
    shuffleAmount = getRandInRange(state, 0x200)
    for i in range(shuffleAmount):
        s1 = getRandInRange(state, 9)

        if getRandInRange(state, 0) & 1:
            s2 = s1 + 1
        else:
            s2 = s1 - 1
        s2 %= 9

        state.tiles[s1], state.tiles[s2] = state.tiles[s2], state.tiles[s1]
        tile_configs[hashable_tiles(state.tiles)] += 1

def print_results(state: Gamestate):
    print_order: list[TileValue] = [None] * 9
    for i in range(9):
        print_order[state.tiles[i]] = symbol_order[i]

    for i in range(9):
        print(print_order[i].emoji, end=" ")
        if (i + 1) % 3 == 0:
            print()

def run_game(start_seed: int):
    state = Gamestate(start_seed)
    x = get_random_byte(state)
    seed_initial_subseed(state, x)
    shuffle_tiles(state)

    # print_results(state)

for i in range(0x400):
    run_game(i)
#print(len(tile_configs))
