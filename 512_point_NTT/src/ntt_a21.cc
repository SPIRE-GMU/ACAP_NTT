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


	v8int32   ra_p  = *((v8int32  * restrict)ra);
	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v8int32 ma1, ma2, ma3, ma4, ma1_conv, ma2_conv, ma3_conv, ma4_conv;
	v8int32 ma1t, ma2t, ma3t, ma4t, ma1t_conv, ma2t_conv, ma3t_conv, ma4t_conv;
	ma1t_conv = null_v8int32();

	v16int32 ma1_mod, ma2_mod, ma1_mod1, ma2_mod1, ma1_mod2, ma2_mod2;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	v8int32 * restrict p0 = (v8int32 * restrict) (ntt_01 ); // write to buffer
	v8int32 * restrict p1 = (v8int32 * restrict) (ntt_01 + 16);
	v8int32 * restrict p2 = (v8int32 * restrict) (ntt_01 + 32);
	v8int32 * restrict p3 = (v8int32 * restrict) (ntt_01 + 48);


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

	ma1t = ext_w(ma1_mod,0);
	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);


	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod1 = sub16(concat(ma1t,ma2t), concat(ma1t_conv,ma2t_conv));
	ma2_mod1 = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));



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



	ma1t = ext_w(ma1_mod,0);
	ma2t = srs(mul(ext_w(ma1_mod,1), *tw2++),0);  // tw multiply
	ma3t = srs(mul(ext_w(ma2_mod,0), *tw3++),0);
	ma4t = srs(mul(ext_w(ma2_mod,1), *tw4++),0);

	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), *tw2c++),30),pa_p),0);
	ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), *tw3c++),30),pa_p),0);
	ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), *tw4c++),30),pa_p),0);

	ma1_mod2 = sub16(concat(ma1t,ma2t), concat(ma1t_conv,ma2t_conv));
	ma2_mod2 = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));



	writeincr_v4(s1_out, ext_v(ma1_mod1,0));  //a0
    writeincr_v4(s2_out, ext_v(ma1_mod1,1));  //a1

	writeincr_v4(s1_out, ext_v(ma1_mod2,0));  //a2
    writeincr_v4(s2_out, ext_v(ma1_mod2,1));  //a3

	writeincr_v4(s1_out, ext_v(ma1_mod1,2));  //b0
    writeincr_v4(s2_out, ext_v(ma1_mod1,3));  //b1

	writeincr_v4(s1_out, ext_v(ma1_mod2,2));  //b2
    writeincr_v4(s2_out, ext_v(ma1_mod2,3));  //b3

	writeincr_v4(s1_out, ext_v(ma2_mod1,0));
    writeincr_v4(s2_out, ext_v(ma2_mod1,1));

	writeincr_v4(s1_out, ext_v(ma2_mod2,0));
    writeincr_v4(s2_out, ext_v(ma2_mod2,1));

	writeincr_v4(s1_out, ext_v(ma2_mod1,2));
    writeincr_v4(s2_out, ext_v(ma2_mod1,3));

	writeincr_v4(s1_out, ext_v(ma2_mod2,2));
    writeincr_v4(s2_out, ext_v(ma2_mod2,3));

}




