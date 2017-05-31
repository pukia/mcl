#pragma once
/**
	@file
	@brief C interface of 256/384-bit optimal ate pairing over BN curves
	@author MITSUNARI Shigeo(@herumi)
	@license modified new BSD license
	http://opensource.org/licenses/BSD-3-Clause
*/
#ifndef BN_MAX_FP_UNIT_SIZE
	#error "define BN_MAX_FP_UNIT_SIZE 4(or 6)"
#endif

#include <stdint.h> // for uint64_t, uint8_t
#include <stdlib.h> // for size_t

#ifdef _MSC_VER
#ifdef BN_DLL_EXPORT
#define BN_DLL_API __declspec(dllexport)
#else
#define BN_DLL_API __declspec(dllimport)
#ifndef MCL_NO_AUTOLINK
	#if BN_MAX_FP_UNIT_SIZE == 4
		#pragma comment(lib, "bn_if256.lib")
	#else
		#pragma comment(lib, "bn_if384.lib")
	#endif
#endif
#endif
#else
#define BN_DLL_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BN_DEFINE_STRUCT

typedef struct {
	uint64_t d[BN_MAX_FP_UNIT_SIZE];
} BN_Fr;

typedef struct {
	uint64_t d[BN_MAX_FP_UNIT_SIZE * 3];
} BN_G1;

typedef struct {
	uint64_t d[BN_MAX_FP_UNIT_SIZE * 2 * 3];
} BN_G2;

typedef struct {
	uint64_t d[BN_MAX_FP_UNIT_SIZE * 12];
} BN_GT;

#else

typedef struct BN_Fr BN_Fr;
typedef struct BN_G1 BN_G1;
typedef struct BN_G2 BN_G2;
typedef struct BN_GT BN_GT;

#endif

/*
	set errlog file name
	use stderr if name == "stderr"
	close if name == ""
	return 0 if success
	@note not threadsafe
*/
BN_DLL_API int BN_setErrFile(const char *name);

enum {
	BN_curveFp254BNb = 0,
	BN_curveFp382_1 = 1,
	BN_curveFp382_2 = 2
};

/*
	init library
	@param curve [in] type of bn curve
	@param maxUnitSize [in] 4 or 6
	curve = BN_CurveFp254BNb is allowed if maxUnitSize = 4
	curve = BN_CurveFp254BNb/BN_CurveFp382_1/BN_CurveFp382_2 are allowed if maxUnitSize = 6
	@note not threadsafe
	@note BN_init is used in libeay32
*/
BN_DLL_API int BN_initLib(int curve, int maxUnitSize);

////////////////////////////////////////////////
// set zero
BN_DLL_API void BN_Fr_clear(BN_Fr *x);

// set x to y
BN_DLL_API void BN_Fr_setInt(BN_Fr *y, int x);

// return 0 if success
BN_DLL_API int BN_Fr_setDecStr(BN_Fr *x, const char *s);
BN_DLL_API int BN_Fr_setHexStr(BN_Fr *x, const char *s);
// mask buf with (1 << (bitLen(r) - 1)) - 1 if buf >= r
BN_DLL_API int BN_Fr_setLittleEndian(BN_Fr *x, const void *buf, size_t bufSize);

// return 1 if true and 0 otherwise
BN_DLL_API int BN_Fr_isValid(const BN_Fr *x);
BN_DLL_API int BN_Fr_isEqual(const BN_Fr *x, const BN_Fr *y);
BN_DLL_API int BN_Fr_isZero(const BN_Fr *x);
BN_DLL_API int BN_Fr_isOne(const BN_Fr *x);

BN_DLL_API void BN_Fr_setByCSPRNG(BN_Fr *x);

// hash(s) and set x
BN_DLL_API void BN_hashToFr(BN_Fr *x, const void *buf, size_t bufSize);

// return strlen(buf) if sucess else 0
BN_DLL_API size_t BN_Fr_getDecStr(char *buf, size_t maxBufSize, const BN_Fr *x);
BN_DLL_API size_t BN_Fr_getHexStr(char *buf, size_t maxBufSize, const BN_Fr *x);
// return written byte if sucess else 0
BN_DLL_API size_t BN_Fr_getLittleEndian(void *buf, size_t bufSize, const BN_Fr *x);

BN_DLL_API void BN_Fr_neg(BN_Fr *y, const BN_Fr *x);
BN_DLL_API void BN_Fr_inv(BN_Fr *y, const BN_Fr *x);
BN_DLL_API void BN_Fr_add(BN_Fr *z, const BN_Fr *x, const BN_Fr *y);
BN_DLL_API void BN_Fr_sub(BN_Fr *z, const BN_Fr *x, const BN_Fr *y);
BN_DLL_API void BN_Fr_mul(BN_Fr *z, const BN_Fr *x, const BN_Fr *y);
BN_DLL_API void BN_Fr_div(BN_Fr *z, const BN_Fr *x, const BN_Fr *y);

////////////////////////////////////////////////
// set zero
BN_DLL_API void BN_G1_clear(BN_G1 *x);

// return 0 if success
BN_DLL_API int BN_G1_setHexStr(BN_G1 *x, const char *s);
BN_DLL_API int BN_G1_deserialize(BN_G1 *x, const char *buf, size_t bufSize);

// return 1 if true and 0 otherwise
BN_DLL_API int BN_G1_isValid(const BN_G1 *x);
BN_DLL_API int BN_G1_isEqual(const BN_G1 *x, const BN_G1 *y);
BN_DLL_API int BN_G1_isZero(const BN_G1 *x);

