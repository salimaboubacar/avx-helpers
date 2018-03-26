//{AVX x8 float. 32-bit x 8=256bits
#define Pv8fab v8f const &a,v8f const &b
#define Pv8fas v8f const &a,float b
#define ALIGN __attribute__((aligned(32)))
#define OP operator
#define STI static inline

template <int i0>
STI __m256 constant8f(){
    static const union {int i[8];__m256 ymm;} u = {{i0,i0,i0,i0,i0,i0,i0,i0}};
    return u.ymm;}

class v8f{
public:
    __m256 ALIGN v;
    v8f(){v= _mm256_setzero_ps();}
    v8f(float f){v=_mm256_set1_ps(f);}
    v8f(float f0,float f1,float f2,float f3,float f4,float f5,float f6,float f7){v=_mm256_setr_ps(f0,f1,f2,f3,f4,f5,f6,f7);}
    v8f(__m256 const& x){v=x;}
    // OP __m256() const {return v;}
    inline void load(float const * p){v=_mm256_loadu_ps(p);}
    inline void loada(float const * p){v=_mm256_load_ps(p); }
    inline void store(float * p){_mm256_storeu_ps(p,v);}
    inline void storea(float * p){_mm256_store_ps(p,v);}
    inline void insert(int index, float value){ v[index]=value; }
    inline float OP [] (int index) const { return v[index];}
};

//Constant creation via templates
template <int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7>
STI v8f C_F(){
    static const union {int f[8];v8f ymm;} u = {{(float)i0,(float)i1,(float)i2,(float)i3,(float)i4,(float)i5,(float)i6,(float)i7}}; return u.ymm;}
