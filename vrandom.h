//{FAST RANDOM
constexpr int RND_SEED=8000;
static unsigned int g_seed=RND_SEED;
inline int fastrand() {	g_seed = (214013 * g_seed + 2531011);return (g_seed >> 16) & 0x7FFF;}
inline int fastRandInt(int maxSize) {return fastrand() % maxSize;}
inline int fastRandInt(int a, int b) {return(a + fastRandInt(b - a));}
inline float fastRandDouble(){return static_cast<float>(fastrand())/0x7FFF;}
inline float fastRandDouble(float a,float b){return a+(static_cast<float>(fastrand())/0x7FFF)*(b-a);}
//}FAST RANDOM
//{FAST RANDOM VECTORIZED
#pragma GCC diagnostic ignored "-Wnarrowing"
__m128i SEED_RANDOM={UINT64_C(2000),UINT64_C(8000)};
static const __m128i v2i_k1 = {UINT64_C(0x9E3779B97F4A7C15),UINT64_C(0x9E3779B97F4A7C15)};
static const __m128i v2i_k2 = {UINT64_C(0xBF58476D1CE4E5B9),UINT64_C(0xBF58476D1CE4E5B9)};
static const __m128i v2i_k3 = {UINT64_C(0x94D049BB133111EB),UINT64_C(0x94D049BB133111EB)};
static const __m128i v2i_k4 = {2685821657736338717LL,2685821657736338717LL};
static const __m128i v2i_9  = {UINT64_C(9),UINT64_C(9)};
static const __m128i v2i_27 = {UINT64_C(27),UINT64_C(27)};
static const __m128i v2i_28 = {UINT64_C(28),UINT64_C(28)};
static const __m128i v2i_30 = {UINT64_C(30),UINT64_C(30)};
static const __m128i v2i_31 = {UINT64_C(31),UINT64_C(31)};
static const __m128i v2i_55 = {UINT64_C(55),UINT64_C(55)};
static const __m128i v2i_36 = {UINT64_C(36),UINT64_C(36)};
static const __m128i v2i_12 = {UINT64_C(12),UINT64_C(12)};
static const __m128i v2i_14 = {UINT64_C(14),UINT64_C(14)};
static const __m128i v2i_25 = {UINT64_C(25),UINT64_C(25)};
static const __m128i v8i_FF = {UINT64_C(0xFFFFFFFFFFFFFFFF),UINT64_C(0xFFFFFFFFFFFFFFFF)};
static const __m128i v8i_UINT={UINT64_C(0x7FFF7FFF7FFF7FFF),UINT64_C(0x7FFF7FFF7FFF7FFF)};
static const __m128i v8i_UB = {UINT64_C(0x7F7F7F7F7F7F7F7F),UINT64_C(0x7F7F7F7F7F7F7F7F)};

inline __m128i mul64(__m128i const & a,__m128i const & b){
    __m128i bswap   = _mm_shuffle_epi32(b,0xB1);           // b0H,b0L,b1H,b1L (swap H<->L)
    __m128i prodlh  = _mm_mullo_epi32(a,bswap);            // a0Lb0H,a0Hb0L,a1Lb1H,a1Hb1L, 32 bit L*H products
    __m128i zero    = _mm_setzero_si128();                 // 0
    __m128i prodlh2 = _mm_hadd_epi32(prodlh,zero);         // a0Lb0H+a0Hb0L,a1Lb1H+a1Hb1L,0,0
    __m128i prodlh3 = _mm_shuffle_epi32(prodlh2,0x73);     // 0, a0Lb0H+a0Hb0L, 0, a1Lb1H+a1Hb1L
    __m128i prodll  = _mm_mul_epu32(a,b);                  // a0Lb0L,a1Lb1L, 64 bit unsigned products
    __m128i prod    = _mm_add_epi64(prodll,prodlh3);       // a0Lb0L+(a0Lb0H+a0Hb0L)<<32, a1Lb1L+(a1Lb1H+a1Hb1L)<<32
    return  prod;    
}

__m128i xnext() {
    SEED_RANDOM = _mm_add_epi64(SEED_RANDOM,v2i_k1);
    __m128i ALIGN16 z = SEED_RANDOM;
	z=mul64(_mm_xor_si128(z,_mm_srl_epi64(z,v2i_30)),v2i_k2);
	z=mul64(_mm_xor_si128(z,_mm_srl_epi64(z,v2i_27)),v2i_k3);
	return _mm_xor_si128(z,_mm_srl_epi64(z,v2i_31));
}
__m128i ALIGN16 seed[2]={ xnext(),xnext() };

