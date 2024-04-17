#include <adf.h>
#include "twiddle2.h"
#include "include2.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_a21 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out){

	int32 const chess_storage(%chess_alignof(v4int32))   ra[8] =   {349496, 349496, 349496, 349496, 349496, 349496, 349496, 349496}; // [( 1 << 32 ) / q]
	int32 const chess_storage(%chess_alignof(v4int32))   pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289}; // q


    v8int32 * restrict tw2  = (v8int32 * restrict) stage2_tw; // tw
    v8int32 * restrict tw2c = (v8int32 * restrict) stage2_twc;

    v8int32 * restrict tw3 = (v8int32 * restrict) (stage2_tw +16);
    v8int32 * restrict tw3c = (v8int32 * restrict)(stage2_twc+16);

    v8int32 * restrict tw4 = (v8int32 * restrict) (stage2_tw + 32);
    v8int32 * restrict tw4c = (v8int32 * restrict)(stage2_twc+ 32);

    v8int32 * restrict ntt_a0 = (v8int32 * restrict) ntt_01; // write to buffer
    v8int32 * restrict ntt_a1 = (v8int32 * restrict) ntt_02;
    v8int32 * restrict ntt_a2 = (v8int32 * restrict) ntt_03;
    v8int32 * restrict ntt_a3 = (v8int32 * restrict) ntt_04;

    v8int32 * restrict ntt_b0 = (v8int32 * restrict) ntt_05;
    v8int32 * restrict ntt_b1 = (v8int32 * restrict) ntt_06;
    v8int32 * restrict ntt_b2 = (v8int32 * restrict) ntt_07;
    v8int32 * restrict ntt_b3 = (v8int32 * restrict) ntt_08;

    v8int32 * restrict ntt_c0 = (v8int32 * restrict) ntt_09;
    v8int32 * restrict ntt_c1 = (v8int32 * restrict) ntt_10;
    v8int32 * restrict ntt_c2 = (v8int32 * restrict) ntt_11;
    v8int32 * restrict ntt_c3 = (v8int32 * restrict) ntt_12;

    v8int32 * restrict ntt_d0 = (v8int32 * restrict) ntt_13;
    v8int32 * restrict ntt_d1 = (v8int32 * restrict) ntt_14;
    v8int32 * restrict ntt_d2 = (v8int32 * restrict) ntt_15;
    v8int32 * restrict ntt_d3 = (v8int32 * restrict) ntt_16;



	v8int32   ra_p  = *((v8int32  * restrict)ra);
	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v8int32 ma1, ma2, ma3, ma4, ma1_conv, ma2_conv, ma3_conv, ma4_conv;
	v8int32 ma1t, ma2t, ma3t, ma4t, ma1t_conv, ma2t_conv, ma3t_conv, ma4t_conv;

	v16int32 ma1_mod, ma2_mod;


	//ntt_a0

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a0   + 2) = ext_w(ma1_mod,0);
	*(ntt_a0   + 4) = ext_w(ma2_mod,0);
	*(ntt_a0++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a0   + 2) = ext_w(ma1_mod,0);
	*(ntt_a0   + 4) = ext_w(ma2_mod,0);
	*(ntt_a0   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req



	//ntt_a1
	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a1   + 2) = ext_w(ma1_mod,0);
	*(ntt_a1   + 4) = ext_w(ma2_mod,0);
	*(ntt_a1++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a1   + 2) = ext_w(ma1_mod,0);
	*(ntt_a1   + 4) = ext_w(ma2_mod,0);
	*(ntt_a1   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_a2

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a2   + 2) = ext_w(ma1_mod,0);
	*(ntt_a2   + 4) = ext_w(ma2_mod,0);
	*(ntt_a2++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a2   + 2) = ext_w(ma1_mod,0);
	*(ntt_a2   + 4) = ext_w(ma2_mod,0);
	*(ntt_a2   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req



	//ntt_a3

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a3   + 2) = ext_w(ma1_mod,0);
	*(ntt_a3   + 4) = ext_w(ma2_mod,0);
	*(ntt_a3++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_a3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_a3   + 2) = ext_w(ma1_mod,0);
	*(ntt_a3   + 4) = ext_w(ma2_mod,0);
	*(ntt_a3   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_b0

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b0   + 2) = ext_w(ma1_mod,0);
	*(ntt_b0   + 4) = ext_w(ma2_mod,0);
	*(ntt_b0++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b0   + 2) = ext_w(ma1_mod,0);
	*(ntt_b0   + 4) = ext_w(ma2_mod,0);
	*(ntt_b0   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_b1

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b1   + 2) = ext_w(ma1_mod,0);
	*(ntt_b1   + 4) = ext_w(ma2_mod,0);
	*(ntt_b1++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b1   + 2) = ext_w(ma1_mod,0);
	*(ntt_b1   + 4) = ext_w(ma2_mod,0);
	*(ntt_b1   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req



	//ntt_b2

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b2   + 2) = ext_w(ma1_mod,0);
	*(ntt_b2   + 4) = ext_w(ma2_mod,0);
	*(ntt_b2++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b2   + 2) = ext_w(ma1_mod,0);
	*(ntt_b2   + 4) = ext_w(ma2_mod,0);
	*(ntt_b2   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_b3

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b3   + 2) = ext_w(ma1_mod,0);
	*(ntt_b3   + 4) = ext_w(ma2_mod,0);
	*(ntt_b3++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_b3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_b3   + 2) = ext_w(ma1_mod,0);
	*(ntt_b3   + 4) = ext_w(ma2_mod,0);
	*(ntt_b3   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req



	//ntt_c0

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c0   + 2) = ext_w(ma1_mod,0);
	*(ntt_c0   + 4) = ext_w(ma2_mod,0);
	*(ntt_c0++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c0   + 2) = ext_w(ma1_mod,0);
	*(ntt_c0   + 4) = ext_w(ma2_mod,0);
	*(ntt_c0   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req




	//ntt_c1

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c1   + 2) = ext_w(ma1_mod,0);
	*(ntt_c1   + 4) = ext_w(ma2_mod,0);
	*(ntt_c1++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c1   + 2) = ext_w(ma1_mod,0);
	*(ntt_c1   + 4) = ext_w(ma2_mod,0);
	*(ntt_c1   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req



	//ntt_c2

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c2   + 2) = ext_w(ma1_mod,0);
	*(ntt_c2   + 4) = ext_w(ma2_mod,0);
	*(ntt_c2++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c2   + 2) = ext_w(ma1_mod,0);
	*(ntt_c2   + 4) = ext_w(ma2_mod,0);
	*(ntt_c2   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_c3

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c3   + 2) = ext_w(ma1_mod,0);
	*(ntt_c3   + 4) = ext_w(ma2_mod,0);
	*(ntt_c3++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_c3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_c3   + 2) = ext_w(ma1_mod,0);
	*(ntt_c3   + 4) = ext_w(ma2_mod,0);
	*(ntt_c3   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_d0

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d0   + 2) = ext_w(ma1_mod,0);
	*(ntt_d0   + 4) = ext_w(ma2_mod,0);
	*(ntt_d0++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d0 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d0   + 2) = ext_w(ma1_mod,0);
	*(ntt_d0   + 4) = ext_w(ma2_mod,0);
	*(ntt_d0   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_d1

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d1   + 2) = ext_w(ma1_mod,0);
	*(ntt_d1   + 4) = ext_w(ma2_mod,0);
	*(ntt_d1++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d1 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d1   + 2) = ext_w(ma1_mod,0);
	*(ntt_d1   + 4) = ext_w(ma2_mod,0);
	*(ntt_d1   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req


	//ntt_d2

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d2   + 2) = ext_w(ma1_mod,0);
	*(ntt_d2   + 4) = ext_w(ma2_mod,0);
	*(ntt_d2++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d2 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d2   + 2) = ext_w(ma1_mod,0);
	*(ntt_d2   + 4) = ext_w(ma2_mod,0);
	*(ntt_d2   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req



	//ntt_d3

	tw2 -=2; tw2c -=2;
	tw3 -=2; tw3c -=2;
	tw4 -=2; tw4c -=2;

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d3   + 2) = ext_w(ma1_mod,0);
	*(ntt_d3   + 4) = ext_w(ma2_mod,0);
	*(ntt_d3++ + 6) = ext_w(ma2_mod,1);  // eight elem increment

	///////////////////////////////////////////////////////

	ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
	ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

	ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
	ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

	ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
	ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

	ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
	ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);
	ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
	ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod
	ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));


	*ntt_d3 = ext_w(ma1_mod,0);


	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
	ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

	*(ntt_d3   + 2) = ext_w(ma1_mod,0);
	*(ntt_d3   + 4) = ext_w(ma2_mod,0);
	*(ntt_d3   + 6) = ext_w(ma2_mod,1);  // eight elem increment not req





    v4int32 * restrict ntt_wa0 = (v4int32 * restrict) ntt_01; // write to stream
    v4int32 * restrict ntt_wa1 = (v4int32 * restrict) ntt_02;
    v4int32 * restrict ntt_wa2 = (v4int32 * restrict) ntt_03;
    v4int32 * restrict ntt_wa3 = (v4int32 * restrict) ntt_04;

    v4int32 * restrict ntt_wb0 = (v4int32 * restrict) ntt_05;
    v4int32 * restrict ntt_wb1 = (v4int32 * restrict) ntt_06;
    v4int32 * restrict ntt_wb2 = (v4int32 * restrict) ntt_07;
    v4int32 * restrict ntt_wb3 = (v4int32 * restrict) ntt_08;

    v4int32 * restrict ntt_wc0 = (v4int32 * restrict) ntt_09;
    v4int32 * restrict ntt_wc1 = (v4int32 * restrict) ntt_10;
    v4int32 * restrict ntt_wc2 = (v4int32 * restrict) ntt_11;
    v4int32 * restrict ntt_wc3 = (v4int32 * restrict) ntt_12;

    v4int32 * restrict ntt_wd0 = (v4int32 * restrict) ntt_13;
    v4int32 * restrict ntt_wd1 = (v4int32 * restrict) ntt_14;
    v4int32 * restrict ntt_wd2 = (v4int32 * restrict) ntt_15;
    v4int32 * restrict ntt_wd3 = (v4int32 * restrict) ntt_16;



    for (int i = 0; i < 4; i++){

    	  writeincr_v4(s1_out, *ntt_wa0++);
    	    writeincr_v4(s2_out, *ntt_wa0++);
    	    writeincr_v4(s1_out, *ntt_wa0++);
    	    writeincr_v4(s2_out, *ntt_wa0++);


    	    writeincr_v4(s1_out, *ntt_wb0++);
    	    writeincr_v4(s2_out, *ntt_wb0++);
    	    writeincr_v4(s1_out, *ntt_wb0++);
    	    writeincr_v4(s2_out, *ntt_wb0++);

    	    writeincr_v4(s1_out, *ntt_wc0++);
    	    writeincr_v4(s2_out, *ntt_wc0++);
    	    writeincr_v4(s1_out, *ntt_wc0++);
    	    writeincr_v4(s2_out, *ntt_wc0++);


    	    writeincr_v4(s1_out, *ntt_wd0++);
    	    writeincr_v4(s2_out, *ntt_wd0++);
    	    writeincr_v4(s1_out, *ntt_wd0++);
    	    writeincr_v4(s2_out, *ntt_wd0++);


    	    writeincr_v4(s1_out, *ntt_wa1++);
    	    writeincr_v4(s2_out, *ntt_wa1++);
    	    writeincr_v4(s1_out, *ntt_wa1++);
    	    writeincr_v4(s2_out, *ntt_wa1++);

    	    writeincr_v4(s1_out, *ntt_wb1++);
    	    writeincr_v4(s2_out, *ntt_wb1++);
    	    writeincr_v4(s1_out, *ntt_wb1++);
    	    writeincr_v4(s2_out, *ntt_wb1++);


    	    writeincr_v4(s1_out, *ntt_wc1++);
    	    writeincr_v4(s2_out, *ntt_wc1++);
    	    writeincr_v4(s1_out, *ntt_wc1++);
    	    writeincr_v4(s2_out, *ntt_wc1++);

    	    writeincr_v4(s1_out, *ntt_wd1++);
    	    writeincr_v4(s2_out, *ntt_wd1++);
    	    writeincr_v4(s1_out, *ntt_wd1++);
    	    writeincr_v4(s2_out, *ntt_wd1++);


    	    writeincr_v4(s1_out, *ntt_wa2++);
    	    writeincr_v4(s2_out, *ntt_wa2++);
    	    writeincr_v4(s1_out, *ntt_wa2++);
    	    writeincr_v4(s2_out, *ntt_wa2++);


    	    writeincr_v4(s1_out, *ntt_wb2++);
    	    writeincr_v4(s2_out, *ntt_wb2++);
    	    writeincr_v4(s1_out, *ntt_wb2++);
    	    writeincr_v4(s2_out, *ntt_wb2++);


    	    writeincr_v4(s1_out, *ntt_wc2++);
    	    writeincr_v4(s2_out, *ntt_wc2++);
    	    writeincr_v4(s1_out, *ntt_wc2++);
    	    writeincr_v4(s2_out, *ntt_wc2++);


    	    writeincr_v4(s1_out, *ntt_wd2++);
    	    writeincr_v4(s2_out, *ntt_wd2++);
    	    writeincr_v4(s1_out, *ntt_wd2++);
    	    writeincr_v4(s2_out, *ntt_wd2++);



    	    writeincr_v4(s1_out, *ntt_wa3++);
    	    writeincr_v4(s2_out, *ntt_wa3++);
    	    writeincr_v4(s1_out, *ntt_wa3++);
    	    writeincr_v4(s2_out, *ntt_wa3++);


    	    writeincr_v4(s1_out, *ntt_wb3++);
    	    writeincr_v4(s2_out, *ntt_wb3++);
    	    writeincr_v4(s1_out, *ntt_wb3++);
    	    writeincr_v4(s2_out, *ntt_wb3++);

    	    writeincr_v4(s1_out, *ntt_wc3++);
    	    writeincr_v4(s2_out, *ntt_wc3++);
    	    writeincr_v4(s1_out, *ntt_wc3++);
    	    writeincr_v4(s2_out, *ntt_wc3++);

    	    writeincr_v4(s1_out, *ntt_wd3++);
    	    writeincr_v4(s2_out, *ntt_wd3++);
    	    writeincr_v4(s1_out, *ntt_wd3++);
    	    writeincr_v4(s2_out, *ntt_wd3++);

    }


}




