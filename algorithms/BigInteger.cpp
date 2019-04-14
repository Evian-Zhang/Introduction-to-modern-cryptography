//
//  BigInteger.cpp
//  RSA
//
//  Created by Evian张 on 2019/4/12.
//  Copyright © 2019 Evian张. All rights reserved.
//

#include "BigInteger.hpp"

BigInteger::BigInteger()
{
    this->length = 0;
    this->value = NULL;
    this->sign = true;
}

BigInteger::BigInteger(int length, bool *value, bool sign)
{
    this->length = length;
    this->value = value;
    this->sign = sign;
}

BigInteger::BigInteger(const int number)
{
    if (!number)
    {
        this->length = 0;
        this->value = NULL;
        this->sign = true;
    }
    else
    {
        int tmp1 = number;
        int tmp2 = number;
        
        if (number > 0)
            this->sign = true;
        else
        {
            this->sign = false;
            tmp1 = -number;
            tmp2 = tmp1;
        }
        
        this->length = 1;
        while (tmp1 >= 2)
        {
            this->length++;
            tmp1 /= 2;
        }
        
        this->value = new bool[this->length];
        for (int digit = 0; digit < this->length; digit++)
        {
            *(this->value + digit) = tmp2 % 2;
            tmp2 /= 2;
        }
    }
}

BigInteger::BigInteger(char *str)
{
    char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    bool first[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    bool second[16] = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0 ,0, 1, 1};
    bool third[16] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};
    bool fourth[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
    
    int length = strlen(str);
    if (length > 2 && str[0] == '0' && str[1] == 'x')
    {
        bool *value = new bool[4 * (length - 2)];
        for (int ch = length - 1; ch >= 2; ch--)
            for (int index = 0; index < 16; index++)
                if (str[ch] == hex[index])
                {
                    value[4 * (length - ch - 1)] = first[index];
                    value[4 * (length - ch - 1) + 1] = second[index];
                    value[4 * (length - ch - 1) + 2] = third[index];
                    value[4 * (length - ch - 1) + 3] = fourth[index];
                    break;
                }
        
        this->length = 4 * (length - 2);
        this->value = value;
        this->sign = true;
    }
    else
    {
        bool *value = new bool[length];
        for (int digit = 0; digit < length; digit++)
        {
            if (str[length - digit - 1] == '0')
                value[digit] = false;
            else
                value[digit] = true;
        }
        
        this->length = length;
        this->value = value;
        this->sign = true;
    }
}

BigInteger::BigInteger(const BigInteger &bigInt)
{
    this->sign = bigInt.sign;
    this->length = bigInt.length;
    if (this->length)
    {
        this->value = new bool[this->length];
        for (int digit = 0; digit < this->length; digit++)
        {
            *(this->value + digit) = bigInt.value[digit];
        }
    }
    else
        this->value = NULL;
}

BigInteger::~BigInteger()
{
    if (this->length && this->value)
        delete[] this->value;
}

std::ostream& operator<<(std::ostream &output, const BigInteger &bigInt)
{
    if (!bigInt.sign)
        output << "-";
    if (!bigInt.length)
    {
        output << "0";
        return output;
    }
    for (int digit = bigInt.length - 1; digit >= 0; digit--)
    {
        if (bigInt.value[digit])
            output << "1";
        else
            output << "0";
    }
    return output;
}

bool *BigInteger::getValue() const
{
    return this->value;
}

bool BigInteger::operator<(const BigInteger &bigInt) const
{
    if (!this->sign && bigInt.sign)
        return true;
    if (this->sign && !bigInt.sign)
        return false;
    
    if (this->length < bigInt.length)
        return this->sign;
    if (this->length > bigInt.length)
        return !this->sign;
    
    if (!this->length)
        return false;
    
    for (int digit = this->length - 1; digit >= 0; digit--)
    {
        if (*(this->value + digit) > bigInt.value[digit])
            return !this->sign;
        else if (*(this->value + digit) < bigInt.value[digit])
            return this->sign;
    }
    
    return false;
}

bool BigInteger::operator>(const BigInteger &bigInt) const
{
    return !(bigInt <= *this);
}

bool BigInteger::operator==(const BigInteger &bigInt) const
{
    if (this->sign == bigInt.sign && this->length == bigInt.length)
    {
        if (!this->length)
            return true;
        for (int digit = this->length - 1; digit >= 0; digit--)
            if (*(this->value + digit) != bigInt.value[digit])
                return false;
        return true;
    }
    
    return false;
}