inline __m128i Xrandom() {
	__m128i ALIGN16 s0=seed[0];
	__m128i ALIGN16 s1=seed[1];
	__m128i ALIGN16 result=_mm_add_epi64(s0,s1);
	s1= _mm_xor_si128(s1,s0);
	seed[0]=_mm_xor_si128(_mm_or_si128(_mm_sll_epi64(s0,v2i_55),_mm_srl_epi64(s1,v2i_9)),_mm_xor_si128(s1,_mm_sll_epi64(s1,v2i_14)));
	seed[1]=_mm_or_si128(_mm_sll_epi64(s1,v2i_36),_mm_srl_epi64(s1,v2i_28));
	return result;
}

 inline __m128i Irandom(){return _mm_and_si128(Xrandom(),v8i_UINT);}
 template <int i0>
 inline __m128i Irandom(){
switch(i0) {   
 case 2:case 4:case 8:case 16:case 32:case 64:case 128:case 256:case 512:case 1024:

  static const union {int16_t f[8];__m128i v;} u = {{(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1)
                                                    ,(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1)}};
    return _mm_and_si128(Xrandom(),u.v);
 default: break;
}  
     
//Slower calculation     
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)i0);
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)i0));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return convert(_mm256_sub_ps(a, base));
 }
 template <int init,int end>
 inline __m128i Irandom(){
switch(end-init) {   
 case 2:case 4:case 8:case 16:case 32:case 64:case 128:case 256:case 512:case 1024:

  static const union {int16_t f[8];__m128i v;} u = {{(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1)
                                                    ,(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1)}};
    return _mm_add_epi16(_mm_and_si128(Xrandom(),u.v),_mm_set1_epi16(init));
 default: break;
}          
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)(end-init));
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)(end-init)));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return convert(_mm256_add_ps(_mm256_sub_ps(a, base),_mm256_set1_ps((float)init)));
 }
 inline __m128i Irandom(const int& i0){
switch(i0) {   
 case 2:case 4:case 8:case 16:case 32:case 64:case 128:case 256:case 512:case 1024:

  static const union {int16_t f[8];__m128i v;} u = {{(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1)
                                                    ,(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1)}};
    return _mm_and_si128(Xrandom(),u.v);
 default: break;
}  
     
//Slower calculation     
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)i0);
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)i0));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return convert(_mm256_sub_ps(a, base));
 }
 inline __m128i Irandom(const int& init,const int& end){
switch(end-init) {   
 case 2:case 4:case 8:case 16:case 32:case 64:case 128:case 256:case 512:case 1024:

    static const union {int16_t f[8];__m128i v;} u = {{(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1)
                                                    ,(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1)}};
    return _mm_add_epi16(_mm_and_si128(Xrandom(),u.v),_mm_set1_epi16(init));
 default: break;
}          
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)(end-init));
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)(end-init)));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return convert(_mm256_add_ps(_mm256_sub_ps(a, base),_mm256_set1_ps((float)init)));
 }
 inline __m128i IrandomBool(){return  _mm_cmpeq_epi16(_mm_and_si128(Xrandom(),C_I<1>().v), _mm_setzero_si128());}


 inline __m256 Frandom(){
	 return convert(_mm_and_si128(Xrandom(),v8i_UINT));
 }

 template <int i0>
 inline __m256 Frandom(){
switch(i0) {   
 case 2:case 4:case 8:case 16:case 32:case 64:case 128:case 256:case 512:case 1024:

  static const union {int16_t f[8];__m128i v;} u = {{(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1)
                                                    ,(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1),(int16_t)(i0-1)}};
    return convert(_mm_and_si128(Xrandom(),u.v));
 default: break;
}       
//Slower calculation     
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)i0);
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)i0));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return _mm256_sub_ps(a, base);
 }
 
 template <int init,int end>
 inline __m256 Frandom(){
switch(end-init) {   
 case 2:case 4:case 8:case 16:case 32:case 64:case 128:case 256:case 512:case 1024:

  static const union {int16_t f[8];__m128i v;} u = {{(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1)
                                                    ,(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1),(int16_t)(end-init-1)}};
    return _mm256_add_ps(convert(_mm_and_si128(Xrandom(),u.v)),_mm256_set1_ps((float)init));
 default: break;
}          
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)(end-init));
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)(end-init)));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return _mm256_add_ps(_mm256_sub_ps(a, base),_mm256_set1_ps((float)init));
 }
 
 inline __m256 Frandom(const int& i0){
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)i0);
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)i0));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return _mm256_sub_ps(a, base);
 }

 inline __m256 Frandom(const int& init,const int& end){
    __m256 ALIGN a = convert(_mm_and_si128(Xrandom(),v8i_UINT));
    __m256 ALIGN den = _mm256_set1_ps((float)(end-init));
    __m256 ALIGN c = _mm256_mul_ps(a,_mm256_set1_ps(1.0f/(float)(end-init)));
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return _mm256_add_ps(_mm256_sub_ps(a, base),_mm256_set1_ps((float)init));
 }
 
 inline __m256 FrandomBool(){
     static const union {int16_t f[8];__m128i v;} V_1 = {{1,1,1,1,1,1,1,1}};
     return _mm256_cmp_ps(convert(_mm_and_si128(Xrandom(),V_1.v)),_mm256_setzero_ps(),0);
     }
//}FAST RANDOM VECTORIZED