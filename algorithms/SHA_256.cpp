//
//  main.cpp
//  SHA_256
//
//  Created by Evian张 on 2019/5/17.
//  Copyright © 2019 Evian张. All rights reserved.
//

#include <iostream>
#include "BigInteger.hpp"
using namespace std;

BigInteger appendPaddingBits(BigInteger plainText)
{
    int spareLength = plainText.getLength() % 512;
    if (spareLength < 448)
    {
        int appendLength = 448 - spareLength;
        bool *appendValue = new bool[appendLength];
        *(appendValue + appendLength - 1) = true;
        int index = appendLength - 2;
        while (index > -1)
        {
            *(appendValue + index) = false;
            index--;
        }
        plainText.append(appendValue, appendLength);
    }
    else if (spareLength > 448)
    {
        int appendLength = 512 - spareLength + 448;
        bool *appendValue = new bool[appendLength];
        *(appendValue + appendLength - 1) = true;
        int index = appendLength - 2;
        while (index > -1)
        {
            *(appendValue + index) = false;
            index--;
        }
        plainText.append(appendValue, appendLength);
    }
    return plainText;
}

BigInteger appendLength(BigInteger appendedText, int textLength)
{
    BigInteger appendBigInt(textLength);
    appendBigInt.limitTo(64);
    appendedText.append(appendBigInt);
    return appendedText;
}

BigInteger CH(BigInteger x, BigInteger y, BigInteger z)
{
    return (x & y) ^ (~x & z);
}