template <int i0,int i1>
STI v8f C_F(){
    static const union {float f[8];v8f  ymm;} u = {{(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1}};
    return u.ymm;
}

STI v8f OP  +(Pv8fab){return _mm256_add_ps(a.v,b.v);}
STI v8f OP  +(Pv8fas){return _mm256_add_ps(a.v,_mm256_set1_ps(b));}
STI v8f &OP +=(v8f &a,v8f const &b){a=a+b;return a;}
STI v8f OP  -(Pv8fab){return _mm256_sub_ps(a.v,b.v);}
STI v8f OP  -(Pv8fas){return _mm256_sub_ps(a.v,_mm256_set1_ps(b));}
STI v8f &OP -=(v8f &a,v8f const &b){a=a-b;return a;}
STI v8f OP  *(Pv8fab){return _mm256_mul_ps(a.v,b.v);}
STI v8f OP  *(Pv8fas){return _mm256_mul_ps(a.v,_mm256_set1_ps(b));}
STI v8f &OP *=(v8f &a,v8f const &b){a=a*b;return a;}
STI v8f OP  /(Pv8fab){return _mm256_div_ps(a.v,b.v);}
STI v8f OP  /(Pv8fas){return _mm256_div_ps(a.v,_mm256_set1_ps(b));}
STI v8f &OP /=(v8f &a,v8f const &b){a=a/b;return a;}

STI v8f OP -(v8f const &a){return _mm256_sub_ps(_mm256_setzero_ps(),a.v);}
STI v8f &OP ++(v8f &a){a=a+1;return a;}
STI v8f  OP ++(v8f &a,int){v8f ALIGN a0(a);a=a+1;return a0;}
STI v8f &OP --(v8f &a){a=a-1;return a;}
STI v8f  OP --(v8f &a,int){v8f ALIGN a0=a;a=a-1;return a0;}


STI v8f OP ==(Pv8fab){return _mm256_cmp_ps(a.v,b.v,0);}
STI v8f OP !=(Pv8fab){return _mm256_cmp_ps(a.v,b.v,4);}
STI v8f OP <(Pv8fab) {return _mm256_cmp_ps(a.v,b.v,1);}
STI v8f OP <=(Pv8fab){return _mm256_cmp_ps(a.v,b.v,2);}
STI v8f OP >(Pv8fab) {return _mm256_cmp_ps(b.v,a.v,1);}
STI v8f OP >=(Pv8fab){return _mm256_cmp_ps(b.v,a.v,2);}

STI v8f OP &(Pv8fab){return _mm256_and_ps(a.v,b.v);}
STI v8f OP &&(Pv8fab){return _mm256_and_ps(a.v,b.v);}
//STI v8f &OP &=(v8f &a,v8f const &b){a=a&b;return a;}
STI v8f OP |(Pv8fab){return _mm256_or_ps(a.v,b.v);}
STI v8f OP ||(Pv8fab){return _mm256_or_ps(a.v,b.v);}
//STI v8f &OP |=(v8f &a,v8f const &b){a=a|b;return a;}
STI v8f OP ^(Pv8fab){return _mm256_xor_ps(a.v,b.v);}
//STI v8f &OP ^=(v8f &a,v8f const &b){a=a^b;return a;}

STI v8f OP !(v8f const &a){return _mm256_cmp_ps(a.v,_mm256_setzero_ps(),0);}
//STI v8f OP ~(v8f const &a){return _mm_xor_si128(a.v, _mm_set1_epi32(-1));}


STI v8f andnot(Pv8fab){return _mm256_andnot_ps(b.v,a.v);}
STI std::ostream &OP<<(std::ostream& output, const v8f& p){output<<"v8f: [";for(int i=0;i<(8);++i) output<<p.v[i]<<",";output << "]";return output;}

// Each byte in s must be either 0 (false) or 0xFFFFFFFF (true). No other values are allowed.
STI v8f if_select(v8f const &s,Pv8fab){return _mm256_blendv_ps(b.v,a.v,s.v);}
STI v8f if_add(v8f const &f,Pv8fab){return a + (f&b);}
STI v8f if_sub(v8f const &f,Pv8fab){return a - (f&b);}
STI v8f if_mul(v8f const &f,Pv8fab){return a*if_select(f,b,C_F<1,1>());}
STI v8f if_div(v8f const &f,Pv8fab){return a/if_select(f,b,C_F<1,1>());}
STI void when_select(v8f& res,v8f const &s,const v8f&a){res=_mm256_blendv_ps(res.v,a.v,s.v);}
STI void when_add(v8f& res,const v8f &f,const v8f& b){res+=(f&b);}
STI void when_sub(v8f& res,const v8f &f,const v8f& b){res-=(f&b);}
STI void when_mul(v8f& res,const v8f &f,const v8f& b){res*=if_select(f,b,C_F<1,1>());}
STI void when_div(v8f& res,const v8f &f,const v8f& b){res/=if_select(f,b,C_F<1,1>());}


STI v8f max(Pv8fab){return _mm256_max_ps(a.v,b.v);}
STI v8f min(Pv8fab){return _mm256_min_ps(a.v,b.v);}
STI v8f abs(v8f const &a){
    __m256 ALIGN mask = constant8f<0x7FFFFFFF>();
    return _mm256_and_ps(a.v,mask);}
STI v8f sqrt(v8f const &a){return _mm256_sqrt_ps(a.v);}

// function round: round to nearest integer (even). (result as float vector)
STI v8f round(v8f const &a){return _mm256_round_ps(a.v,0+8);}
// function truncate: round towards zero. (result as float vector)
STI v8f truncate(v8f const &a){return _mm256_round_ps(a.v,3+8);}
// function floor: round towards minus infinity. (result as float vector)
STI v8f floor(v8f const &a){return _mm256_round_ps(a.v,1+8);}
// function ceil: round towards plus infinity. (result as float vector)
STI v8f ceil(v8f const &a){return _mm256_round_ps(a.v,2+8);}


STI v8f OP %(Pv8fab){  //Slow due to division
    __m256 ALIGN c = _mm256_div_ps(a.v,b.v);
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),b.v);
    return _mm256_sub_ps(a.v, base);
}
STI v8f OP %(Pv8fas){ //Slow due to division
    __m256 ALIGN den = _mm256_set1_ps(b);
    __m256 ALIGN c = _mm256_div_ps(a.v,den);
    __m256 ALIGN base = _mm256_mul_ps(_mm256_round_ps(c,3+8),den);
    return _mm256_sub_ps(a.v, base);
}

STI v8f approx_recipr(v8f const &a){return _mm256_rcp_ps(a.v);}
STI v8f approx_rsqrt(v8f const &a) {return _mm256_rsqrt_ps(a.v);}
STI v8f infinite8f() {return constant8f<0x7F800000>();}
STI v8f nan8f(int n = 0x10) {return _mm256_castsi256_ps(_mm256_set1_epi32(0x7FC00000 + n));}

STI bool horizontal_or(v8f const &a){return ! _mm256_testz_ps(a.v,a.v);}
//STI bool horizontal_and(v8f const &a){return _mm256_testc_ps(a.v,constant8f<-1>())!= 0);}

STI float horizontal_add(v8f const & a) {
    __m256 t1 = _mm256_hadd_ps(a.v,a.v);
    __m256 t2 = _mm256_hadd_ps(t1,t1);
    __m128 t3 = _mm256_extractf128_ps(t2,1);
    __m128 t4 = _mm_add_ss(_mm256_castps256_ps128(t2),t3);
    return _mm_cvtss_f32(t4);                                       // sign extend to 32 bits
}

STI float get(const v8f& a,const int& b){return (float)a[b];}
STI float get(const v8f& a){return (float)a[0];}

STI int compare(Pv8fab){
    int dif = 0;
    for(int i=0;i<(8);++i){if (std::abs(a[i] - b[i])>0.000001) ++dif;}
    return dif;
}

//}AVX float x8
