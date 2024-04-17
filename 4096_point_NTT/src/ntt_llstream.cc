#include <adf.h>

#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_llstream1 (input_stream_int32  * restrict s1_in,
				  output_stream_int32 *  restrict s1_out,
				  output_stream_int32 *  restrict s2_out){


	for (int i =0; i<128; i++){

		writeincr_v4(s1_out,readincr_v4(s1_in)); //a
		writeincr_v4(s1_out,readincr_v4(s1_in)); //a

		writeincr_v4(s2_out,readincr_v4(s1_in)); //b
		writeincr_v4(s2_out,readincr_v4(s1_in)); //b

	}

}


void ntt_llstream2 (input_stream_int32  * restrict s1_in,
				  output_stream_int32 *  restrict s1_out,
				  output_stream_int32 *  restrict s2_out){


	for (int i =0; i<128; i++){

		writeincr_v4(s1_out,readincr_v4(s1_in)); //c
		writeincr_v4(s1_out,readincr_v4(s1_in)); //c

		writeincr_v4(s2_out,readincr_v4(s1_in)); //d
		writeincr_v4(s2_out,readincr_v4(s1_in)); //d

	}

}
