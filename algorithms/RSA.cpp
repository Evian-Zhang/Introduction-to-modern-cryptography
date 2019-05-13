//
//  main.cpp
//  RSA
//
//  Created by Evian张 on 2019/4/12.
//  Copyright © 2019 Evian张. All rights reserved.
//

#include <iostream>
#include <cstdlib>

#include "BigInteger.hpp"

using namespace std;

bool Miller_Rabin(BigInteger n, int round)
{
    BigInteger m = n - 1;
    int k = 0;
    while (!m.getValue()[0])
    {
        k++;
        m = m >> 1;
    }
    
    for (int i = 0; i < round; i++)
    {
        BigInteger a = BigInteger::getRand() % (n - 1) + 1;
        BigInteger b = BigInteger::fastExp(a, m, n);
        if (b == 1)
            return true;
        
        for (int j = 0; j < k; j++)
        {
            if (b == n - 1)
                return true;
            b = BigInteger::mulmod(b, b, n);
        }
    }
    return false;
}

void generatePrimes(BigInteger &p, BigInteger &q)
{
    p = BigInteger::getRandBit(1024);
    if (!p.getValue()[0])
        p += 1;
    
    while (!Miller_Rabin(p, 40))
        p += 2;
    
    
    q = BigInteger::getRandBit(1024);
    if (!q.getValue()[0])
        q += 1;
    
    while (!Miller_Rabin(q, 40))
        q += 2;
}

BigInteger generateE(BigInteger phi)
{
    BigInteger list[5] = {65537, 257, 17, 5, 3};
    for (int i = 0; i < 5; i++)
        if (BigInteger::gcd(phi, list[i]) == 1)
            return list[i];
    
    return -1;
}

BigInteger generateD(BigInteger e, BigInteger phi)
{
    return BigInteger::inverse(e, phi);
}

BigInteger RSA_ENC(BigInteger plainText, BigInteger e, BigInteger n)
{
    return BigInteger::fastExp(plainText, e, n);
}

BigInteger RSA_DEC(BigInteger cipher, BigInteger d, BigInteger p, BigInteger q)
{
    BigInteger dp = d % (p - 1);
    BigInteger dq = d % (q - 1);
    
    BigInteger mp = BigInteger::fastExp(cipher, dp, p);
    BigInteger mq = BigInteger::fastExp(cipher, dq, q);
    
    BigInteger ep = BigInteger::inverse(q, p);
    BigInteger eq = BigInteger::inverse(p, q);
    
    BigInteger n = p * q;
    
    BigInteger tmp1 = BigInteger::mulmod(q, ep, n);
    tmp1 = BigInteger::mulmod(tmp1, mp, n);
    
    BigInteger tmp2 = BigInteger::mulmod(p, eq, n);
    tmp2 = BigInteger::mulmod(tmp2, mq, n);
    
    return (tmp1 + tmp2) % mod;
}

int main(int argc, const char * argv[])
{
    BigInteger p = 0;
    BigInteger q = 0;
    generatePrimes(p, q);
    
    BigInteger n = p * q;
    BigInteger phi = (p - 1) * (q - 1);
    BigInteger e = generateE(phi);
    BigInteger d = generateD(e, phi);
    
    BigInteger plainText = "0xabc";
    
    BigInteger cipher = RSA_ENC(plainText, e, n);
    
    cout << RSA_DEC(cipher, d, p, q) << endl;
    
    return 0;
}
