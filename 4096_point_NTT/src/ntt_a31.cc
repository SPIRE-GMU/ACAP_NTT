#include <adf.h>
//#include "twiddle0.h"
//#include "include0.h"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_a31 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out){

	int32 const chess_storage(%chess_alignof(v4int32))   ra[8] =   {349496, 349496, 349496, 349496, 349496, 349496, 349496, 349496}; // [( 1 << 32 ) / q]
	int32 const chess_storage(%chess_alignof(v4int32))   pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289}; // q



	int32 const chess_storage(%chess_alignof(int32)) stage3_tw2[8] =   {1, 7506, 4134, 7260, 1, 7506, 4134, 7260};
	int32 const chess_storage(%chess_alignof(int32)) stage3_tw3[8] =   {1, 7260, 8246, 79, 1, 7260, 8246, 79};
	int32 const chess_storage(%chess_alignof(int32)) stage3_tw4[8] =   {1, 4134, 11567, 8246, 1, 4134, 11567, 8246};

	int32 const chess_storage(%chess_alignof(int32)) stage3_tw2c[8] =   {87374, 655830916, 361205037, 634336857, 87374, 655830916, 361205037, 634336857};
	int32 const chess_storage(%chess_alignof(int32)) stage3_tw3c[8] =   {87374, 634336857, 720487841, 6902563, 87374, 634336857, 720487841, 6902563};
	int32 const chess_storage(%chess_alignof(int32)) stage3_tw4c[8] =   {87374, 361205037, 1010657635, 720487841, 87374, 361205037, 1010657635, 720487841};

    v8int32  tw2  = *((v8int32 * restrict) stage3_tw2); // tw pointer changes while not connected as an array
    v8int32  tw2c = *((v8int32 * restrict) stage3_tw2c);

    v8int32  tw3 = *((v8int32 * restrict) stage3_tw3);
    v8int32  tw3c = *((v8int32 * restrict)stage3_tw3c);

    v8int32  tw4 = *((v8int32 * restrict) stage3_tw4);
    v8int32  tw4c = *((v8int32 * restrict)stage3_tw4c);

    //v8int32 * restrict ntt_0a = (v8int32 * restrict)  ntt_0; // write to buffer




	v8int32   ra_p  = *((v8int32  * restrict)ra);
	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v8int32 ma1, ma2, ma3, ma4, ma1_conv, ma2_conv, ma3_conv, ma4_conv;
	v8int32 ma1t, ma2t, ma3t, ma4t, ma1t_conv, ma2t_conv, ma3t_conv, ma4t_conv, ma_tmp;

	ma1t_conv = null_v8int32();
	v16int32 ma1_mod, ma2_mod;
	v32int32 temp;



	for (int i = 0; i < 32; i++){

		ma1 = upd_v(ma1, 0, readincr_v4(s1_in));
		ma1 = upd_v(ma1, 1, readincr_v4(s2_in));

		ma2 = upd_v(ma2, 0, readincr_v4(s1_in));
		ma2 = upd_v(ma2, 1, readincr_v4(s2_in));

		ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
		ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);

		//aie::vector<int32, 8> v = ma1_conv;
		//aie::print(v, true ,"vin= ");

		//v = ma2_conv;
		//aie::print(v, true ,"vin= ");

		ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod



		ma1t = ext_w(ma1_mod,0);

		ma2t = srs(mul(ext_w(ma1_mod,1), tw2),0);  // tw multiply

		ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), tw2c),30),pa_p),0);

		ma1_mod = sub16(concat(ma1t,ma2t), concat(ma1t_conv,ma2t_conv));

		//aie::vector<int32, 8> v = ma_tmp;
		//aie::print(v, true ,"vin= ");

		//v = tw2;
		//aie::print(v, true ,"vtn= ");

		writeincr_v4(s1_out,ext_v(ma1_mod,0));
		writeincr_v4(s2_out,ext_v(ma1_mod,1));
		writeincr_v4(s1_out,ext_v(ma1_mod,2));
		writeincr_v4(s2_out,ext_v(ma1_mod,3));


		ma3 = upd_v(ma3, 0, readincr_v4(s1_in));
		ma3 = upd_v(ma3, 1, readincr_v4(s2_in));

		ma4 = upd_v(ma4, 0, readincr_v4(s1_in));
		ma4 = upd_v(ma4, 1, readincr_v4(s2_in));

		ma3_conv = srs(mul(srs(mul(ma3,ra_p),32),pa_p),0);  //m_conv = q * [(349496 * m) >> 32 ]
		ma4_conv = srs(mul(srs(mul(ma4,ra_p),32),pa_p),0);

		ma2_mod = sub16(concat(ma3,ma4), concat(ma3_conv,ma4_conv));

		//v = ma2_mod;
		//aie::print(v, true ,"v2m= ");

		ma3t = srs(mul(ext_w(ma2_mod,0), tw3),0);
		ma4t = srs(mul(ext_w(ma2_mod,1), tw4),0);

		ma3t_conv = srs(mul(srs(mul(ext_w(ma2_mod,0), tw3c),30),pa_p),0);
		ma4t_conv = srs(mul(srs(mul(ext_w(ma2_mod,1), tw4c),30),pa_p),0);

		ma2_mod = sub16(concat(ma3t,ma4t), concat(ma3t_conv,ma4t_conv));



		writeincr_v4(s1_out,ext_v(ma2_mod,0));
		writeincr_v4(s2_out,ext_v(ma2_mod,1));
		writeincr_v4(s1_out,ext_v(ma2_mod,2));
		writeincr_v4(s2_out,ext_v(ma2_mod,3));



	}



}