bool BigInteger::operator<=(const BigInteger &bigInt) const
{
    if (!this->sign && bigInt.sign)
        return true;
    if (this->sign && !bigInt.sign)
        return false;
    
    if (this->length < bigInt.length)
        return this->sign;
    if (this->length > bigInt.length)
        return !this->sign;
    
    if (!this->length)
        return true;
    for (int digit = this->length - 1; digit >= 0; digit--)
    {
        if (*(this->value + digit) > bigInt.value[digit])
            return !this->sign;
        else if (*(this->value + digit) < bigInt.value[digit])
            return this->sign;
    }
    
    return true;
}

bool BigInteger::operator>=(const BigInteger &bigInt) const
{
    return !(*this < bigInt);
}

bool BigInteger::operator!=(const BigInteger &bigInt) const
{
    return !(*this == bigInt);
}

BigInteger BigInteger::operator<<(const int n) const
{
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    int length = this->length + n;
    
    bool *value = new bool[length];
    for (int digit = 0; digit < n; digit++)
        value[digit] = false;
    for (int digit = n; digit < length; digit++)
        value[digit] = *(this->value + digit - n);
    
    return BigInteger(length, value, this->sign);
}

BigInteger BigInteger::operator>>(const int n) const
{
    if (this->length <= n)
        return BigInteger(0, NULL, true);
    
    int length = this->length - n;
    
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        value[digit] = *(this->value + digit + n);
    
    return BigInteger(length, value, this->sign);
}

BigInteger BigInteger::operator+(const BigInteger &bigInt) const
{
    if (this->sign && !bigInt.sign)
    {
        BigInteger tmp(bigInt);
        tmp.sign = true;
        return this->operator-(tmp);
    }
    if (!this->sign && bigInt.sign)
    {
        BigInteger tmp(*this);
        tmp.sign = true;
        return bigInt.operator-(tmp);
    }
    
    int length = this->length > bigInt.length ? this->length : bigInt.length;
    if (!length)
        return BigInteger(0, NULL, true);
    length++;
    
    bool *value = new bool[length];
    bool overflow = false;
    for (int digit = 0; digit < length - 1; digit++)
    {
        bool adder1 = false;
        bool adder2 = false;
        if (digit < this->length)
            adder1 = this->value[digit];
        if (digit < bigInt.length)
            adder2 = bigInt.value[digit];
        *(value + digit) = adder1 ^ adder2 ^ overflow;
        overflow = (adder1 && adder2) || (adder1 && overflow) || (adder2 && overflow);
    }
    if (overflow)
        value[length - 1] = true;
    else
        length--;
    
    return BigInteger(length, value, this->sign);
}

BigInteger BigInteger::operator-(const BigInteger &bigInt) const
{
    if (this->sign && !bigInt.sign)
    {
        BigInteger tmp(bigInt);
        tmp.sign = true;
        return this->operator+(tmp);
    }
    if (*this < bigInt)
    {
        BigInteger result = bigInt - *this;
        result.sign = !result.sign;
        return result;
    }
    
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    bool *value = new bool[this->length];
    bool carry = false;
    for (int digit = 0; digit < this->length; digit++)
    {
        bool minuend = false;
        bool subtrahend = false;
        if (digit < this->length)
            minuend = *(this->value + digit);
        if (digit < bigInt.length)
            subtrahend = bigInt.value[digit];
        
        *(value + digit) = (!minuend && !subtrahend && carry) || (!minuend && subtrahend && !carry) || (minuend && !subtrahend && !carry) || (minuend && subtrahend && carry);
        carry = (!minuend && !(!subtrahend && !carry)) || (minuend && subtrahend && carry);
    }
    
    int length = this->length;
    for (int digit = this->length - 1; digit >= 0; digit--)
    {
        if (value[digit])
            break;
        else
            length--;
    }
    
    return BigInteger(length, value, this->sign);
}

BigInteger BigInteger::operator*(const BigInteger &bigInt) const
{
    if (this->length == 0 || bigInt.length == 0)
        return BigInteger(0, NULL, true);
    
    BigInteger result(0, NULL, true);
    for (int digit = 0; digit < bigInt.length; digit++)
        if (bigInt.value[digit])
        {
            BigInteger tmp = *this << digit;
            result = result + tmp;
        }
    
    result.sign = (this->sign && bigInt.sign) || (!this->sign && !bigInt.sign);
    
    return result;
}

