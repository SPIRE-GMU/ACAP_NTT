#include <adf.h>

#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

using namespace adf;

void ntt_a00 (input_window_int32  * restrict inputx,
			  output_stream_int32 *  restrict s1_out,
			  output_stream_int32 *  restrict s2_out){


   int32 *xbuff =  (int32 *)inputx->ptr;




	v8int32 * restrict pi0 = (v8int32 * restrict)  xbuff;
	v8int32 * restrict pi1 = (v8int32 * restrict) (xbuff + 64);
	v8int32 * restrict pi2 = (v8int32 * restrict) (xbuff + 128);
	v8int32 * restrict pi3 = (v8int32 * restrict) (xbuff + 192);




	int16 const chess_storage(%chess_alignof(v4int32)) tw[16] =   {1, 1479, -1, -1479,
											                    0,  0,   0,  0, 0,  0,   0,  0, 0,  0,   0,  0}; // twiddle for 4-point NTT (w_4)^1 == (w_4096)^1024

	int32 const chess_storage(%chess_alignof(v4int32)) pn1[16] =   {0,  0,   0,  0, 0,  0,   0,  0, 18187720, 18187720, 18187720, 18187720, 18187720, 18187720, 18187720, 18187720};
	int32 const chess_storage(%chess_alignof(v4int32)) pn2[16] =   {24578, 24578, 24578, 24578, 24578, 24578, 24578, 24578, 18187720, 18187720, 18187720, 18187720, 18187720, 18187720, 18187720, 18187720};


	v16int32 pna, pnb;

	pna = *((v16int32  * restrict)pn1);
	pnb = *((v16int32  * restrict)pn2);

	v16int16 zbuff = *((v16int16 *)tw);

	v32int32 x1buff;
	v8int32 m0, m1, m2, m3;

	v16int32 x2buff, x3buff;

	v16int32 nullv = null_v16int32();
	unsigned int mask;

	//aie::tile tile=aie::tile::current();
	//unsigned long long time1=tile.cycles();


	for (int i =0; i<8; i++){

		x1buff = upd_w(x1buff, 0, *pi0++);
		x1buff = upd_w(x1buff, 1, *pi1++);
		x1buff = upd_w(x1buff, 3, *pi2++);
		x1buff = upd_w(x1buff, 2, *pi3++);

		//aie::vector<int32, 32> v = x1buff;
		//aie::print(v, true ,"vin= ");

		m0 =     srs(lmul8_sym (x1buff, 0, 0x76543210, 8, 24, zbuff, 0, 0x00000000, 0),0);
		m1 = srs(lmul8_antisym (x1buff, 0, 0x76543210, 8, 24, zbuff, 0, 0x00000000, 1),0);
		m2 =     srs(lmul8_sym (x1buff, 0, 0x76543210, 8, 24, zbuff, 0, 0x00000000, 2),0);
		m3 = srs(lmul8_antisym (x1buff, 0, 0x76543210, 8, 24, zbuff, 0, 0x00000000, 3),0);

		mask  = lt16(concat(m0,m1), nullv);  // checks negative element

		x2buff = add16(concat(m0,m1) , pna);
		x3buff = select16(mask, concat(m0,m1), 0, 0x76543210, 0xFEDCBA98, x2buff , 0, 0x76543210, 0xFEDCBA98); // if x1 elem negative, take elem from x2

		writeincr_v4(s1_out,ext_v(x3buff,0));
		writeincr_v4(s2_out,ext_v(x3buff,1));
		writeincr_v4(s1_out,ext_v(x3buff,2));
		writeincr_v4(s2_out,ext_v(x3buff,3));

		mask  = lt16(concat(m2,m3), nullv);  // checks negative element

		x2buff = add16(concat(m2,m3) , pnb);
		x3buff = select16(mask, concat(m2,m3), 0, 0x76543210, 0xFEDCBA98, x2buff , 0, 0x76543210, 0xFEDCBA98); // if x1 elem negative, take elem from x2


		writeincr_v4(s1_out,ext_v(x3buff,0));
		writeincr_v4(s2_out,ext_v(x3buff,1));
		writeincr_v4(s1_out,ext_v(x3buff,2));
		writeincr_v4(s2_out,ext_v(x3buff,3));

	}

	//unsigned long long time2=tile.cycles(); //second time
	//long long time=time2-time1;
	//printf("concat %lld \n", time);


}

