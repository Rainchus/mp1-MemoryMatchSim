#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

#define NUM_OF_TILES 9
Vec4t tileData[NUM_OF_TILES];

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
s32 panelVals[3][3];

// statistics
u32 bowsers[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void setInitialPanelPositions(void) {
    tileData[0].x = -1.0f;
    tileData[0].y = 0.0f;
    tileData[0].z = -1.0f;

    tileData[1].x = -1.0f;
    tileData[1].y = 0.0f;
    tileData[1].z = 0.0f;

    tileData[2].x = -1.0f;
    tileData[2].y = 0.0f;
    tileData[2].z = 1.0f;

    tileData[3].x = 0.0f;
    tileData[3].y = 0.0f;
    tileData[3].z = -1.0f;

    tileData[4].x = 0.0f;
    tileData[4].y = 0.0f;
    tileData[4].z = 0.0f;

    tileData[5].x = 0.0f;
    tileData[5].y = 0.0f;
    tileData[5].z = 1.0f;

    tileData[6].x = 1.0f;
    tileData[6].y = 0.0f;
    tileData[6].z = -1.0f;

    tileData[7].x = 1.0f;
    tileData[7].y = 0.0f;
    tileData[7].z = 0.0f;

    tileData[8].x = 1.0f;
    tileData[8].y = 0.0f;
    tileData[8].z = 1.0f;
}

void func_800F7B6C_CEFDC(u16 arg0) {
    subSeed = arg0 * subSeed;
    subSeed += arg0;
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
        return (subSeed % range);
    }
}

void swapTiles(void) {
    Vec4t temp_f0;
    s16 tile1;
    s16 tile2;
    u16 numSwaps = random_in_range(0x200);
    u16 i;

    for (i = 0; i < numSwaps; i++) {
        tile1 = random_in_range(NUM_OF_TILES);

        if (tile1 >= NUM_OF_TILES) {
            tile1 = 0;
        }

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
        
        temp_f0.x = tileData[tile2].x;
        temp_f0.z = tileData[tile2].z;

        tileData[tile2].x = tileData[tile1].x;
        tileData[tile2].z = tileData[tile1].z;

        tileData[tile1].x = temp_f0.x;
        tileData[tile1].z = temp_f0.z;
    }
}

void do_game(u32 seed) {
    mainSeed = seed;
    subSeed = 0x19971204; //sub rng starts with this initial constant (always initialized to this value)
    setInitialPanelPositions();

    u8 subseedSeed = GetRandomByte();

    func_800F7B6C_CEFDC(subseedSeed); //set inital sub rng
    swapTiles();

    for (int i = 0; i < NUM_OF_TILES; i++) {
        tileData[i].value = i;
    }

    for (int i = 0; i < 9; i++) {
        // normalize coords to act as array indices
        s32 Zcoord = (s32)tileData[i].z + 1;
        s32 Xcoord = (s32)tileData[i].x + 1;

        // set values for the item at the correct location
        panelVals[Zcoord][Xcoord] = tileData[i].value;

        if (tileData[i].value == 4) {
            bowsers[(Zcoord * 3) + Xcoord]++;
        }
    }
}

void main(void) {
    //char seedString[16];
    //char* tempString;
    // start: ;
    // printf("Enter a hex seed: ");
    // gets(seedString);
    //mainSeed = (u32)strtoul (seedString, &tempString, 16);
    
    for (u32 i = 0; i < 0xFFFFFFFF; i++) {
        do_game(i);
        if ((i % 0x10000) == 0) {
            printf("%d: ", i);
            for (u8 j = 0; j < 9; j++) {
                printf("%d ", bowsers[j]);
            }
            printf("\n");
        }
   }


    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
        // print panel values in display order
            printf("%s\t", itemImagesArray[panelVals[i][j]]);
        }

        printf("\n");
    }
    // printf("\n\n");
    // goto start;
}