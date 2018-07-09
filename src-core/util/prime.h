#ifndef UTIL_PRIME_H_
#define UTIL_PRIME_H_

extern const unsigned long prime_list[];

enum { _S_n_primes = sizeof(unsigned long) != 8 ? 256 : 256 + 48 };

#endif //!defined(UTIL_PRIME_H_)
