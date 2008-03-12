#include "complex.h"

complex_t complex_new(_complex_t r, _complex_t i)
{
    return r + (i * _Complex_I);
}

complex_t complex_mean(complex_t a, complex_t b)
{
    return complex_new(
            (real(a) + real(b)) / 2,
            (imag(a) + imag(b)) / 2
    );
}
