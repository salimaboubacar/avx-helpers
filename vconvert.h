//Vector 8xshort <-> 8xfloat conversion
STI __m256 convert(const __m128i& vector_int)  //8xint16_t to 8xfloat
{
 __m128 ALIGN16 lo = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(vector_int));
 __m128 ALIGN16 hi = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(_mm_srli_si128(vector_int, 8)));
 return _mm256_insertf128_ps(_mm256_castps128_ps256(lo),(hi),1);
}

STI __m128i convert(const __m256& vector_float) //8xfloat to 8xint16_t
{
 __m128i ALIGN16 lo =  _mm_cvtps_epi32(_mm256_castps256_ps128(vector_float));
 __m128i ALIGN16 hi =  _mm_cvtps_epi32(_mm256_extractf128_ps(vector_float,1));
  return _mm_packs_epi32(lo,hi);
}
STI v8f convert(const v8i& vector_int)  //8xint16_t to 8xfloat
{
 __m128 ALIGN16 lo = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(vector_int.v));
 __m128 ALIGN16 hi = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(_mm_srli_si128(vector_int.v, 8)));
 return _mm256_insertf128_ps(_mm256_castps128_ps256(lo),(hi),1);
}

STI v8i convert(const v8f& vector_float) //8xfloat to 8xint16_t
{
 __m128i ALIGN16 lo =  _mm_cvtps_epi32(_mm256_castps256_ps128(vector_float.v));
 __m128i ALIGN16 hi =  _mm_cvtps_epi32(_mm256_extractf128_ps(vector_float.v,1));
  return _mm_packs_epi32(lo,hi);
}