BN_DLL_API int BN_hashAndMapToG1(BN_G1 *x, const void *buf, size_t bufSize);

// return 0 if success
BN_DLL_API size_t BN_G1_getHexStr(char *buf, size_t maxBufSize, const BN_G1 *x);
// return written size if sucess else 0
BN_DLL_API size_t BN_G1_serialize(void *buf, size_t maxBufSize, const BN_G1 *x);

BN_DLL_API void BN_G1_neg(BN_G1 *y, const BN_G1 *x);
BN_DLL_API void BN_G1_dbl(BN_G1 *y, const BN_G1 *x);
BN_DLL_API void BN_G1_add(BN_G1 *z, const BN_G1 *x, const BN_G1 *y);
BN_DLL_API void BN_G1_sub(BN_G1 *z, const BN_G1 *x, const BN_G1 *y);
BN_DLL_API void BN_G1_mul(BN_G1 *z, const BN_G1 *x, const BN_Fr *y);

////////////////////////////////////////////////
// set zero
BN_DLL_API void BN_G2_clear(BN_G2 *x);

// return 0 if success
BN_DLL_API int BN_G2_setHexStr(BN_G2 *x, const char *s);
BN_DLL_API int BN_G2_deserialize(BN_G2 *x, const char *buf, size_t bufSize);

// return 1 if true and 0 otherwise
BN_DLL_API int BN_G2_isValid(const BN_G2 *x);
BN_DLL_API int BN_G2_isEqual(const BN_G2 *x, const BN_G2 *y);
BN_DLL_API int BN_G2_isZero(const BN_G2 *x);

BN_DLL_API int BN_hashAndMapToG2(BN_G2 *x, const void *buf, size_t bufSize);

// return 0 if success
BN_DLL_API size_t BN_G2_getHexStr(char *buf, size_t maxBufSize, const BN_G2 *x);
// return written size if sucess else 0
BN_DLL_API size_t BN_G2_serialize(void *buf, size_t maxBufSize, const BN_G2 *x);

BN_DLL_API void BN_G2_neg(BN_G2 *y, const BN_G2 *x);
BN_DLL_API void BN_G2_dbl(BN_G2 *y, const BN_G2 *x);
BN_DLL_API void BN_G2_add(BN_G2 *z, const BN_G2 *x, const BN_G2 *y);
BN_DLL_API void BN_G2_sub(BN_G2 *z, const BN_G2 *x, const BN_G2 *y);
BN_DLL_API void BN_G2_mul(BN_G2 *z, const BN_G2 *x, const BN_Fr *y);

////////////////////////////////////////////////
// set zero
BN_DLL_API void BN_GT_clear(BN_GT *x);

// return 0 if success
BN_DLL_API int BN_GT_setDecStr(BN_GT *x, const char *s);
BN_DLL_API int BN_GT_setHexStr(BN_GT *x, const char *s);
BN_DLL_API int BN_GT_deserialize(BN_GT *x, const char *buf, size_t bufSize);

// return 1 if true and 0 otherwise
BN_DLL_API int BN_GT_isEqual(const BN_GT *x, const BN_GT *y);
BN_DLL_API int BN_GT_isZero(const BN_GT *x);
BN_DLL_API int BN_GT_isOne(const BN_GT *x);

// return 0 if success
BN_DLL_API size_t BN_GT_getDecStr(char *buf, size_t maxBufSize, const BN_GT *x);
BN_DLL_API size_t BN_GT_getHexStr(char *buf, size_t maxBufSize, const BN_GT *x);
// return written size if sucess else 0
BN_DLL_API size_t BN_GT_serialize(void *buf, size_t maxBufSize, const BN_GT *x);

BN_DLL_API void BN_GT_neg(BN_GT *y, const BN_GT *x);
BN_DLL_API void BN_GT_inv(BN_GT *y, const BN_GT *x);
BN_DLL_API void BN_GT_add(BN_GT *z, const BN_GT *x, const BN_GT *y);
BN_DLL_API void BN_GT_sub(BN_GT *z, const BN_GT *x, const BN_GT *y);
BN_DLL_API void BN_GT_mul(BN_GT *z, const BN_GT *x, const BN_GT *y);
BN_DLL_API void BN_GT_div(BN_GT *z, const BN_GT *x, const BN_GT *y);

BN_DLL_API void BN_GT_pow(BN_GT *z, const BN_GT *x, const BN_Fr *y);

BN_DLL_API void BN_pairing(BN_GT *z, const BN_G1 *x, const BN_G2 *y);
BN_DLL_API void BN_finalExp(BN_GT *y, const BN_GT *x);
BN_DLL_API void BN_millerLoop(BN_GT *z, const BN_G1 *x, const BN_G2 *y);

// return precomputedQcoeffSize * sizeof(Fp6) / sizeof(uint64_t)
BN_DLL_API int BN_getUint64NumToPrecompute(void);

// allocate Qbuf[BN_getUint64NumToPrecompute()] before calling this
BN_DLL_API void BN_precomputeG2(uint64_t *Qbuf, const BN_G2 *Q);

BN_DLL_API void BN_precomputedMillerLoop(BN_GT *f, const BN_G1 *P, const uint64_t *Qbuf);
BN_DLL_API void BN_precomputedMillerLoop2(BN_GT *f, const BN_G1 *P1, const uint64_t *Q1buf, const BN_G1 *P2, const uint64_t *Q2buf);

#ifdef __cplusplus
}
#endif