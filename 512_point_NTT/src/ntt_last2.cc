#include <adf.h>
#include "include4.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_last2 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out){

	v8int32 * restrict p0   = (v8int32 * restrict)  ntt_last;
	v8int32 * restrict p1   = (v8int32 * restrict) (ntt_last+64);
	v8int32 * restrict p2   = (v8int32 * restrict) (ntt_last+128);
	v8int32 * restrict p3   = (v8int32 * restrict) (ntt_last+192);

	int32 * restrict p_out   = (int32 * restrict)  ntt_last;

	int32 const chess_storage(%chess_alignof(v4int32))   ra[8] =   {349496, 349496, 349496, 349496, 349496, 349496, 349496, 349496}; // [( 1 << 32 ) / q]
	int32 const chess_storage(%chess_alignof(v4int32))   pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289}; // q

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

		ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
		ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);

		ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod

		*p0++ = ext_w(ma1_mod,0);
		*p1++ = ext_w(ma1_mod,1);

		/*writeincr_v4(s1_out,ext_v(ma1_mod,0));
		writeincr_v4(s1_out,ext_v(ma1_mod,1));
		writeincr_v4(s1_out,ext_v(ma1_mod,2));
		writeincr_v4(s1_out,ext_v(ma1_mod,3));*/


		ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
		ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

		ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
		ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

		ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
		ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);


		ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));

		*p2++ = ext_w(ma2_mod,0);
		*p3++ = ext_w(ma2_mod,1);


		/*writeincr_v4(s2_out,ext_v(ma2_mod,0));
		writeincr_v4(s2_out,ext_v(ma2_mod,1));
		writeincr_v4(s2_out,ext_v(ma2_mod,2));
		writeincr_v4(s2_out,ext_v(ma2_mod,3));*/

	}


	for (int i =0; i < 256; i++){

		writeincr(s1_out, *p_out++);
	}


}

