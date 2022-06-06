#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

#define SUBSEED_START 0x19971204
#define NUM_OF_TILES 9

void hash_board(void);

Tile tileData[NUM_OF_TILES];

char FireFlower1String[] = "Flower";
char Mushroom1String[] = "Mush";
char Shell1String[] = "Shell";
char Shell2String[] = "Shell";
char bowserString[] = "Bowser";
char oneUp1String[] = "1UP";
char oneUp2String[] = "1UP";
char FireFlower2String[] = "Flower";
char Mushroom2String[] = "Mush";

//0 is fire flower
//1 is shell
//2 is 1UP
//3 is mushroom
//4 is bowser
//5 is fire flower
//6 is shell
//7 is 1UP
//8 is mushroom

char* itemImagesArray[] = {
    FireFlower1String,
    Shell1String,
    oneUp1String,
    Mushroom1String,
    bowserString,
    FireFlower2String,
    Shell2String,
    oneUp2String,
    Mushroom2String
};

u32 subSeed;
u32 mainSeed;
s32 panelVals[NUM_OF_TILES];

// statistics
static u32 board_freqs[0xFFFFFFF];
u32 bowsers[NUM_OF_TILES];
u32 nnswaps[0x200];

void setInitialPanelPositions(void) {
    tileData[0].x = -1;
    tileData[0].z = -1;

    tileData[1].x = -1;
    tileData[1].z = 0;

    tileData[2].x = -1;
    tileData[2].z = 1;

    tileData[3].x = 0;
    tileData[3].z = -1;

    tileData[4].x = 0;
    tileData[4].z = 0;

    tileData[5].x = 0;
    tileData[5].z = 1;

    tileData[6].x = 1;
    tileData[6].z = -1;

    tileData[7].x = 1;
    tileData[7].z = 0;

    tileData[8].x = 1;
    tileData[8].z = 1;
}

void func_800F7B6C_CEFDC(u16 subseedSeed) {
    subSeed = subseedSeed * subSeed;
    subSeed += subseedSeed;
}

u8 GetRandomByte(void) {
    mainSeed = mainSeed * 0x41C64E6D + 0x3039;
    return (mainSeed + 1) >> 16;
}

u16 random_in_range(u16 range) {
    subSeed = subSeed * 0x19971204;
    subSeed = (subSeed + 0x19760831) >> 0x10;

    if (range == 0) {
        return subSeed;
    } else {
        return subSeed % range;
    }
}

void swap_tiles(void) {
    s32 tempx;
    s32 tempz;
    s32 tile1;
    s32 tile2;
    u16 numSwaps = random_in_range(0x200);
    u16 i;

    nnswaps[numSwaps]++;

    for (i = 0; i < numSwaps; i++) {
        tile1 = random_in_range(NUM_OF_TILES);

        if (random_in_range(0) & 1) {
            tile2 = tile1 + 1;
        } else {
            tile2 = tile1 - 1;
        }

        if (tile2 >= NUM_OF_TILES) {
            tile2 = 0;
        } else if (tile2 < 0) {
            tile2 = 8;
        }
        
        tempx = tileData[tile2].x;
        tempz = tileData[tile2].z;

        tileData[tile2].x = tileData[tile1].x;
        tileData[tile2].z = tileData[tile1].z;

        tileData[tile1].x = tempx;
        tileData[tile1].z = tempz;
    }
}

u32 random_freq[0xFFFF];

void do_game(u32 seed) {
    mainSeed = seed;
    subSeed = SUBSEED_START; //sub rng starts with this initial constant (always initialized to this value)
    setInitialPanelPositions();

    u8 subseedSeed = GetRandomByte();

    func_800F7B6C_CEFDC(subseedSeed); //set inital sub rng


    swap_tiles();

    for (int i = 0; i < NUM_OF_TILES; i++) {
        tileData[i].value = i;
    }

    for (int i = 0; i < NUM_OF_TILES; i++) {
        // normalize coords to act as array indices
        s32 Zcoord = tileData[i].z + 1;
        s32 Xcoord = tileData[i].x + 1;

        s32 idx = (Zcoord * 3) + Xcoord;

        // set values for the item at the correct location
        panelVals[idx] = tileData[i].value;

        if (tileData[i].value == 4) {
            bowsers[idx]++;
        }
    }
    hash_board();
}

void print_bowsers(void) {
    for (u8 j = 0; j < NUM_OF_TILES; j++) {
        printf("%d ", bowsers[j]);
    }
    printf("\n");
}

void hash_board(void) {
    u32 tableIdx = 0;
    for (int i = 0; i < NUM_OF_TILES; i++) {
        tableIdx |= (panelVals[i] % 5) << (i * 3);
    }
    board_freqs[tableIdx]++;
}

void print_board_freqs(void) {
    int unique = 0;

    for (int i = 0; i < 0xFFFFFFF; i++) {
        if (board_freqs[i] > 0) {
            for (int j = 0; j < NUM_OF_TILES; j++) {
                printf("%d ", (i >> (j * 3)) & 7);
            }
            printf("%u: %u\n", i, board_freqs[i]);
            unique++;
        }
    }
    printf("%d unique\n", unique);
}

void print_num_swap_freqs(void) {
    int unique = 0;

    for (int i = 0; i < 0x200; i++) {
        if (nnswaps[i] > 0) {
            printf("%u: %u\n", i, nnswaps[i]);
            unique++;
        }
    }
    printf("%d unique numSwaps\n", unique);
}

void print_num_random_freq(void) {
    int unique = 0;

    for (int i = 0; i < 0xFFFF; i++) {
        if (random_freq[i] > 0) {
            printf("%u: %u\n", i, random_freq[i]);
            unique++;
        }
    }
    printf("%d unique random_freq\n", unique);
}

void main(void) {
    // for (u32 i = 0; i < 1000; i++) {
    //     do_game(i);
    // }

    do_game(0x40000);

    //print data in rows and columns like the game does
    s32 panelVals[3][3];
    for (int i = 0; i < 9; i++) {
        // normalize coords to act as array indices
        s32 Zcoord = tileData[i].z + 1;
        s32 Xcoord = tileData[i].x + 1;

        // set values for the item at the correct location
        panelVals[Zcoord][Xcoord] = tileData[i].value;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
        // print panel values in display order
            printf("%s\t", itemImagesArray[panelVals[i][j]]);
        }

        printf("\n");
    }
    printf("\n\n");

    //print_num_random_freq();
    // print_num_swap_freqs();
    // print_board_freqs();
    //print_bowsers();
}
