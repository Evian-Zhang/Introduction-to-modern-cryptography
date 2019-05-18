//
//  main.cpp
//  MD5
//
//  Created by Evian张 on 2019/5/12.
//  Copyright © 2019 Evian张. All rights reserved.
//

#include <iostream>
#include <math.h>

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
    BigInteger bitSlice[8];
    for (int byte = 0; byte < 8; byte++)
    {
        bitSlice[byte] = appendBigInt.slice(8 * byte, 8 * (byte + 1));
        appendedText.append(bitSlice[7 - byte]);
    }
    return appendedText;
}

BigInteger A, B, C, D;
BigInteger T[64];
void initializeBuffer()
{
    A = BigInteger("0x67452301");
    B = BigInteger("0xefcdab89");
    C = BigInteger("0x98badcfe");
    D = BigInteger("0x10325476");


    for (int i = 0; i < 64; i++)
    {
        T[i] = BigInteger((unsigned int)(4294967296 * fabs(sin(i + 1))));
        T[i].limitTo(32);
    }
}

BigInteger F(BigInteger X, BigInteger Y, BigInteger Z)
{
    return (X & Y) | (~X & Z);
}

BigInteger G(BigInteger X, BigInteger Y, BigInteger Z)
{
    return (X & Z) | (Y & ~Z);
}

BigInteger H(BigInteger X, BigInteger Y, BigInteger Z)
{
    return (X ^ Y) ^ Z;
}

BigInteger I(BigInteger X, BigInteger Y, BigInteger Z)
{
    return Y ^ (X | ~Z);
}

BigInteger X[16];

void FF(BigInteger &A, BigInteger B, BigInteger C, BigInteger D, 
        int k, int s, int i)
{
    BigInteger tmp = A + F(B, C, D) + X[k] + T[i];
    tmp.limitTo(32);
    A = B + tmp.rotLeft(s);
    A.limitTo(32);
}

void GG(BigInteger &A, BigInteger B, BigInteger C, BigInteger D, int k, int s, int i)
{
    BigInteger tmp = A + G(B, C, D) + X[k] + T[i];
    tmp.limitTo(32);
    A = B + tmp.rotLeft(s);
    A.limitTo(32);
}

void HH(BigInteger &A, BigInteger B, BigInteger C, BigInteger D, int k, int s, int i)
{
    BigInteger tmp = A + H(B, C, D) + X[k] + T[i];
    tmp.limitTo(32);
    A = B + tmp.rotLeft(s);
    A.limitTo(32);
}

void II(BigInteger &A, BigInteger B, BigInteger C, BigInteger D, int k, int s, int i)
{
    BigInteger tmp = A + I(B, C, D) + X[k] + T[i];
    tmp.limitTo(32);
    A = B + tmp.rotLeft(s);
    A.limitTo(32);
}

