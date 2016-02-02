/*
 * Some definition about integer AC.
 * CODE_BITS is the number of bits we can use when encoding.
 * TOP_VALUE is the maximun number which can be represented by CODE_BITS bits, which is also the interval length.
 * FIRST_QTR, HALF, THIRD_QTR is three points that divide the interval [0, TOP_VALUE] into 4 equal length part.
 */
#ifndef AC_DEF
#define AC_DEF

#define CODE_BITS b 
#define TOP_VALUE (((long long)1<<CODE_BITS)-1)
#define FIRST_QTR ((TOP_VALUE>>2)+1)
#define HALF (FIRST_QTR<<1)
#define THIRD_QTR (FIRST_QTR*3)

#endif
