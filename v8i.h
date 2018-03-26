//{SSE x8 short int. 16-bit x 8=128bits

#define Pv8iab v8i const &a,v8i const &b
#define Pv8ias v8i const &a,int16_t b
#define ALIGN16 __attribute__((aligned(16)))
#define OP operator
#define STI static inline

class v8i{
public:
    __m128i ALIGN16 v;
    v8i(){v= _mm_setzero_si128();}
    v8i(int f){v=_mm_set1_epi16((int16_t)f);}
    v8i(int16_t i0,int16_t i1,int16_t i2,int16_t i3,int16_t i4,int16_t i5,int16_t i6,int16_t i7){v=_mm_setr_epi16(i0, i1, i2, i3,i4,i5,i6,i7);}
    v8i(__m128i const& x){v=x;}
    // OP __m128i() const {return v;}
    void load(int16_t const * p){v=_mm_loadu_si128((__m128i const*)p);}
    void loada(int16_t const * p){v=_mm_load_si128((__m128i const*)p);}
    void store(int16_t * p) {_mm_storeu_si128((__m128i*)p,v);}
    void storea(int16_t * p){_mm_store_si128((__m128i*)p, v);}
    void insert(uint32_t index, int16_t value) {
        switch(index) {
            case 0:v = _mm_insert_epi16(v,value,0);  break;
            case 1:v = _mm_insert_epi16(v,value,1);  break;
            case 2:v = _mm_insert_epi16(v,value,2);  break;
            case 3:v = _mm_insert_epi16(v,value,3);  break;
            case 4:v = _mm_insert_epi16(v,value,4);  break;
            case 5:v = _mm_insert_epi16(v,value,5);  break;
            case 6:v = _mm_insert_epi16(v,value,6);  break;
            case 7:v = _mm_insert_epi16(v,value,7);  break;
        }
    }
    int16_t OP [] (uint32_t index) const {
        switch(index) {
            case 0:return (int16_t)_mm_extract_epi16(v,0);
            case 1:return (int16_t)_mm_extract_epi16(v,1);
            case 2:return (int16_t)_mm_extract_epi16(v,2);
            case 3:return (int16_t)_mm_extract_epi16(v,3);
            case 4:return (int16_t)_mm_extract_epi16(v,4);
            case 5:return (int16_t)_mm_extract_epi16(v,5);
            case 6:return (int16_t)_mm_extract_epi16(v,6);
            case 7:return (int16_t)_mm_extract_epi16(v,7);
        }
    }
};
//Constant creation via templates
template <int16_t i0,int16_t i1,int16_t i2,int16_t i3,int16_t i4,int16_t i5,int16_t i6,int16_t i7>
STI v8i C_I(){static const union {int16_t f[8];v8i ymm;} u = {{i0,i1,i2,i3,i4,i5,i6,i7}}; return u.ymm;}
template <int16_t i0>
STI v8i C_I(){static const union {int16_t f[8];v8i ymm;} u = {{i0,i0,i0,i0,i0,i0,i0,i0}}; return u.ymm;}

//Operators
STI v8i OP +(Pv8iab){return _mm_add_epi16(a.v,b.v);}
STI v8i OP +(Pv8ias){return _mm_add_epi16(a.v,_mm_set1_epi16((int16_t)b));}
STI v8i OP -(Pv8iab){return _mm_sub_epi16(a.v,b.v);}
STI v8i OP -(Pv8ias){return _mm_sub_epi16(a.v,_mm_set1_epi16((int16_t)b));}
STI v8i OP -(v8i const &a){return _mm_sub_epi16(_mm_setzero_si128(),a.v);}

STI v8i &OP ++(v8i &a){a=a+1;return a;}
STI v8i OP ++(v8i &a,int){v8i ALIGN16 a0(a);a=a+1;return a0;}
STI v8i &OP +=(v8i &a,v8i const &b){a=a+b;return a;}
STI v8i &OP --(v8i &a){a=a-1;return a;}
STI v8i OP --(v8i &a,int){v8i ALIGN16 a0=a;a=a-1;return a0;}
STI v8i &OP -=(v8i &a,v8i const &b){a=a-b;return a;}

