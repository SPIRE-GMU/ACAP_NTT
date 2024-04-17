#include <adf.h>
#include "twiddle1.h"
#include "include1.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_a11 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out){

	int32 const chess_storage(%chess_alignof(v4int32))   ra[8] =   {349496, 349496, 349496, 349496, 349496, 349496, 349496, 349496}; // [( 1 << 32 ) / q]
	int32 const chess_storage(%chess_alignof(v4int32))   pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289}; // q


    v8int32 * restrict tw2  = (v8int32 * restrict) stage1_tw; // tw
    v8int32 * restrict tw2c = (v8int32 * restrict) stage1_twc;

    v8int32 * restrict tw3 = (v8int32 * restrict) (stage1_tw +64);
    v8int32 * restrict tw3c = (v8int32 * restrict)(stage1_twc+64);

    v8int32 * restrict tw4 = (v8int32 * restrict) (stage1_tw + 128);
    v8int32 * restrict tw4c = (v8int32 * restrict)(stage1_twc+ 128);

    v8int32 * restrict ntt_a = (v8int32 * restrict) ntt_1; // write to buffer
    v8int32 * restrict ntt_b = (v8int32 * restrict) ntt_2;
    v8int32 * restrict ntt_c = (v8int32 * restrict) ntt_3;
    v8int32 * restrict ntt_d = (v8int32 * restrict) ntt_4;



	v8int32   ra_p  = *((v8int32  * restrict)ra);
	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v8int32 ma1, ma2, ma3, ma4, ma1_conv, ma2_conv, ma3_conv, ma4_conv;
	v8int32 ma1t, ma2t, ma3t, ma4t, ma1t_conv, ma2t_conv, ma3t_conv, ma4t_conv;

	v16int32 ma1_mod, ma2_mod;


	for (int i = 0; i < 8; i++){

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


		*ntt_a = ext_w(ma1_mod,0);


		ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
		ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
		ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

		ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
		ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
		ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

		ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
		ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

		*(ntt_a   +  8) = ext_w(ma1_mod,0);
		*(ntt_a   + 16) = ext_w(ma2_mod,0);
		*(ntt_a++ + 24) = ext_w(ma2_mod,1);  // eight elem increment

	}

	//ntt_b
	tw2 -=8; tw2c -=8;
	tw3 -=8; tw3c -=8;
	tw4 -=8; tw4c -=8;


	for (int i = 0; i < 8; i++){

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


		*ntt_b = ext_w(ma1_mod,0);


		ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
		ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
		ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

		ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
		ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
		ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

		ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
		ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

		*(ntt_b   +  8) = ext_w(ma1_mod,0);
		*(ntt_b   + 16) = ext_w(ma2_mod,0);
		*(ntt_b++ + 24) = ext_w(ma2_mod,1);  // eight elem increment

	}

	//ntt_c
	tw2 -=8; tw2c -=8;
	tw3 -=8; tw3c -=8;
	tw4 -=8; tw4c -=8;

	for (int i = 0; i < 8; i++){

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


		*ntt_c = ext_w(ma1_mod,0);


		ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
		ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
		ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

		ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
		ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
		ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

		ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
		ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

		*(ntt_c   +  8) = ext_w(ma1_mod,0);
		*(ntt_c   + 16) = ext_w(ma2_mod,0);
		*(ntt_c++ + 24) = ext_w(ma2_mod,1);  // eight elem increment

	}

	//ntt_d
	tw2 -=8; tw2c -=8;
	tw3 -=8; tw3c -=8;
	tw4 -=8; tw4c -=8;

	for (int i = 0; i < 8; i++){

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


		*ntt_d = ext_w(ma1_mod,0);


		ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
		ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
		ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

		ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
		ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
		ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

		ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
		ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

		*(ntt_d   +  8) = ext_w(ma1_mod,0);
		*(ntt_d   + 16) = ext_w(ma2_mod,0);
		*(ntt_d++ + 24) = ext_w(ma2_mod,1);  // eight elem increment

	}


	v4int32 * restrict ntt_0wa0;  // write to stream
	v4int32 * restrict ntt_0wa1;
	v4int32 * restrict ntt_0wa2;
	v4int32 * restrict ntt_0wa3;

	for (int i = 0; i<256; i+=64){

		ntt_0wa0 = (v4int32 * restrict)  (ntt_1+ i);
		ntt_0wa1 = (v4int32 * restrict)  (ntt_1+ i + 16);
		ntt_0wa2 = (v4int32 * restrict)  (ntt_1+ i + 32);
		ntt_0wa3 = (v4int32 * restrict)  (ntt_1+ i + 48);


		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

		////////////////////////////////////////////////////

		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

	}


	for (int i = 0; i<256; i+=64){

		ntt_0wa0 = (v4int32 * restrict)  (ntt_2+ i);
		ntt_0wa1 = (v4int32 * restrict)  (ntt_2+ i + 16);
		ntt_0wa2 = (v4int32 * restrict)  (ntt_2+ i + 32);
		ntt_0wa3 = (v4int32 * restrict)  (ntt_2+ i + 48);


		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

		////////////////////////////////////////////////////

		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

	}


	for (int i = 0; i<256; i+=64){

		ntt_0wa0 = (v4int32 * restrict)  (ntt_3+ i);
		ntt_0wa1 = (v4int32 * restrict)  (ntt_3+ i + 16);
		ntt_0wa2 = (v4int32 * restrict)  (ntt_3+ i + 32);
		ntt_0wa3 = (v4int32 * restrict)  (ntt_3+ i + 48);


		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

		////////////////////////////////////////////////////

		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

	}

	for (int i = 0; i<256; i+=64){

		ntt_0wa0 = (v4int32 * restrict)  (ntt_4+ i);
		ntt_0wa1 = (v4int32 * restrict)  (ntt_4+ i + 16);
		ntt_0wa2 = (v4int32 * restrict)  (ntt_4+ i + 32);
		ntt_0wa3 = (v4int32 * restrict)  (ntt_4+ i + 48);


		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

		////////////////////////////////////////////////////

		writeincr_v4(s1_out, *ntt_0wa0++);
		writeincr_v4(s2_out, *ntt_0wa0++);

		writeincr_v4(s1_out, *ntt_0wa1++);
		writeincr_v4(s2_out, *ntt_0wa1++);

		writeincr_v4(s1_out, *ntt_0wa2++);
		writeincr_v4(s2_out, *ntt_0wa2++);

		writeincr_v4(s1_out, *ntt_0wa3++);
		writeincr_v4(s2_out, *ntt_0wa3++);

	}




}




