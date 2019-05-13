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
    this->arithmeticLength = 0;
    this->value = NULL;
    this->sign = true;
}

BigInteger::BigInteger(int length, bool *value, bool sign)
{
    this->length = length;
    this->value = value;
    this->sign = sign;
    this->arithmeticLength = this->length;
    while (this->arithmeticLength > 0 && this->value[this->arithmeticLength - 1] == false)
        this->arithmeticLength--;
}

BigInteger::BigInteger(const int number)
{
    if (!number)
    {
        this->length = 0;
        this->arithmeticLength = 0;
        this->value = NULL;
        this->sign = true;
    }
    else
    {
        int tmp1 = number;
        int tmp2 = number;
        
        //set sign
        if (number > 0)
            this->sign = true;
        else
        {
            this->sign = false;
            tmp1 = -number;
            tmp2 = tmp1;
        }
        
        //set length
        this->length = 1;
        while (tmp1 >= 2)
        {
            this->length++;
            tmp1 /= 2;
        }
        
        //set arithmeticLength
        this->arithmeticLength = this->length;
        
        //set value
        this->value = new bool[this->length];
        for (int digit = 0; digit < this->length; digit++)
        {
            *(this->value + digit) = tmp2 % 2;
            tmp2 /= 2;
        }
    }
}

BigInteger::BigInteger(const unsigned int number)
{
    if (!number)
    {
        this->length = 0;
        this->arithmeticLength = 0;
        this->value = NULL;
        this->sign = true;
    }
    else
    {
        unsigned int tmp1 = number;
        unsigned int tmp2 = number;
        
        //set sign
        this->sign = true;
        
        //set length
        this->length = 1;
        while (tmp1 >= 2)
        {
            this->length++;
            tmp1 /= 2;
        }
        
        //set arithmeticLength
        this->arithmeticLength = this->length;
        
        //set value
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
        
        //set length
        this->length = 4 * (length - 2);
        
        //set value
        this->value = value;
        
        //set sign
        this->sign = true;
        
        //set arithmeticLength
        this->arithmeticLength = this->length;
        while (this->arithmeticLength > 0 && this->value[this->arithmeticLength - 1] == false)
            this->arithmeticLength--;
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
        
        //set length
        this->length = length;
        
        //set value
        this->value = value;
        
        //set sign
        this->sign = true;
        
        //set arithmeticLength
        this->arithmeticLength = this->length;
        while (this->arithmeticLength > 0 && this->value[this->arithmeticLength - 1] == false)
            this->arithmeticLength--;
    }
}