STI v8i OP *(Pv8iab){return _mm_mullo_epi16(a.v,b.v);}
STI v8i OP *(Pv8ias){return _mm_mullo_epi16(a.v,_mm_set1_epi16((int16_t)b));}
STI v8i &OP *=(v8i &a,v8i const &b){a=a*b;return a;}
STI v8i OP !(v8i const &a){return _mm_cmpeq_epi16(a.v,_mm_setzero_si128());}  //{return _mm_xor_si128(a, _mm_set1_epi16(-1));}
STI v8i OP ~(v8i const &a){return _mm_xor_si128(a.v, _mm_set1_epi32(-1));}
STI v8i OP &(Pv8iab){return _mm_and_si128(a.v,b.v);}
STI v8i OP &&(Pv8iab){return _mm_and_si128(a.v,b.v);}
STI v8i &OP &=(v8i &a,v8i const &b){a=a&b;return a;}
STI v8i OP |(Pv8iab){return _mm_or_si128(a.v,b.v);}
STI v8i OP ||(Pv8iab){return _mm_or_si128(a.v,b.v);}
STI v8i &OP |=(v8i &a,v8i const &b){a=a|b;return a;}
STI v8i OP ^(Pv8iab){return _mm_xor_si128(a.v,b.v);}
STI v8i &OP ^=(v8i &a,v8i const &b){a=a^b;return a;}
STI v8i OP ==(Pv8iab){return _mm_cmpeq_epi16(a.v, b.v);}
STI v8i OP !=(Pv8iab){return  ~(_mm_cmpeq_epi16(a.v, b.v));}
STI v8i OP >(Pv8iab){return  _mm_cmpgt_epi16(a.v,b.v); }
STI v8i OP <(Pv8iab){return  _mm_cmpgt_epi16(b.v,a.v); }
STI v8i OP >=(Pv8iab){return  ~(_mm_cmpgt_epi16(b.v,a.v)); }
STI v8i OP <=(Pv8iab){return  ~(_mm_cmpgt_epi16(a.v,b.v)); }
STI v8i andnot(Pv8iab){return _mm_andnot_si128(a.v,b.v);}  //a&!b
STI v8i OP <<(Pv8ias){return _mm_sll_epi16(a.v,_mm_cvtsi32_si128(b));}  //STI v8i shl8(Pv8iab){return _mm_sll_epi16(a,b);}
STI v8i OP >>(Pv8ias){return _mm_sra_epi16(a.v,_mm_cvtsi32_si128(b));}  //STI v8i shr8(Pv8iab){return _mm_sra_epi16(a,b);}
STI v8i &OP <<=(v8i &a,int const &b){a = a<<b;return a;}
STI v8i &OP >>=(v8i &a,int const &b){a = a>>b;return a;}
STI v8i OP /(Pv8ias){
switch(b) {    //Baaaaaaaad way :S
 case 1:return a;    case 2:return a>>1;  case 4:return a>>2;
 case 8:return a>>3; case 16:return a>>4; case 32:return a>>5;
 case 64:return a>>6;case 128:return a>>7;case 256:return a>>8;
 default: break;}
    int16_t d[8];_mm_storeu_si128((__m128i*)d,a.v);
    for(int i=0;i<(8);++i) d[i] /= b; return _mm_loadu_si128((__m128i const*)d);
}
STI v8i OP /(Pv8iab){ //Baaaaaaaad way :S
    int16_t d[8]; _mm_storeu_si128((__m128i*)d,a.v);
    for(int i=0;i<(8);++i) d[i] /= b[i];return _mm_loadu_si128((__m128i const*)d);
}
STI v8i &OP /=(v8i &a,v8i const &b){a=a/b;return a;}
STI std::ostream &OP<<(std::ostream& output, const v8i& p){output<<"v8i: [";for(int i=0;i<(8);++i) output<<p[i]<<",";output << "]";return output;}
// Each byte in s must be either 0 (false) or 0xFFFFFFFF (true). No other values are allowed.
STI v8i if_select(v8i const &s,Pv8iab){return _mm_blendv_epi8(b.v,a.v,s.v);}
STI v8i if_add(v8i const &f,Pv8iab){return a + (f&b);}
STI v8i if_sub(v8i const &f,Pv8iab){return a - (f&b);}
STI v8i if_mul(v8i const &f,Pv8iab){return a*if_select(f,b,_mm_set1_epi16(1));}
STI v8i if_div(v8i const &f,Pv8iab){return a/if_select(f,b,_mm_set1_epi16(1));}
STI void when_select(v8i& res,v8i const &s,const v8i&a){res=_mm_blendv_epi8(res.v,a.v,s.v);}
STI void when_add(v8i& res,const v8i &f,const v8i& b){res+=(f&b);}
STI void when_sub(v8i& res,const v8i &f,const v8i& b){res-=(f&b);}
STI void when_mul(v8i& res,const v8i &f,const v8i& b){res*=if_select(f,b,_mm_set1_epi16(1));}
STI void when_div(v8i& res,const v8i &f,const v8i& b){res/=if_select(f,b,_mm_set1_epi16(1));}

STI v8i max(Pv8iab){return _mm_max_epi16(a.v,b.v);}
STI v8i min(Pv8iab){return _mm_min_epi16(a.v,b.v);}
STI v8i abs(v8i const &a){ return _mm_sign_epi16(a.v,a.v);}
STI bool horizontal_and(v8i const &a){return _mm_movemask_epi8(a.v) == 0xFFFF;}
STI bool horizontal_or(v8i const &a){return ! _mm_testz_si128(a.v,a.v);}
STI int16_t horizontal_add8(v8i const & a) {
    __m128i sum1  = _mm_hadd_epi16(a.v,a.v);                   // horizontally add 8 elements in 3 steps
    __m128i sum2  = _mm_hadd_epi16(sum1,sum1);
    __m128i sum3  = _mm_hadd_epi16(sum2,sum2);
    int16_t sum4  = (int16_t)_mm_cvtsi128_si32(sum3);      // 16 bit sum
    return  sum4;                                          // sign extend to 32 bits
}

STI int get(const v8i&a,const int& b){return (int)a[b];}
STI int get(const v8i&a){return (int)a[0];}

STI v8i rotate_left(v8i const & a, int16_t b) {
    __m128i left  = _mm_sll_epi16(a.v,_mm_cvtsi32_si128(b & 0x0F));      // a << b
    __m128i right = _mm_srl_epi16(a.v,_mm_cvtsi32_si128((16-b) & 0x0F)); // a >> (16 - b)
    return _mm_or_si128(left,right);                          // or
}
STI v8i rotate_right(v8i const & a, int16_t b) {return rotate_left(a,-b);}

//}
