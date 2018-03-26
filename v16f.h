//{AVX x8 float. 32-bit x 8=512bits
#define Pv16fab v16f const &a,v16f const &b
#define Pv16fas v16f const &a,float b
#define ALIGN __attribute__((aligned(32)))
#define OP operator
#define STI static inline

template <int i0>
STI __m512 constant8f(){
    static const union {int i[8];__m512 ymm;} u = {{i0,i0,i0,i0,i0,i0,i0,i0}};
    return u.ymm;}

class v16f{
public:
    __m512 ALIGN v;
    v16f(){v= _mm512_setzero_ps();}
    v16f(float f){v=_mm512_set1_ps(f);}
    v16f(float f0,float f1,float f2,float f3,float f4,float f5,float f6,float f7){v=_mm512_setr_ps(f0,f1,f2,f3,f4,f5,f6,f7);}
    v16f(__m512 const& x){v=x;}
    // OP __m512() const {return v;}
    inline void load(float const * p){v=_mm512_loadu_ps(p);}
    inline void loada(float const * p){v=_mm512_load_ps(p); }
    inline void store(float * p){_mm512_storeu_ps(p,v);}
    inline void storea(float * p){_mm512_store_ps(p,v);}
    inline void insert(int index, float value){ v[index]=value; }
    inline float OP [] (int index) const { return v[index];}
};

//Constant creation via templates
template <int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7>
STI v16f C_F(){
    static const union {int f[8];v16f ymm;} u = {{(float)i0,(float)i1,(float)i2,(float)i3,(float)i4,(float)i5,(float)i6,(float)i7}}; return u.ymm;}
