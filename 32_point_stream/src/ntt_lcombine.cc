#include <adf.h>


#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_lcombine (input_stream_int32  * restrict s1_in,
				   input_stream_int32  * restrict s2_in,
				   output_stream_int32 *  restrict s1_out){


	for (int i =0; i<16; i++){

		writeincr(s1_out,readincr(s1_in)); //a


	}

	for (int i =0; i<16; i++){

		writeincr(s1_out,readincr(s2_in)); //a


	}


}

