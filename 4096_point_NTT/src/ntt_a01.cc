#include <adf.h>
#include "twiddle0.h"
#include "include0.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_a01 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out){

	int32 const chess_storage(%chess_alignof(v4int32))   ra[8] =   {349496, 349496, 349496, 349496, 349496, 349496, 349496, 349496}; // [( 1 << 32 ) / q]
	int32 const chess_storage(%chess_alignof(v4int32))   pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289}; // q


    v8int32 * restrict tw2  = (v8int32 * restrict) stage0_tw; // tw
    v8int32 * restrict tw2c = (v8int32 * restrict) stage0_twc;

    v8int32 * restrict tw3 = (v8int32 * restrict) (stage0_tw +256);
    v8int32 * restrict tw3c = (v8int32 * restrict)(stage0_twc+256);

    v8int32 * restrict tw4 = (v8int32 * restrict) (stage0_tw + 512);
    v8int32 * restrict tw4c = (v8int32 * restrict)(stage0_twc+ 512);

    v8int32 * restrict ntt_0a = (v8int32 * restrict)  ntt_0; // write to buffer



	v8int32   ra_p  = *((v8int32  * restrict)ra);
	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v8int32 ma1, ma2, ma3, ma4, ma1_conv, ma2_conv, ma3_conv, ma4_conv;
	v8int32 ma1t, ma2t, ma3t, ma4t, ma1t_conv, ma2t_conv, ma3t_conv, ma4t_conv;

	v16int32 ma1_mod, ma2_mod;


	for (int i = 0; i < 32; i++){

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


		*ntt_0a = ext_w(ma1_mod,0);


		ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
		ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
		ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

		ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
		ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
		ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

		ma1_mod = sub16(concat(ma2t,ma3t), concat(ma2t_conv,ma3t_conv));
		ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));

		*(ntt_0a   + 32) = ext_w(ma1_mod,0);
		*(ntt_0a   + 64) = ext_w(ma2_mod,0);
		*(ntt_0a++ + 96) = ext_w(ma2_mod,1);  // eight elem increment

	}

	v4int32 * restrict ntt_0wa0;  // write to stream
	v4int32 * restrict ntt_0wa1;
	v4int32 * restrict ntt_0wa2;
	v4int32 * restrict ntt_0wa3;

	for (int i = 0; i<1024; i+=256){

		ntt_0wa0 = (v4int32 * restrict)  (ntt_0 + i);
		ntt_0wa1 = (v4int32 * restrict)  (ntt_0 + i + 64);
		ntt_0wa2 = (v4int32 * restrict)  (ntt_0 + i + 128);
		ntt_0wa3 = (v4int32 * restrict)  (ntt_0 + i + 192);

		for (int j =0; j < 8; j++){

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



}