template <int i0,int i1>
STI v16f C_F(){
    static const union {float f[8];v16f  ymm;} u = {{(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1,(float)i0/i1}};
    return u.ymm;
}

STI v16f OP  +(Pv16fab){return _mm512_add_ps(a.v,b.v);}
STI v16f OP  +(Pv16fas){return _mm512_add_ps(a.v,_mm512_set1_ps(b));}
STI v16f &OP +=(v16f &a,v16f const &b){a=a+b;return a;}
STI v16f OP  -(Pv16fab){return _mm512_sub_ps(a.v,b.v);}
STI v16f OP  -(Pv16fas){return _mm512_sub_ps(a.v,_mm512_set1_ps(b));}
STI v16f &OP -=(v16f &a,v16f const &b){a=a-b;return a;}
STI v16f OP  *(Pv16fab){return _mm512_mul_ps(a.v,b.v);}
STI v16f OP  *(Pv16fas){return _mm512_mul_ps(a.v,_mm512_set1_ps(b));}
STI v16f &OP *=(v16f &a,v16f const &b){a=a*b;return a;}
STI v16f OP  /(Pv16fab){return _mm512_div_ps(a.v,b.v);}
STI v16f OP  /(Pv16fas){return _mm512_div_ps(a.v,_mm512_set1_ps(b));}
STI v16f &OP /=(v16f &a,v16f const &b){a=a/b;return a;}

STI v16f OP -(v16f const &a){return _mm512_sub_ps(_mm512_setzero_ps(),a.v);}
STI v16f &OP ++(v16f &a){a=a+1;return a;}
STI v16f  OP ++(v16f &a,int){v16f ALIGN a0(a);a=a+1;return a0;}
STI v16f &OP --(v16f &a){a=a-1;return a;}
STI v16f  OP --(v16f &a,int){v16f ALIGN a0=a;a=a-1;return a0;}


STI v16f OP ==(Pv16fab){return _mm512_cmp_ps(a.v,b.v,0);}
STI v16f OP !=(Pv16fab){return _mm512_cmp_ps(a.v,b.v,4);}
STI v16f OP <(Pv16fab) {return _mm512_cmp_ps(a.v,b.v,1);}
STI v16f OP <=(Pv16fab){return _mm512_cmp_ps(a.v,b.v,2);}
STI v16f OP >(Pv16fab) {return _mm512_cmp_ps(b.v,a.v,1);}
STI v16f OP >=(Pv16fab){return _mm512_cmp_ps(b.v,a.v,2);}

STI v16f OP &(Pv16fab){return _mm512_and_ps(a.v,b.v);}
STI v16f OP &&(Pv16fab){return _mm512_and_ps(a.v,b.v);}
//STI v16f &OP &=(v16f &a,v16f const &b){a=a&b;return a;}
STI v16f OP |(Pv16fab){return _mm512_or_ps(a.v,b.v);}
STI v16f OP ||(Pv16fab){return _mm512_or_ps(a.v,b.v);}
//STI v16f &OP |=(v16f &a,v16f const &b){a=a|b;return a;}
STI v16f OP ^(Pv16fab){return _mm512_xor_ps(a.v,b.v);}
//STI v16f &OP ^=(v16f &a,v16f const &b){a=a^b;return a;}

STI v16f OP !(v16f const &a){return _mm512_cmp_ps(a.v,_mm512_setzero_ps(),0);}
//STI v16f OP ~(v16f const &a){return _mm_xor_si128(a.v, _mm_set1_epi32(-1));}


STI v16f andnot(Pv16fab){return _mm512_andnot_ps(b.v,a.v);}
STI std::ostream &OP<<(std::ostream& output, const v16f& p){output<<"v16f: [";for(int i=0;i<(8);++i) output<<p.v[i]<<",";output << "]";return output;}

// Each byte in s must be either 0 (false) or 0xFFFFFFFF (true). No other values are allowed.
STI v16f if_select(v16f const &s,Pv16fab){return _mm512_blendv_ps(b.v,a.v,s.v);}
STI v16f if_add(v16f const &f,Pv16fab){return a + (f&b);}
STI v16f if_sub(v16f const &f,Pv16fab){return a - (f&b);}
STI v16f if_mul(v16f const &f,Pv16fab){return a*if_select(f,b,C_F<1,1>());}
STI v16f if_div(v16f const &f,Pv16fab){return a/if_select(f,b,C_F<1,1>());}
STI void when_select(v16f& res,v16f const &s,const v16f&a){res=_mm512_blendv_ps(res.v,a.v,s.v);}
STI void when_add(v16f& res,const v16f &f,const v16f& b){res+=(f&b);}
STI void when_sub(v16f& res,const v16f &f,const v16f& b){res-=(f&b);}
STI void when_mul(v16f& res,const v16f &f,const v16f& b){res*=if_select(f,b,C_F<1,1>());}
STI void when_div(v16f& res,const v16f &f,const v16f& b){res/=if_select(f,b,C_F<1,1>());}


STI v16f max(Pv16fab){return _mm512_max_ps(a.v,b.v);}
STI v16f min(Pv16fab){return _mm512_min_ps(a.v,b.v);}
STI v16f abs(v16f const &a){
    __m512 ALIGN mask = constant8f<0x7FFFFFFF>();
    return _mm512_and_ps(a.v,mask);}
STI v16f sqrt(v16f const &a){return _mm512_sqrt_ps(a.v);}

// function round: round to nearest integer (even). (result as float vector)
STI v16f round(v16f const &a){return _mm512_round_ps(a.v,0+8);}
// function truncate: round towards zero. (result as float vector)
STI v16f truncate(v16f const &a){return _mm512_round_ps(a.v,3+8);}
// function floor: round towards minus infinity. (result as float vector)
STI v16f floor(v16f const &a){return _mm512_round_ps(a.v,1+8);}
// function ceil: round towards plus infinity. (result as float vector)
STI v16f ceil(v16f const &a){return _mm512_round_ps(a.v,2+8);}


STI v16f OP %(Pv16fab){  //Slow due to division
    __m512 ALIGN c = _mm512_div_ps(a.v,b.v);
    __m512 ALIGN base = _mm512_mul_ps(_mm512_round_ps(c,3+8),b.v);
    return _mm512_sub_ps(a.v, base);
}
STI v16f OP %(Pv16fas){ //Slow due to division
    __m512 ALIGN den = _mm512_set1_ps(b);
    __m512 ALIGN c = _mm512_div_ps(a.v,den);
    __m512 ALIGN base = _mm512_mul_ps(_mm512_round_ps(c,3+8),den);
    return _mm512_sub_ps(a.v, base);
}

STI v16f approx_recipr(v16f const &a){return _mm512_rcp_ps(a.v);}
STI v16f approx_rsqrt(v16f const &a) {return _mm512_rsqrt_ps(a.v);}
STI v16f infinite8f() {return constant8f<0x7F800000>();}
STI v16f nan8f(int n = 0x10) {return _mm512_castsi512_ps(_mm512_set1_epi32(0x7FC00000 + n));}

STI bool horizontal_or(v16f const &a){return ! _mm512_testz_ps(a.v,a.v);}
//STI bool horizontal_and(v16f const &a){return _mm512_testc_ps(a.v,constant8f<-1>())!= 0);}

// TO Check
STI float horizontal_add(v16f const & a) {
    __m512 t1 = _mm512_hadd_ps(a.v,a.v);
    __m512 t2 = _mm512_hadd_ps(t1,t1);
    __m256 t3 = _mm512_extractf256_ps(t2,1);
    __m256 t4 = _mm_add_ss(_mm512_castps512_ps256(t2),t3);
    return _mm_cvtss_f32(t4);
}

STI float get(const v16f& a,const int& b){return (float)a[b];}
STI float get(const v16f& a){return (float)a[0];}

STI int compare(Pv16fab){
    int dif = 0;
    for(int i=0;i<(8);++i){if (std::abs(a[i] - b[i])>0.000001) ++dif;}
    return dif;
}

//}AVX float x8
