//
//  main.cpp
//  Poly1305
//
//  Created by Evian张 on 2019/5/22.
//  Copyright © 2019 Evian张. All rights reserved.
//

#include <iostream>

#include "BigInteger.hpp"
using namespace std;

void changeEndian(BigInteger &bigInt)
{
    const int N = bigInt.getLength() / 8;
    BigInteger bitSlice[N];
    for (int i = 0; i < N; i++)
        bitSlice[i] = bigInt.slice(i * 8, (i + 1) * 8);
    bigInt = bitSlice[N - 1];
    int byte = N - 2;
    while (byte >= 0)
    {
        bigInt.append(bitSlice[byte]);
        byte--;
    }
}

void clampR(BigInteger &r)
{
    r &= BigInteger::bigIntegerFromHexString("0ffffffc0ffffffc0ffffffc0fffffff");
}

void processBlock(BigInteger &block)
{
    const int N = block.getLength() / 8;
    changeEndian(block);
    BigInteger addByte = BigInteger(1) << block.getLength();
    block += addByte;
    if (block.getLength() < 136)
    {
        block.limitTo(136);
    }
}

BigInteger Poly1305_MAC(BigInteger plainText, BigInteger key)
{
    BigInteger r = key.slice(0, 128);
    changeEndian(r);
    clampR(r);
    BigInteger s = key.slice(128, 256);
    changeEndian(s);
    BigInteger accumulator = 0;
    BigInteger p = (BigInteger(1) << 130) - 5;
    int length = plainText.getLength();
    int N = length / 128;
    if (length % 128 != 0)
        N++;
    for (int i = 0; i < N; i++)
    {
        int upper = (i + 1) * 128;
        if (upper > length)
            upper = length;
        BigInteger block = plainText.slice(i * 128, upper);
        processBlock(block);
        accumulator += block;
        accumulator = (r * accumulator) % p;
    }
    accumulator += s;
    BigInteger tag = accumulator;
    tag.limitTo(128);
    changeEndian(tag);
    return tag;
}

int main(int argc, const char * argv[]) {
    BigInteger plainText = BigInteger::bigIntegerFromHexString("43727970746f6772617068696320466f72756d2052657365617263682047726f7570");
    BigInteger key = BigInteger::bigIntegerFromHexString("85d6be7857556d337f4452fe42d506a80103808afb0db2fd4abff6af4149f51b");
    BigInteger mac = Poly1305_MAC(plainText, key);
    cout << mac.hexString() << endl;
    return 0;
}
