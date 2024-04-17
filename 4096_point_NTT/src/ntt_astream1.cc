#include <adf.h>

#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_stream1 (input_stream_int32  * restrict s1_in,
				  input_stream_int32  * restrict s2_in,
				  output_stream_int32 *  restrict s1_out){


	for (int i =0; i<128; i++){

		writeincr_v4(s1_out,readincr_v4(s1_in)); //a
		writeincr_v4(s1_out,readincr_v4(s1_in)); //a

		writeincr_v4(s1_out,readincr_v4(s2_in)); //b
		writeincr_v4(s1_out,readincr_v4(s2_in)); //b

	}

}


void ntt_stream2 (input_stream_int32  * restrict s1_in,
				  input_stream_int32  * restrict s2_in,
				  output_stream_int32 *  restrict s1_out){


	for (int i =0; i<128; i++){

		writeincr_v4(s1_out,readincr_v4(s1_in)); //c
		writeincr_v4(s1_out,readincr_v4(s1_in)); //c

		writeincr_v4(s1_out,readincr_v4(s2_in)); //d
		writeincr_v4(s1_out,readincr_v4(s2_in)); //d

	}

}



