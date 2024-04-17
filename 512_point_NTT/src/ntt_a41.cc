#include <adf.h>

#include "include4.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_a41 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out){

	int32 const chess_storage(%chess_alignof(v4int32))   ra[8] =   {349496, 349496, 349496, 349496, 349496, 349496, 349496, 349496}; // [( 1 << 32 ) / q]
	int32 const chess_storage(%chess_alignof(v4int32))   pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289}; // q



    int32 * restrict ntt_4a = (int32 * restrict)  ntt_4_1; // write to buffer
    int32 * restrict ntt_4aw = (int32 * restrict)  ntt_4_1; // write to stream




	v8int32   ra_p  = *((v8int32  * restrict)ra);
	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v8int32 ma1, ma2, ma3, ma4, ma1_conv, ma2_conv, ma3_conv, ma4_conv;
	v8int32 ma1t, ma2t, ma3t, ma4t, ma1t_conv, ma2t_conv, ma3t_conv, ma4t_conv;

	v16int32 ma1_mod, ma2_mod;


	for (int i = 0; i < 2; i++){

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

		//aie::vector<int32, 16> v = ma1_mod;
		//aie::print(v, true ,"vm1= ");

    	//v = ma2_mod;
    	//aie::print(v, true ,"vm2= ");

		*ntt_4a         = ext_elem (ma1_mod, 0);
		*(ntt_4a + 1)   = ext_elem (ma1_mod, 4);

		*(ntt_4a + 4)  = ext_elem (ma1_mod, 1);
		*(ntt_4a + 5)  = ext_elem (ma1_mod, 5);

		*(ntt_4a + 8) = ext_elem (ma1_mod, 2);
		*(ntt_4a + 9) = ext_elem (ma1_mod, 6);

		*(ntt_4a + 12) = ext_elem (ma1_mod, 3);
		*(ntt_4a + 13) = ext_elem (ma1_mod, 7);

		*(ntt_4a + 16) = ext_elem (ma1_mod, 8);
		*(ntt_4a + 17) = ext_elem (ma1_mod, 12);

		*(ntt_4a + 20) = ext_elem (ma1_mod, 9);
		*(ntt_4a + 21) = ext_elem (ma1_mod, 13);

		*(ntt_4a + 24) = ext_elem (ma1_mod, 10);
		*(ntt_4a + 25) = ext_elem (ma1_mod, 14);

		*(ntt_4a + 28) = ext_elem (ma1_mod, 11);
		*(ntt_4a + 29) = ext_elem (ma1_mod, 15);


		//////////////////////////////////////////////////////

		*(ntt_4a + 32) = ext_elem (ma2_mod, 0);
		*(ntt_4a + 33) = ext_elem (ma2_mod, 4);

		*(ntt_4a + 36) = ext_elem (ma2_mod, 1);
		*(ntt_4a + 37) = ext_elem (ma2_mod, 5);

		*(ntt_4a + 40) = ext_elem (ma2_mod, 2);
		*(ntt_4a + 41) = ext_elem (ma2_mod, 6);

		*(ntt_4a + 44) = ext_elem (ma2_mod, 3);
		*(ntt_4a + 45) = ext_elem (ma2_mod, 7);

		*(ntt_4a + 48) = ext_elem (ma2_mod, 8);
		*(ntt_4a + 49) = ext_elem (ma2_mod, 12);

		*(ntt_4a + 52) = ext_elem (ma2_mod, 9);
		*(ntt_4a + 53) = ext_elem (ma2_mod, 13);

		*(ntt_4a + 56) = ext_elem (ma2_mod, 10);
		*(ntt_4a + 57) = ext_elem (ma2_mod, 14);

		*(ntt_4a + 60) = ext_elem (ma2_mod, 11);
		*(ntt_4a + 61) = ext_elem (ma2_mod, 15);


		ntt_4a += 2;


	}


	for (int j = 0; j < 64; j++){

		writeincr(s1_out, *ntt_4aw++);
	}



}

