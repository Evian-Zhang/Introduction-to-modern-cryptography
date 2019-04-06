//
//  main.cpp
//  DES
//
//  Created by Evian张 on 2019/3/29.
//  Copyright © 2019 Evian张. All rights reserved.
//

#include <iostream>
#include <bitset>
using namespace std;

int IP[] = {58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

int IP_1[] = {40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25};

int PC_1[] = {57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4};

int PC_2[] = {14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

int shiftBits[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
int inverseShiftBits[] = {0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2,  2, 2, 2, 2, 1};

int E[] = {32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1};

int S_BOX[8][4][16] = {
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

int P[] = {16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25 };

enum ShiftStyle {
    normalShift,
    inverseShift
};

bitset<64> getInitialPermutation(bitset<64> plainText)
{
    bitset<64> initialPermutation;
    for (int i = 0; i < 64; i++)
    {
        initialPermutation[63 - i] = plainText[64 - IP[i]];
    }
    return initialPermutation;
}

bitset<4> S_boxi(bitset<6> input, int i)
{
    int row = 2 * input[5] + input[0];
    int column = 8 * input[4] + 4 * input[3] + 2 * input[2] + input[1];
    int outputint = S_BOX[i][row][column];
    bitset<4> output(outputint);
    return output;
}

bitset<32> S_box(bitset<48> input)
{
    bitset<32> output;
    bitset<6> SiInput[8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            SiInput[i][5 - j] = input[47 - (j + i * 6)];
        }
        bitset<4> SiOutput = S_boxi(SiInput[i], i);
        for (int j = 0; j < 4; j++)
        {
            output[31 - (j + i * 4)] = SiOutput[3 - j];
        }
    }
    return output;
}

bitset<32> F(bitset<32> rightPart, bitset<48> ki)
{
    bitset<32> output;
    bitset<48> expandedInput;
    for (int i = 0; i < 48; i++)
    {
        expandedInput[47 - i] = rightPart[32 - E[i]];
    }
    bitset<48> S_boxInput = expandedInput ^ ki;
    bitset<32> S_boxOutput = S_box(S_boxInput);
    for (int i = 0; i < 32; i++)
    {
        output[31 - i] = S_boxOutput[32 - P[i]];
    }
    return output;
}

bitset<64> round(bitset<64> input, bitset<48> ki)
{
    bitset<64> output;
    bitset<32> previousLeftPart;
    bitset<32> leftPart;
    bitset<32> rightPart;
    
    for (int i = 0; i < 32; i++)
    {
        previousLeftPart[31 - i] = input[63 - i];
    }
    
    for (int i = 0; i < 32; i++)
    {
        leftPart[31 - i] = input[31 - i];
    }
    
    rightPart = previousLeftPart ^ F(leftPart, ki);
    
    for (int i = 0; i < 32; i++)
    {
        output[63 - i] = leftPart[31 - i];
    }
    
    for (int i = 32; i < 64; i++)
    {
        output[63 - i] = rightPart[63 - i];
    }
    return output;
}

bitset<56> getKeyPermutation(bitset<64> key)
{
    bitset<56> output;
    for (int i = 0; i < 56; i++)
    {
        output[55 - i] = key[64 - PC_1[i]];
    }
    return output;
}

int mod28(int a)
{
    if (a >= 0)
    {
        return a % 28;
    }
    else
    {
        return 28 - (-a) % 28;
    }
}

bitset<56> shiftKey(bitset<56> key, int round, ShiftStyle shiftStyle)
{
    bitset<56> output;
    bitset<28> previousLeftPart;
    for (int i = 0; i < 28; i++)
    {
        previousLeftPart[27 - i] = key[55 - i];
    }
    bitset<28> previousRightPart;
    for (int i = 0; i < 28; i++)
    {
        previousRightPart[27 - i] = key[27 - i];
    }
    int *shift;
    switch (shiftStyle)
    {
        case normalShift:
            shift = shiftBits;
            break;
            
        case inverseShift:
            shift = inverseShiftBits;
            break;
            
        default:
            break;
    }
    
    bitset<28> leftPart;
    bitset<28> rightPart;
    int shiftBit = shift[round];
    switch (shiftStyle)
    {
        case normalShift:
            for (int i = 0; i < 28; i++)
            {
                leftPart[27 - i] = previousLeftPart[mod28(27 - i - shiftBit)];
                rightPart[27 - i] = previousRightPart[mod28(27 - i - shiftBit)];
            }
            break;
            
        case inverseShift:
            for (int i = 0; i < 28; i++)
            {
                leftPart[27 - i] = previousLeftPart[mod28(27 - i + shiftBit)];
                rightPart[27 - i] = previousRightPart[mod28(27 - i + shiftBit)];
            }
            break;
            
        default:
            break;
    }
    for (int i = 0; i < 28; i++)
    {
        output[55 - i] = leftPart[27 - i];
    }
    for (int i = 28; i < 56; i++)
    {
        output[55 - i] = rightPart[55 - i];
    }
    return output;
}

bitset<48> getSubkey(bitset<56> key, int round, ShiftStyle shiftStyle)
{
    bitset<48> output;
    for (int i = 0; i < 48; i++)
    {
        output[47 - i] = key[56 - PC_2[i]];
    }
    return output;
}

bitset<64> exchangeLeftAndRight(bitset<64> input)
{
    bitset<64> output;
    for (int i = 0; i < 32; i++)
    {
        output[63 - i] = input[31 - i];
    }
    for (int i = 32; i < 64; i++)
    {
        output[63 - i] = input[95 - i];
    }
    return output;
}

bitset<64> getInversePermutation(bitset<64> input)
{
    bitset<64> output;
    for (int i = 0; i < 64; i++)
    {
        output[63 - i] = input[64 - IP_1[i]];
    }
    return output;
}

bitset<64> DES_ENC(bitset<64> plainText, bitset<64> key)
{
    bitset<64> cipher;
    bitset<64> roundInput = getInitialPermutation(plainText);
    bitset<64> roundOutput;
    bitset<56> permutatedKey = getKeyPermutation(key);
    bitset<56> previousShiftOutput = shiftKey(permutatedKey, 0, normalShift);
    for (int i = 0; i < 16; i++)
    {
        bitset<48> subkey = getSubkey(previousShiftOutput, i, normalShift);
        roundOutput = round(roundInput, subkey);
        roundInput = roundOutput;
        previousShiftOutput = shiftKey(previousShiftOutput, i + 1, normalShift);
    }
    cipher = getInversePermutation(exchangeLeftAndRight(roundOutput));
    return cipher;
}

bitset<64> DES_DEC(bitset<64> cipher, bitset<64> key)
{
    bitset<64> plainText;
    bitset<64> roundInput = getInitialPermutation(cipher);
    bitset<64> roundOutput;
    bitset<56> permutatedKey = getKeyPermutation(key);
    bitset<56> previousShiftOutput = shiftKey(permutatedKey, 0, inverseShift);
    for (int i = 0; i < 16; i++)
    {
        bitset<48> subkey = getSubkey(previousShiftOutput, i, inverseShift);
        roundOutput = round(roundInput, subkey);
        roundInput = roundOutput;
        previousShiftOutput = shiftKey(previousShiftOutput, i + 1, inverseShift);
    }
    plainText = getInversePermutation(exchangeLeftAndRight(roundOutput));
    return plainText;
}

bitset<64> tripleDES_ENC(bitset<64> plainText, bitset<64> key1, bitset<64> key2)
{
    return DES_ENC(DES_DEC(DES_ENC(plainText, key1), key2), key1);
}

bitset<64> tripleDES_DEC(bitset<64> cipher, bitset<64> key1, bitset<64> key2)
{
    return DES_DEC(DES_ENC(DES_DEC(cipher, key1), key2), key1);
}

int main(int argc, const char * argv[])
{
    bitset<64> input(0x85E813540F0AB405);
    bitset<64> key(0x133457799BBCDFF1);
    bitset<64> output = DES_DEC(input, key);
    cout << hex << output.to_ulong() << endl;
    return 0;
}