BigInteger::BigInteger(const BigInteger &bigInt)
{
    //set sign
    this->sign = bigInt.sign;
    
    //set length
    this->length = bigInt.length;
    
    //set arithmeticLength
    this->arithmeticLength = bigInt.arithmeticLength;
    
    //set value
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

int BigInteger::getLength() const
{
    return this->length;
}

bool BigInteger::operator<(const BigInteger &bigInt) const
{
    if (!this->sign && bigInt.sign)
        return true;
    if (this->sign && !bigInt.sign)
        return false;
    
    if (this->arithmeticLength < bigInt.arithmeticLength)
        return this->sign;
    if (this->arithmeticLength > bigInt.arithmeticLength)
        return !this->sign;
    
    if (!this->arithmeticLength)
        return false;
    
    for (int digit = this->arithmeticLength - 1; digit >= 0; digit--)
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
    if (this->sign == bigInt.sign && this->arithmeticLength == bigInt.arithmeticLength)
    {
        if (!this->arithmeticLength)
            return true;
        for (int digit = this->arithmeticLength - 1; digit >= 0; digit--)
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
    
    if (this->arithmeticLength < bigInt.arithmeticLength)
        return this->sign;
    if (this->arithmeticLength > bigInt.arithmeticLength)
        return !this->sign;
    
    if (!this->arithmeticLength)
        return true;
    for (int digit = this->arithmeticLength - 1; digit >= 0; digit--)
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
    
    int length = this->arithmeticLength > bigInt.arithmeticLength ? this->arithmeticLength : bigInt.arithmeticLength;
    if (!length)
        return BigInteger(0, NULL, true);
    length++;
    
    bool *value = new bool[length];
    bool overflow = false;
    for (int digit = 0; digit < length - 1; digit++)
    {
        bool adder1 = false;
        bool adder2 = false;
        if (digit < this->arithmeticLength)
            adder1 = this->value[digit];
        if (digit < bigInt.arithmeticLength)
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
    
    if (!this->arithmeticLength)
        return BigInteger(0, NULL, true);
    
    bool *value = new bool[this->arithmeticLength];
    bool carry = false;
    for (int digit = 0; digit < this->arithmeticLength; digit++)
    {
        bool minuend = false;
        bool subtrahend = false;
        if (digit < this->arithmeticLength)
            minuend = *(this->value + digit);
        if (digit < bigInt.arithmeticLength)
            subtrahend = bigInt.value[digit];
        
        *(value + digit) = (!minuend && !subtrahend && carry) || (!minuend && subtrahend && !carry) || (minuend && !subtrahend && !carry) || (minuend && subtrahend && carry);
        carry = (!minuend && !(!subtrahend && !carry)) || (minuend && subtrahend && carry);
    }
    
    int length = this->arithmeticLength;
    for (int digit = this->arithmeticLength - 1; digit >= 0; digit--)
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
    if (this->arithmeticLength == 0 || bigInt.arithmeticLength == 0)
        return BigInteger(0, NULL, true);
    
    BigInteger result(0, NULL, true);
    for (int digit = 0; digit < bigInt.arithmeticLength; digit++)
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
    if (!this->arithmeticLength || this->arithmeticLength < bigInt.arithmeticLength)
        return BigInteger(0, NULL, true);
    
    int length = this->arithmeticLength - bigInt.arithmeticLength + 1;
    
    bool *value = new bool[length];
    BigInteger remainder(*this);
    BigInteger divisor(bigInt);
    remainder.sign = true;
    divisor.sign = true;
    for (int digit = length - 1; digit >= 0; digit--)
    {
        if (!remainder.arithmeticLength)
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
    if (!this->arithmeticLength || this->arithmeticLength < bigInt.arithmeticLength)
        return *this;
    int length = this->arithmeticLength - bigInt.arithmeticLength + 1;
    
    bool *value = new bool[length];
    BigInteger remainder(*this);
    BigInteger divisor(bigInt);
    remainder.sign = true;
    divisor.sign = true;
    for (int digit = length - 1; digit >= 0; digit--)
    {
        if (!remainder.arithmeticLength)
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
    delete[] value;
    return remainder;
}

BigInteger BigInteger::operator&(const BigInteger &bigInt) const
{
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    int length = this->length;
    bool sign = true;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        value[digit] = this->value[digit] & bigInt.value[digit];
    
    return BigInteger(length, value, sign);
}

BigInteger BigInteger::operator|(const BigInteger &bigInt) const
{
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    int length = this->length;
    bool sign = true;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        value[digit] = this->value[digit] | bigInt.value[digit];
    
    return BigInteger(length, value, sign);
}

BigInteger BigInteger::operator^(const BigInteger &bigInt) const
{
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    int length = this->length;
    bool sign = true;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        value[digit] = (this->value[digit] & !bigInt.value[digit]) | (!this->value[digit] & bigInt.value[digit]);
    
    return BigInteger(length, value, sign);
}

BigInteger BigInteger::operator~() const
{
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    int length = this->length;
    bool sign = true;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        value[digit] = !this->value[digit];
    
    return BigInteger(length, value, sign);
}

BigInteger BigInteger::rotLeft(const int n) const
{
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    int length = this->length;
    bool sign = this->sign;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        *(value + digit) = this->value[(digit + length - n) % length];
    
    return BigInteger(length, value, sign);
}

BigInteger BigInteger::rotRight(const int n) const
{
    if (!this->length)
        return BigInteger(0, NULL, true);
    
    int length = this->length;
    bool sign = this->sign;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        *(value + digit) = this->value[(digit + n) % length];
    return BigInteger(length, value, sign);
}

void BigInteger::append(bool *value, int length)
{
    int newLength = this->length + length;
    bool *newValue = new bool[newLength];
    for (int i = 0; i < length; i++)
        newValue[i] = value[i];
    for (int i = 0; i < this->length; i++)
        newValue[i + length] = this->value[i];
    
    delete[] this->value;
    this->value = newValue;
    this->length = newLength;
    this->arithmeticLength = this->length;
    while (this->arithmeticLength > 0 && this->value[this->arithmeticLength - 1] == false)
        this->arithmeticLength--;
}

void BigInteger::append(const BigInteger &bigInt)
{
    int newLength = this->length + bigInt.length;
    bool *newValue = new bool[newLength];
    for (int i = 0; i < bigInt.length; i++)
        newValue[i] = bigInt.value[i];
    for (int i = 0; i < this->length; i++)
        newValue[i + bigInt.length] = this->value[i];
    
    delete[] this->value;
    this->value = newValue;
    this->length = newLength;
    this->arithmeticLength = this->length;
    while (this->arithmeticLength > 0 && this->value[this->arithmeticLength - 1] == false)
        this->arithmeticLength--;
}

BigInteger BigInteger::slice(int start, int end) const
{
    int length = end - start;
    bool sign = this->sign;
    bool *value = new bool[length];
    for (int digit = 0; digit < length; digit++)
        value[digit] = this->value[start + digit];
    
    return BigInteger(length, value, sign);
}

void BigInteger::limitTo(int length)
{
    bool *value = new bool[length];
    if (this->length <= length)
    {
        for (int digit = 0; digit < this->length; digit++)
            value[digit] = this->value[digit];
        int digit = this->length;
        while (digit < length)
        {
            value[digit] = false;
            digit++;
        }
    }
    else
    {
        for (int digit = 0; digit < length; digit++)
            value[digit] = this->value[digit];
    }
    delete[] this->value;
    this->value = value;
    this->length = length;
    this->arithmeticLength = this->length;
    while (this->arithmeticLength > 0 && this->value[this->arithmeticLength - 1] == false)
        this->arithmeticLength--;
}

BigInteger &BigInteger::operator=(const BigInteger &bigInt)
{
    if (this == &bigInt)
        return *this;
    
    if (this->value)
        delete[] this->value;
    
    this->length = bigInt.length;
    this->arithmeticLength = bigInt.arithmeticLength;
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

BigInteger BigInteger::bigIntegerFromASCIIString(char *string)
{
    int strLen = 0;
    while (*(string + strLen) != '\0')
        strLen++;
    int length = strLen * 8;
    if (!length)
        return BigInteger(0, NULL, true);
    
    bool *value = new bool[length];
    for (int word = strLen - 1; word >= 0; word--)
    {
        int ch = *(string + word);
        for (int digit = 0; digit < 8; digit++)
        {
            *(value + (strLen - word - 1) * 8 + digit) = ch % 2;
            ch /= 2;
        }
    }
    return BigInteger(length, value, true);
}

int BigInteger::to_int() const
{
    if (!this->length)
        return 0;
    
    int value = 0;
    for (int digit = 0; digit < this->arithmeticLength; digit++)
        if (this->value[digit])
        {
            int power = 1;
            int i = 0;
            while (i < digit)
            {
                power *= 2;
                i++;
            }
            value += power;
        }
    return value;
}

char *BigInteger::ASCIIString() const
{
    if (!this->length)
        return NULL;
    char *str = new char[this->length / 8 + 1];
    for (int index = 0; index < this->length / 8; index++)
    {
        int ch = 0;
        for (int digit = 0; digit < 8; digit++)
        {
            int a = this->value[(this->length / 8 - index - 1) * 8 + digit];
            int k = 0;
            while (k < digit)
            {
                a *= 2;
                k++;
            }
            ch += a;
        }
        *(str + index) = ch;
    }
    *(str + this->length / 8) = '\0';
    return str;
}

char *BigInteger::hexString() const
{
    if (!this->length)
        return NULL;
    char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char *str = new char[this->length / 4 + 1];
    for (int index = 0; index < this->length / 4; index++)
    {
        int ch = 0;
        for (int digit = 0; digit < 4; digit++)
        {
            int a = this->value[(this->length / 4 - index - 1) * 4 + digit];
            int k = 0;
            while (k < digit)
            {
                a *= 2;
                k++;
            }
            ch += a;
        }
        *(str + index) = hex[ch];
    }
    *(str + this->length / 4) = '\0';
    return str;
}

char *BigInteger::decString() const
{
    if (!this->arithmeticLength)
        return NULL;
    char dec[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char *str = new char[(int)(this->arithmeticLength * log(2) / log(10))];
    int length = 0;
    BigInteger tmp = *this;
    while (!(tmp == 0))
    {
        BigInteger a = tmp % 10;
        str[length] = dec[(tmp % 10).to_int()];
        tmp /= 10;
        length++;
    }
    char *result;
    if (this->sign)
    {
        result = new char[length + 1];
        
        for (int ch = length - 1; ch >= 0; ch--)
            result[ch] = str[length - ch - 1];
        result[length] = '\0';
    }
    else
    {
        result = new char[length + 2];
        result[0] = '-';
        for (int ch = length; ch >= 1; ch--)
            result[ch] = str[length - ch];
        result[length + 1] = '\0';
    }
    delete[] str;
    return result;
}

BigInteger BigInteger::mulmod(const BigInteger mul1, const BigInteger mul2, const BigInteger mod)
{
    if (mul1 == 0 || mul2 == 0)
        return 0;
    
    BigInteger product = 0;
    BigInteger modMul1 = mul1;
    for (int digit = 0; digit < mul2.arithmeticLength; digit++)
    {
        if (mul2.value[digit])
            product = (product + modMul1) % mod;
        modMul1 = (modMul1 << 1) % mod;
    }
    
    return product;
}

BigInteger BigInteger::fastExp(const BigInteger base, const BigInteger exponent, const BigInteger mod)
{
    BigInteger power = 1;
    
    for (int digit = exponent.arithmeticLength - 1; digit >= 0; digit--)
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
    return (inverse % mod + mod) % mod;
}

BigInteger BigInteger::getRandBit(int length)
{
    bool *value = new bool[length];
    
    value[length - 1] = true;
    for (int digit = 0; digit < length - 1; digit++)
        value[digit] = rand() % 2;
    return BigInteger(length, value, true);
}
