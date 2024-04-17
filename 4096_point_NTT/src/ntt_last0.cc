#include <adf.h>
#include "twiddle_last.h"
//#include "include0.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_last0 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out){



	int32 const chess_storage(%chess_alignof(v4int32))   pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289}; // q

	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v8int32 * restrict  tw2  = (v8int32 * restrict) last_tw2; // tw pointer changes while not connected as an array
	v8int32 * restrict  tw2c = (v8int32 * restrict) last_tw2c;

	v8int32 * restrict  tw3 =  (v8int32 * restrict) last_tw3;
	v8int32 * restrict  tw3c = (v8int32 * restrict) last_tw3c;

	v8int32 * restrict  tw4 =  (v8int32 * restrict) last_tw4;
	v8int32 * restrict  tw4c = (v8int32 * restrict) last_tw4c;


	v8int32 ma1, ma2, ma3, ma4, ma1_conv, ma2_conv, ma3_conv, ma4_conv;
	v8int32 ma1t, ma2t, ma3t, ma4t, ma1t_conv, ma2t_conv, ma3t_conv, ma4t_conv, ma_tmp;

	ma1t_conv = null_v8int32();
	v16int32 ma1_mod, ma2_mod;




	for (int i = 0; i < 128; i++){

		ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
		ma1 = upd_v(ma1, 1, readincr_v4(s1_in));

		ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
		ma2 = upd_v(ma2, 1, readincr_v4(s1_in));
		//aie::vector<int32, 8> v = ma1_conv;
		//aie::print(v, true ,"vin= ");

		//v = ma2_conv;
		//aie::print(v, true ,"vin= ");


		//ma1t = ma1; //

		ma2t = srs(mul(ma2, *tw2++),0);  // tw multiply

		ma2t_conv = srs(mul(srs(mul(ma2, *tw2c++),30),pa_p),0);

		ma1_mod = sub16(concat(ma1,ma2t), concat(ma1t_conv,ma2t_conv));

		writeincr_v4(s1_out,ext_v(ma1_mod,0));
		writeincr_v4(s2_out,ext_v(ma1_mod,1));
		writeincr_v4(s1_out,ext_v(ma1_mod,2));
		writeincr_v4(s2_out,ext_v(ma1_mod,3));


		ma3 = upd_v(ma3, 0, readincr_v4(s2_in));
		ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

		ma4 = upd_v(ma4, 0, readincr_v4(s2_in));
		ma4 = upd_v(ma4, 1, readincr_v4(s2_in));


		ma3t = srs(mul(ma3, *tw3++),0);
		ma4t = srs(mul(ma4, *tw4++),0);

		ma3t_conv = srs(mul(srs(mul(ma3, *tw3c++),30),pa_p),0);
		ma4t_conv = srs(mul(srs(mul(ma4, *tw4c++),30),pa_p),0);

		ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));



		writeincr_v4(s1_out,ext_v(ma2_mod,0));
		writeincr_v4(s2_out,ext_v(ma2_mod,1));
		writeincr_v4(s1_out,ext_v(ma2_mod,2));
		writeincr_v4(s2_out,ext_v(ma2_mod,3));



	}



}

