#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

#define NUM_OF_TILES 9
Vec4t D_800FD7F0[NUM_OF_TILES];

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

u32 D_800FD7E0;
u32 D_800C2FF4;

Vec3f panelPositionsInOrder[9] =  {
    -1.0f, 0.0f, -1.0f, //top left
    0.0f, 0.0f, -1.0f, //top middle
    1.0f, 0.0f, -1.0f, //top left

    -1.0f, 0.0f, 0.0f, //left middle
    0.0f, 0.0f, 0.0f, //middle
    1.0f, 0.0f, 0.0f, //right middle

    -1.0f, 0.0f, 1.0f, //bottom right
    0.0f, 0.0f, 1.0f, //bottom middle
    1.0f, 0.0f, 1.0f, //bottom right

};

void setInitialPanelPositions(void) {
    D_800FD7F0[0].x = -1.0f;
    D_800FD7F0[0].y = 0.0f;
    D_800FD7F0[0].z = -1.0f;

    D_800FD7F0[1].x = -1.0f;
    D_800FD7F0[1].y = 0.0f;
    D_800FD7F0[1].z = 0.0f;

    D_800FD7F0[2].x = -1.0f;
    D_800FD7F0[2].y = 0.0f;
    D_800FD7F0[2].z = 1.0f;

    D_800FD7F0[3].x = 0.0f;
    D_800FD7F0[3].y = 0.0f;
    D_800FD7F0[3].z = -1.0f;

    D_800FD7F0[4].x = 0.0f;
    D_800FD7F0[4].y = 0.0f;
    D_800FD7F0[4].z = 0.0f;

    D_800FD7F0[5].x = 0.0f;
    D_800FD7F0[5].y = 0.0f;
    D_800FD7F0[5].z = 1.0f;

    D_800FD7F0[6].x = 1.0f;
    D_800FD7F0[6].y = 0.0f;
    D_800FD7F0[6].z = -1.0f;

    D_800FD7F0[7].x = 1.0f;
    D_800FD7F0[7].y = 0.0f;
    D_800FD7F0[7].z = 0.0f;

    D_800FD7F0[8].x = 1.0f;
    D_800FD7F0[8].y = 0.0f;
    D_800FD7F0[8].z = 1.0f;
}

void func_800F7B6C_CEFDC(u16 arg0) {
    D_800FD7E0 = arg0 * D_800FD7E0;
    D_800FD7E0 += arg0;
}

u8 GetRandomByte(void) {
    D_800C2FF4 = D_800C2FF4 * 0x41C64E6D + 0x3039;
    return (D_800C2FF4 + 1) >> 16;
}

u16 func_800F7AFC_CEF6C(u16 arg0) {
    D_800FD7E0 = D_800FD7E0 * 0x19971204;
    D_800FD7E0 = (D_800FD7E0 + 0x19760831) >> 0x10;
    if (arg0 == 0) {
        return D_800FD7E0;
    } else {
        return (D_800FD7E0 % arg0);
    }
}

void func_800F8314_CF784(void) {
    Vec4t temp_f0;
    s16 temp_s0;
    s16 phi_v1;
    u16 temp_s3 = func_800F7AFC_CEF6C(0x200);
    u16 i;

    for (i = 0; i < temp_s3; i++) {
        temp_s0 = func_800F7AFC_CEF6C(NUM_OF_TILES);

        if (temp_s0 >= NUM_OF_TILES) {
            temp_s0 = 0;
        }

        if (func_800F7AFC_CEF6C(0) & 1) {
            phi_v1 = temp_s0 + 1;
        } else {
            phi_v1 = temp_s0 - 1;
        }

        if (phi_v1 >= NUM_OF_TILES) {
            phi_v1 = 0;
        } else if (phi_v1 < 0) {
            phi_v1 = 8;
        }
        
        temp_f0.x = D_800FD7F0[phi_v1].x;
        temp_f0.y = D_800FD7F0[phi_v1].y;
        temp_f0.z = D_800FD7F0[phi_v1].z;
        temp_f0.value = D_800FD7F0[phi_v1].value;

        D_800FD7F0[phi_v1].x = D_800FD7F0[temp_s0].x;
        D_800FD7F0[phi_v1].y = D_800FD7F0[temp_s0].y;
        D_800FD7F0[phi_v1].z = D_800FD7F0[temp_s0].z;
        D_800FD7F0[phi_v1].value = D_800FD7F0[temp_s0].value;

        D_800FD7F0[temp_s0].x = temp_f0.x;
        D_800FD7F0[temp_s0].y = temp_f0.y;
        D_800FD7F0[temp_s0].z = temp_f0.z;
        D_800FD7F0[temp_s0].value = temp_f0.value;
    }
}

void main(void) {
    char seedString[16];
    char* tempString;
    start: ;
    printf("Enter a hex seed: ");
    gets(seedString);
    D_800C2FF4 = (u32)strtoul (seedString, &tempString, 16);
    D_800FD7E0 = 0x19971204; //sub rng starts with this initial constant (always initialized to this value)
    setInitialPanelPositions();

    u8 temp = GetRandomByte();

    func_800F7B6C_CEFDC(temp); //set inital sub rng
    func_800F8314_CF784();

    for (int i = 0; i < NUM_OF_TILES; i++) {
        D_800FD7F0[i].value = i;
        //printf("%s:\n\tX:%1.1f\n\tY:%1.1f\n\tZ:%1.1f\n", itemImagesArray[D_800FD7F0[i].value], D_800FD7F0[i].x, D_800FD7F0[i].y, D_800FD7F0[i].z);
    }

    //print data in rows and columns like the game does
    s32 panelVals[3][3];
    for (int i = 0; i < 9; i++) {
        // normalize coords to act as array indices
        s32 Zcoord = (s32)D_800FD7F0[i].z + 1;
        s32 Xcoord = (s32)D_800FD7F0[i].x + 1;

        // set values for the item at the correct location
        panelVals[Zcoord][Xcoord] = D_800FD7F0[i].value;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
        // print panel values in display order
            printf("%s\t", itemImagesArray[panelVals[i][j]]);
        }

        printf("\n");
    }
    printf("\n\n");
    goto start;
}