BigInteger processMessage(BigInteger message)
{
    int N = message.getLength() / 32;
    for (int i = 0; i < N / 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            X[j] = message.slice(32 * (i * 16 + j), 32 * (i * 16 + j + 1));
            BigInteger bitSlice[4] = {X[j].slice(0, 8), X[j].slice(8, 16), X[j].slice(16, 24), X[j].slice(24, 32)};
            X[j] = bitSlice[3];
            X[j].append(bitSlice[2]);
            X[j].append(bitSlice[1]);
            X[j].append(bitSlice[0]);
        }
        
        BigInteger AA = A, BB = B, CC = C, DD = D;
        FF(A, B, C, D, 0, 7, 0);
        FF(D, A, B, C, 1, 12, 1);
        FF(C, D, A, B, 2, 17, 2);
        FF(B, C, D, A, 3, 22, 3);
        FF(A, B, C, D, 4, 7, 4);
        FF(D, A, B, C, 5, 12, 5);
        FF(C, D, A, B, 6, 17, 6);
        FF(B, C, D, A, 7, 22, 7);
        FF(A, B, C, D, 8, 7, 8);
        FF(D, A, B, C, 9, 12, 9);
        FF(C, D, A, B, 10, 17, 10);
        FF(B, C, D, A, 11, 22, 11);
        FF(A, B, C, D, 12, 7, 12);
        FF(D, A, B, C, 13, 12, 13);
        FF(C, D, A, B, 14, 17, 14);
        FF(B, C, D, A, 15, 22, 15);
        
        GG(A, B, C, D, 1, 5, 16);
        GG(D, A, B, C, 6, 9, 17);
        GG(C, D, A, B, 11, 14, 18);
        GG(B, C, D, A, 0, 20, 19);
        GG(A, B, C, D, 5, 5, 20);
        GG(D, A, B, C, 10, 9 ,21);
        GG(C, D, A, B, 15, 14, 22);
        GG(B, C, D, A, 4, 20, 23);
        GG(A, B, C, D, 9, 5, 24);
        GG(D, A, B, C, 14, 9 ,25);
        GG(C, D, A, B, 3, 14, 26);
        GG(B, C, D, A, 8, 20, 27);
        GG(A, B, C, D, 13, 5, 28);
        GG(D, A, B, C, 2, 9 ,29);
        GG(C, D, A, B, 7, 14, 30);
        GG(B, C, D, A, 12, 20, 31);
        
        HH(A, B, C, D, 5, 4, 32);
        HH(D, A, B, C, 8, 11, 33);
        HH(C, D, A, B, 11, 16,34);
        HH(B, C, D, A, 14, 23, 35);
        HH(A, B, C, D, 1, 4, 36);
        HH(D, A, B, C, 4, 11, 37);
        HH(C, D, A, B, 7, 16, 38);
        HH(B, C, D, A, 10, 23, 39);
        HH(A, B, C, D, 13, 4, 40);
        HH(D, A, B, C, 0, 11, 41);
        HH(C, D, A, B, 3, 16, 42);
        HH(B, C, D, A, 6, 23, 43);
        HH(A, B, C, D, 9, 4, 44);
        HH(D, A, B, C, 12, 11, 45);
        HH(C, D, A, B, 15, 16, 46);
        HH(B, C, D, A, 2, 23, 47);
        
        II(A, B, C, D, 0, 6, 48);
        II(D, A, B, C, 7, 10, 49);
        II(C, D, A, B, 14, 15, 50);
        II(B, C, D, A, 5, 21, 51);
        II(A, B, C, D, 12, 6, 52);
        II(D, A, B, C, 3, 10, 53);
        II(C, D, A, B, 10, 15, 54);
        II(B, C, D, A, 1, 21, 55);
        II(A, B, C, D, 8, 6, 56);
        II(D, A, B, C, 15, 10, 57);
        II(C, D, A, B, 6, 15, 58);
        II(B, C, D, A, 13, 21, 59);
        II(A, B, C, D, 4, 6, 60);
        II(D, A, B, C, 11, 10, 61);
        II(C, D, A, B, 2, 15, 62);
        II(B, C, D, A, 9, 21, 63);
        
        A = A + AA;
        A.limitTo(32);
        
        B = B + BB;
        B.limitTo(32);
        
        C = C + CC;
        C.limitTo(32);
        
        D = D + DD;
        D.limitTo(32);
    }
    
    BigInteger output = A.slice(24, 32);
    output.append(A.slice(16, 24));
    output.append(A.slice(8, 16));
    output.append(A.slice(0, 8));
    output.append(B.slice(24, 32));
    output.append(B.slice(16, 24));
    output.append(B.slice(8, 16));
    output.append(B.slice(0, 8));
    output.append(C.slice(24, 32));
    output.append(C.slice(16, 24));
    output.append(C.slice(8, 16));
    output.append(C.slice(0, 8));
    output.append(D.slice(24, 32));
    output.append(D.slice(16, 24));
    output.append(D.slice(8, 16));
    output.append(D.slice(0, 8));
    return output;
}

BigInteger MD5_hash(BigInteger plainText)
{
    BigInteger appendedText = appendPaddingBits(plainText);
    BigInteger message = appendLength(appendedText, 
                                        plainText.getLength());
    initializeBuffer();
    return processMessage(message);
}

int main()
{
    BigInteger plainText = BigInteger::bigIntegerFromASCIIString("southeastuniversitysoutheastuniversitysoutheastuniversIty");
    BigInteger hash = MD5_hash(message);
    cout << hash.hexString() << endl;
    return 0;
}
