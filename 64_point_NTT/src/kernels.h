#include <adf.h>



using namespace adf;


void ntt_16 (input_stream_int32  * restrict s1_in,
			 output_stream_int32 *  restrict s1_out);

void ntt_stream1 (input_stream_int32  * restrict s1_in,
				  input_stream_int32  * restrict s2_in,
				  output_stream_int32 *  restrict s1_out);

void ntt_stream2 (input_stream_int32  * restrict s1_in,
				  input_stream_int32  * restrict s2_in,
				  output_stream_int32 *  restrict s1_out);


void ntt_last0 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out);


void ntt_last1 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out);

void ntt_last2 (input_stream_int32  * restrict s1_in,
			  input_stream_int32  * restrict s2_in,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out);

void ntt_llstream1 (input_stream_int32  * restrict s1_in,
				  output_stream_int32 *  restrict s1_out,
				  output_stream_int32 *  restrict s2_out);


void ntt_llstream2 (input_stream_int32  * restrict s1_in,
				  output_stream_int32 *  restrict s1_out,
				  output_stream_int32 *  restrict s2_out);


