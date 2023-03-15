#ifndef K_H
#define K_H

#include <cmath>

inline double k(double x)
{
    return -2.43712 * pow(10,-20) * pow(x,7) +
            1.12669 * pow(10,-16) * pow(x,6) -
            2.17325 * pow(10,-13) * pow(x,5) +
            2.27935 * pow(10,-10) * pow(x,4) -
            1.41146 * pow(10,-7)  * pow(x,3) +
            5.19794 * pow(10,-5)  * pow(x,2) -
            0.010683 * x + 2.30172;
}

#endif // K_H
