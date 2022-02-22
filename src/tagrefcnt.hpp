#include <iostream>
using u8 = unsigned char;

// This would be controlled by some compile time switch
#define TAG_REFCNT_OPTIMIZATION

// This is what would get swapped in with the compile flag
// to be able to tag exit states not caught by mRefCnt write-watching
// this would be placed in all forgets, assumes, much like how the logging facilities work
#ifdef TAG_REFCNT_OPTIMIZATION
    #define TAG_ACCOUNTED_FOR(ptr) \
    auto tmp = ptr->mRefCnt;
#else
    #define TAG_ACCOUNTED_FOR(ptr)
#endif
