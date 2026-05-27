#ifndef BITS_H
#define BITS_H 1

#define BITS_CLEAR(DST, POS, MASK) ((DST) &= ~((MASK) << (POS)))
#define BITS_SET(DST, POS, MASK) ((DST) |= ((MASK) << (POS)))
#define BITS_WRITE(DST, POS, MASK, V)                                          \
  ((DST) = ((DST) & ~((MASK) << (POS))) | (((MASK) & (V)) << (POS)))
#define BITS_READ(DST, POS, MASK) (((DST) >> (POS)) & (MASK))

#define BIT_CLEAR(DST, POS) BITS_CLEAR((DST), (POS), 1u)
#define BIT_SET(DST, POS) BITS_SET((DST), (POS), 1u)
#define BIT_WRITE(DST, POS, V) BITS_WRITE((DST), (POS), 1u, (V))
#define BIT_READ(DST, POS) BITS_READ((DST), (POS), 1u)

#endif