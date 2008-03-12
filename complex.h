#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <complex.h>
#include <math.h>

#define _complex_t  double
#define complex_t   complex _complex_t
#define real(x)     creal(x)
#define imag(x)     cimag(x)
#define conj(x)     conj(x)
#define mag(x)      sqrt(pow(real(x), 2) + pow(imag(x), 2))

/**
 * Create a complex number from real and imaginary components
 */
complex_t complex_new(_complex_t r, _complex_t i);

/**
 * Get the midpoint between two complex numbers
 */
complex_t complex_mean(complex_t a, complex_t b);

#endif
