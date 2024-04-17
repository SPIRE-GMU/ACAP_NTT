#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include <twiddle_512.h>

using namespace adf;

void ntt_512_last (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out){



	int32 const chess_storage(%chess_alignof(v4int32))   pa[16] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289 }; // q

	v16int32   pa_p  = *((v16int32  * restrict)pa);
	v8int32    pa_pt = *((v8int32  * restrict)pa);

	v8int32  * restrict tw1  = (v8int32  * restrict)tw512;

	v8int32  * restrict tw1c  = (v8int32  * restrict)tw512c;

	v8int32 ma1, ma2;
	v8int32 ma1t, ma2t, ma1t_conv, ma2t_conv;


	v16int32 x1buff, x2buff, nullv, x3buff, temp_buff, x_out, ma2_mod;

	nullv = null_v16int32();
	unsigned int mask;




	for (int i=0; i<16; i++){

		x1buff = upd_v(x1buff, 0, readincr_v4(s1_in));
		x1buff = upd_v(x1buff, 1, readincr_v4(s1_in));
		x1buff = upd_v(x1buff, 2, readincr_v4(s1_in));
		x1buff = upd_v(x1buff, 3, readincr_v4(s1_in));

		//aie::vector<int32, 16> v = x1buff;
		//aie::print(v, true ,"vin1= ");


		ma1 = upd_v(ma1, 0, readincr_v4(s2_in));
		ma1 = upd_v(ma1, 1, readincr_v4(s2_in));
		ma1t = srs(mul(ma1, *tw1++),0);  // tw multiply
		ma1t_conv = srs(mul(srs(mul(ma1, *tw1c++),30),pa_pt),0);

		ma2 = upd_v(ma2, 0, readincr_v4(s2_in));
		ma2 = upd_v(ma2, 1, readincr_v4(s2_in));
		ma2t = srs(mul(ma2, *tw1++),0);  // tw multiply
		ma2t_conv = srs(mul(srs(mul(ma2, *tw1c++),30),pa_pt),0);

		x2buff = sub16(concat(ma1t,ma2t), concat(ma1t_conv,ma2t_conv));

		//v = concat(ma1,ma2);
		//aie::print(v, true ,"vin2= ");

		x3buff = add16(x1buff, x2buff);
		temp_buff = sub16( x3buff, pa_p );

		mask  = lt16(temp_buff, nullv);


		x_out = select16(mask, temp_buff, 0, 0x76543210, 0xFEDCBA98, x3buff , 0, 0x76543210, 0xFEDCBA98);  // if neg take from x3buff


		writeincr_v4(s1_out,ext_v(x_out,0));
		writeincr_v4(s1_out,ext_v(x_out,1));
		writeincr_v4(s1_out,ext_v(x_out,2));
		writeincr_v4(s1_out,ext_v(x_out,3));





		x3buff = sub16(x1buff, x2buff);
		temp_buff = add16( x3buff, pa_p );

		mask  = lt16(x3buff, nullv); //mask set when x3 elem is negative


		x_out = select16(mask, x3buff, 0, 0x76543210, 0xFEDCBA98, temp_buff , 0, 0x76543210, 0xFEDCBA98);  // if neg take from x3buff


		writeincr_v4(s2_out,ext_v(x_out,0));
		writeincr_v4(s2_out,ext_v(x_out,1));
		writeincr_v4(s2_out,ext_v(x_out,2));
		writeincr_v4(s2_out,ext_v(x_out,3));


	}


}

