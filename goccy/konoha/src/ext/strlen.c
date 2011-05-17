///*
//    fast strlen using SSE2 for gcc 4.x / Visual Studio 2008(32bit/64bit)
//
//    Copyright (C) 2008 MITSUNARI Shigeo at Cybozu Labs, Inc.
//    license:new BSD license
//    2008/7/16 fix overrun in strlenSSE2
//
//*/
//
//#include"../../include/konoha1.h"
//
//#ifdef K_USING_SSE2
//#ifdef _WIN32
//    #include <intrin.h>
//    #define bsf(x) (_BitScanForward(&x, x), x)
//    #define bsr(x) (_BitScanReverse(&x, x), x)
//#else
//    #include <xmmintrin.h>
//    #define bsf(x) __builtin_ctz(x)
//#endif
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//size_t knh_strlen(const char *p)
//{
//	const char *const top = p;
//	__m128i c16 = _mm_set1_epi8(0);
//	/* 16 byte alignment */
//	size_t ip = (size_t)p;
//	size_t n = ip & 15;
//	if (n > 0) {
//		ip &= ~15;
//		__m128i x = *(const __m128i*)ip;
//		__m128i a = _mm_cmpeq_epi8(x, c16);
//		unsigned long mask = _mm_movemask_epi8(a);
//		mask &= 0xffffffffUL << n;
//		if (mask) {
//			return bsf(mask) - n;
//		}
//		p += 16 - n;
//	}
//	DBG_ASSERT(((size_t)(p) & 15) == 0);
//	if ((size_t)(p) & 31) {
//		__m128i x = *(const __m128i*)&p[0];
//		__m128i a = _mm_cmpeq_epi8(x, c16);
//		unsigned long mask = _mm_movemask_epi8(a);
//		if (mask) {
//			return p + bsf(mask) - top;
//		}
//		p += 16;
//	}
//	DBG_ASSERT(((size_t)(p) & 31) == 0);
//	for (;;) {
//		__m128i x = *(const __m128i*)&p[0];
//		__m128i y = *(const __m128i*)&p[16];
//		__m128i a = _mm_cmpeq_epi8(x, c16);
//		__m128i b = _mm_cmpeq_epi8(y, c16);
//		unsigned long mask = (_mm_movemask_epi8(b) << 16) | _mm_movemask_epi8(a);
//		if (mask) {
//			return p + bsf(mask) - top;
//		}
//		p += 32;
//	}
//}
//
//#ifdef __cplusplus
//}
//#endif
//
//#endif
