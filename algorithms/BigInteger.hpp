//
//  BigInteger.hpp
//  RSA
//
//  Created by Evian张 on 2019/4/12.
//  Copyright © 2019 Evian张. All rights reserved.
//

#ifndef BigInteger_hpp
#define BigInteger_hpp

#include <iostream>

class BigInteger
{
    int length;
    bool *value;
    bool sign;
    
    BigInteger(int length, bool *value, bool sign);
    
    static BigInteger extendGcd(BigInteger x, BigInteger y, BigInteger &a, BigInteger &b);
    
public:
    BigInteger();
    BigInteger(const int number);
    BigInteger(char *str);
    BigInteger(const BigInteger &bigInt);
    
    ~BigInteger();
    
    friend std::ostream& operator<<(std::ostream &output, const BigInteger &bigInt);
    bool *getValue() const;
    
    bool operator<(const BigInteger &bigInt) const;
    bool operator>(const BigInteger &bigInt) const;
    bool operator==(const BigInteger &bigInt) const;
    bool operator<=(const BigInteger &bigInt) const;
    bool operator>=(const BigInteger &bigInt) const;
    bool operator!=(const BigInteger &bigInt) const;
    
    BigInteger operator<<(const int n) const;
    BigInteger operator>>(const int n) const;
    
    BigInteger operator+(const BigInteger &bigInt) const;
    BigInteger operator-(const BigInteger &bigInt) const;
    BigInteger operator*(const BigInteger &bigInt) const;
    BigInteger operator/(const BigInteger &bigInt) const;
    BigInteger operator%(const BigInteger &bigInt) const;
    
    BigInteger &operator=(const BigInteger &bigInt);
    BigInteger &operator+=(const BigInteger &bigInt);
    BigInteger &operator-=(const BigInteger &bigInt);
    BigInteger &operator*=(const BigInteger &bigInt);
    BigInteger &operator/=(const BigInteger &bigInt);
    
    static BigInteger getRand();
    
    static BigInteger getRandBit(int length);
    
    static BigInteger mulmod(const BigInteger mul1, const BigInteger mul2, const BigInteger mod);
    
    static BigInteger fastExp(const BigInteger base, const BigInteger exponent, const BigInteger mod);
    
    static BigInteger gcd(const BigInteger a, const BigInteger b);
    
    static BigInteger inverse(const BigInteger a, const BigInteger mod);
};

#endif /* BigInteger_hpp */
