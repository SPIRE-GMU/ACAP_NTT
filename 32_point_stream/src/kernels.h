#include <adf.h>

using namespace adf;

void ntt_16 (input_stream_int32  * restrict s1_in,
			 output_stream_int32 *  restrict s1_out);

void ntt_last (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out);

void ntt_lcombine (input_stream_int32  * restrict s1_in,
				   input_stream_int32  * restrict s2_in,
				   output_stream_int32 *  restrict s1_out);