BigInteger MAJ(BigInteger x, BigInteger y, BigInteger z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

BigInteger BSIG0(BigInteger x)
{
    return x.rotRight(2) ^ x.rotRight(13) ^ x.rotRight(22);
}

BigInteger BSIG1(BigInteger x)
{
    return x.rotRight(6) ^ x.rotRight(11) ^ x.rotRight(25);
}

BigInteger SSIG0(BigInteger x)
{
    BigInteger shifted = x >> 3;
    shifted.limitTo(32);
    return x.rotRight(7) ^ x.rotRight(18) ^ shifted;
}

BigInteger SSIG1(BigInteger x)
{
    BigInteger shifted = x >> 10;
    shifted.limitTo(32);
    return x.rotRight(17) ^ x.rotRight(19) ^ shifted;
}

BigInteger K[64];
BigInteger H[8];

void initializeKH()
{
    K[0] = BigInteger("0x428a2f98");
    K[1] = BigInteger("0x71374491");
    K[2] = BigInteger("0xb5c0fbcf");
    K[3] = BigInteger("0xe9b5dba5");
    K[4] = BigInteger("0x3956c25b");
    K[5] = BigInteger("0x59f111f1");
    K[6] = BigInteger("0x923f82a4");
    K[7] = BigInteger("0xab1c5ed5");
    K[8] = BigInteger("0xd807aa98");
    K[9] = BigInteger("0x12835b01");
    K[10] = BigInteger("0x243185be");
    K[11] = BigInteger("0x550c7dc3");
    K[12] = BigInteger("0x72be5d74");
    K[13] = BigInteger("0x80deb1fe");
    K[14] = BigInteger("0x9bdc06a7");
    K[15] = BigInteger("0xc19bf174");
    K[16] = BigInteger("0xe49b69c1");
    K[17] = BigInteger("0xefbe4786");
    K[18] = BigInteger("0x0fc19dc6");
    K[19] = BigInteger("0x240ca1cc");
    K[20] = BigInteger("0x2de92c6f");
    K[21] = BigInteger("0x4a7484aa");
    K[22] = BigInteger("0x5cb0a9dc");
    K[23] = BigInteger("0x76f988da");
    K[24] = BigInteger("0x983e5152");
    K[25] = BigInteger("0xa831c66d");
    K[26] = BigInteger("0xb00327c8");
    K[27] = BigInteger("0xbf597fc7");
    K[28] = BigInteger("0xc6e00bf3");
    K[29] = BigInteger("0xd5a79147");
    K[30] = BigInteger("0x06ca6351");
    K[31] = BigInteger("0x14292967");
    K[32] = BigInteger("0x27b70a85");
    K[33] = BigInteger("0x2e1b2138");
    K[34] = BigInteger("0x4d2c6dfc");
    K[35] = BigInteger("0x53380d13");
    K[36] = BigInteger("0x650a7354");
    K[37] = BigInteger("0x766a0abb");
    K[38] = BigInteger("0x81c2c92e");
    K[39] = BigInteger("0x92722c85");
    K[40] = BigInteger("0xa2bfe8a1");
    K[41] = BigInteger("0xa81a664b");
    K[42] = BigInteger("0xc24b8b70");
    K[43] = BigInteger("0xc76c51a3");
    K[44] = BigInteger("0xd192e819");
    K[45] = BigInteger("0xd6990624");
    K[46] = BigInteger("0xf40e3585");
    K[47] = BigInteger("0x106aa070");
    K[48] = BigInteger("0x19a4c116");
    K[49] = BigInteger("0x1e376c08");
    K[50] = BigInteger("0x2748774c");
    K[51] = BigInteger("0x34b0bcb5");
    K[52] = BigInteger("0x391c0cb3");
    K[53] = BigInteger("0x4ed8aa4a");
    K[54] = BigInteger("0x5b9cca4f");
    K[55] = BigInteger("0x682e6ff3");
    K[56] = BigInteger("0x748f82ee");
    K[57] = BigInteger("0x78a5636f");
    K[58] = BigInteger("0x84c87814");
    K[59] = BigInteger("0x8cc70208");
    K[60] = BigInteger("0x90befffa");
    K[61] = BigInteger("0xa4506ceb");
    K[62] = BigInteger("0xbef9a3f7");
    K[63] = BigInteger("0xc67178f2");
    
    H[0] = BigInteger("0x6a09e667");
    H[1] = BigInteger("0xbb67ae85");
    H[2] = BigInteger("0x3c6ef372");
    H[3] = BigInteger("0xa54ff53a");
    H[4] = BigInteger("0x510e527f");
    H[5] = BigInteger("0x9b05688c");
    H[6] = BigInteger("0x1f83d9ab");
    H[7] = BigInteger("0x5be0cd19");
}

BigInteger processMessage(BigInteger message)
{
    int N = message.getLength() / 512;
    for (int i = 0; i < N; i++)
    {
        BigInteger Mi = message.slice(512 * (N - i - 1), 512 * (N - i));
        BigInteger W[64];
        for (int t = 0; t < 16; t++)
            W[t] = Mi.slice(32 * (15 - t), 32 * (16 - t));
        for (int t = 16; t < 64; t++)
        {
            W[t] = SSIG1(W[t - 2]) + W[t - 7] + SSIG0(W[t - 15]) + W[t - 16];
            W[t].limitTo(32);
        }
        
        BigInteger a = H[0];
        BigInteger b = H[1];
        BigInteger c = H[2];
        BigInteger d = H[3];
        BigInteger e = H[4];
        BigInteger f = H[5];
        BigInteger g = H[6];
        BigInteger h = H[7];
        
        for (int t = 0; t < 64; t++)
        {
            BigInteger T1 = h + BSIG1(e) + CH(e, f, g) + K[t] + W[t];
            BigInteger T2 = BSIG0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            e.limitTo(32);
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
            a.limitTo(32);
        }
        
        H[0] += a;
        H[0].limitTo(32);
        H[1] += b;
        H[1].limitTo(32);
        H[2] += c;
        H[2].limitTo(32);
        H[3] += d;
        H[3].limitTo(32);
        H[4] += e;
        H[4].limitTo(32);
        H[5] += f;
        H[5].limitTo(32);
        H[6] += g;
        H[6].limitTo(32);
        H[7] += h;
        H[7].limitTo(32);
    }
    BigInteger output = H[0];
    output.append(H[1]);
    output.append(H[2]);
    output.append(H[3]);
    output.append(H[4]);
    output.append(H[5]);
    output.append(H[6]);
    output.append(H[7]);
    return output;
}

int main()
{
    BigInteger plainText = BigInteger::bigIntegerFromASCIIString("southeastuniversitysoutheastuniversitysoutheastuniversIty");
    BigInteger appendedText = appendPaddingBits(plainText);
    BigInteger message = appendLength(appendedText, plainText.getLength());
    initializeKH();
    BigInteger hash = processMessage(message);
    cout << hash.hexString() << endl;
    return 0;
}