BigInteger BigInteger::operator/(const BigInteger &bigInt) const
{
    if (!this->length || this->length < bigInt.length)
        return BigInteger(0, NULL, true);
    
    int length = this->length - bigInt.length + 1;
    
    bool *value = new bool[length];
    BigInteger remainder(*this);
    BigInteger divisor(bigInt);
    remainder.sign = true;
    divisor.sign = true;
    for (int digit = length - 1; digit >= 0; digit--)
    {
        if (!remainder.length)
        {
            for (int i = digit; i >= 0; i--)
                value[i] = false;
            break;
        }
        
        BigInteger tmp(divisor << digit);
        if (remainder < tmp)
            value[digit] = false;
        else
        {
            value[digit] = true;
            remainder -= tmp;
        }
    }
    
    for (int digit = length - 1; digit >= 0; digit--)
    {
        if (!value[digit])
            length--;
        else
            break;
    }
    
    bool sign = (this->sign && bigInt.sign) || (!this->sign && !bigInt.sign);
    
    return BigInteger(length, value, sign);
}

BigInteger BigInteger::operator%(const BigInteger &bigInt) const
{
    BigInteger remainder(*this);
    BigInteger divisor(bigInt);
    remainder.sign = true;
    divisor.sign = true;
    for (int digit = length - 1; digit >= 0; digit--)
    {
        if (!remainder.length)
        {
            for (int i = digit; i >= 0; i--)
                value[i] = false;
            break;
        }
        
        BigInteger tmp(divisor << digit);
        if (remainder < tmp)
            value[digit] = false;
        else
        {
            value[digit] = true;
            remainder -= tmp;
        }
    }
    
    remainder.sign = this->sign;
    return remainder;
}

BigInteger &BigInteger::operator=(const BigInteger &bigInt)
{
    if (this == &bigInt)
        return *this;
    
    if (this->value)
        delete[] this->value;
    
    this->length = bigInt.length;
    this->sign = bigInt.sign;
    if (this->length)
    {
        this->value = new bool[this->length];
        for (int digit = 0; digit < this->length; digit++)
        {
            *(this->value + digit) = bigInt.value[digit];
        }
    }
    else
        this->value = NULL;
    
    return *this;
}

BigInteger &BigInteger::operator+=(const BigInteger &bigInt)
{
    *this = *this + bigInt;
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &bigInt)
{
    *this = *this - bigInt;
    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &bigInt)
{
    *this = *this * bigInt;
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &bigInt)
{
    *this = *this / bigInt;
    return *this;
}

BigInteger BigInteger::getRand()
{
    int length = 10000;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        value[digit] = rand() % 2;
    bool sign = true;
    
    return BigInteger(length, value, sign);
}

BigInteger BigInteger::mulmod(const BigInteger mul1, const BigInteger mul2, const BigInteger mod)
{
    if (mul1 == 0 || mul2 == 0)
        return 0;
    
    BigInteger product = 0;
    BigInteger modMul1 = mul1;
    for (int digit = 0; digit < mul2.length; digit++)
    {
        modMul1 = (modMul1 << 1) % mod;
        if (mul2.value[digit])
            product = (product + modMul1) % mod;
    }
    
    return product;
}

BigInteger BigInteger::fastExp(const BigInteger base, const BigInteger exponent, const BigInteger mod)
{
    BigInteger power = 1;
    
    for (int digit = exponent.length - 1; digit >= 0; digit--)
    {
        power = mulmod(power, power, mod);
        if (exponent.value[digit])
            power = mulmod(power, base, mod);
    }
    
    return power;
}

BigInteger BigInteger::gcd(const BigInteger a, const BigInteger b)
{
    if (a % b == 0)
        return b;
    return gcd(b, a % b);
}

BigInteger BigInteger::extendGcd(BigInteger x, BigInteger y, BigInteger &a, BigInteger &b)
{
    if (y == 0)
    {
        a = 1;
        b = 0;
        return x;
    }
    
    BigInteger r = BigInteger::extendGcd(y, x % y, a, b);
    BigInteger tmp = a;
    a = b;
    b = tmp - x / y * b;
    return r;
}

BigInteger BigInteger::inverse(const BigInteger a, const BigInteger mod)
{
    BigInteger inverse = 0;
    BigInteger tmp = 0;
    BigInteger::extendGcd(a, mod, inverse, tmp);
    return inverse;
}

BigInteger BigInteger::getRandBit(int length)
{
    bool *value = new bool[length];
    
    value[length - 1] = true;
    for (int digit = 0; digit < length - 1; digit++)
        value[digit] = rand() % 2;
    return BigInteger(length, value, true);
}
