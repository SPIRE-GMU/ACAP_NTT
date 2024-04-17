#include <adf.h>

using namespace adf;

void ntt_16 (input_stream_int32  * restrict s1_in,
			 output_stream_int32 *  restrict s1_out){



	int32 const chess_storage(%chess_alignof(v4int32)) tw[8] =   {1, 1479, -1, -1479,
											                      	  0,  0,   0,  0};

	v8int32 zbuff = *((v8int32 *)tw);

	int32 const chess_storage(%chess_alignof(v4int32)) tw_sub[16] =   { 1, 1, 1, 1,
																		1, 4134, 11567, 8246,
																		1, 8246, 5146, 1479,
																		1, 11567, 8155, 5146
																	   }; // twiddle for 16-point NTT (w_16)^1  == (w_4096)^256 // rotate for 0x2310


	int32 const chess_storage(%chess_alignof(v4int32)) tw_subc[16] =   {87374, 87374, 87374, 87374,
																		87374, 361205037, 1010657635, 720487841,
																		87374, 720487841, 449627750, 129226475,
																		87374, 1010657635, 712536786, 449627750
																	   };

	v8int32 tsub1 = *(v8int32 * restrict) tw_sub;
	v8int32 tsub2 = *(v8int32 * restrict) (tw_sub + 8);

	v8int32 tsub1c = *(v8int32 * restrict) tw_subc;
	v8int32 tsub2c = *(v8int32 * restrict) (tw_subc + 8);


	int32 const chess_storage(%chess_alignof(v4int32))  ra[8] =   {349496, 349496, 349496, 349496, 349496, 349496, 349496, 349496};  // 1<<32
	int32 const chess_storage(%chess_alignof(v4int32))  pa[8] =   {12289, 12289, 12289, 12289, 12289, 12289, 12289, 12289};  //q

	int32 const chess_storage(%chess_alignof(v4int32)) pn[16] =   { 0, 0, 0, 0,  18187720, 18187720, 18187720, 18187720,
																    24578, 24578, 24578, 24578,18187720, 18187720, 18187720, 18187720	};   // 0, q(w+1), w = w_4 = 1479


	v8int32   ra_p  = *((v8int32  * restrict)ra);
	v8int32   pa_p  = *((v8int32  * restrict)pa);

	v16int32  p12_p = *(v16int32 * restrict) pn;


	v4int32 m0, m1, m2, m3;
	v8int32 ma1, ma2, ma1_conv, ma2_conv;
	v8int32 ma1t, ma2t, ma1t_conv, ma2t_conv;
	v16int32 x1buff, x2buff, nullv, x3buff, temp_buff2, ma1_mod, ma2_mod;

	nullv = null_v16int32();
	unsigned int mask;

	////////////////////////////////////////////////////////////////////////////////////

	x1buff = upd_v(x1buff, 0, readincr_v4(s1_in));
	x1buff = upd_v(x1buff, 1, readincr_v4(s1_in));
	x1buff = upd_v(x1buff, 3, readincr_v4(s1_in));
	x1buff = upd_v(x1buff, 2, readincr_v4(s1_in));


	//aie::vector<int32, 16> v = x1buff;
	//aie::print(v, true ,"vin= ");

	m0 = srs(lmul4_sym     (x1buff, 0, 0x2310, 4, 12, zbuff, 0, 0x0000, 0) , 0);
	m1 = srs(lmul4_antisym (x1buff, 0, 0x2310, 4, 12, zbuff, 0, 0x0000, 1) , 0);
	m2 = srs(lmul4_sym     (x1buff, 0, 0x2310, 4, 12, zbuff, 0, 0x0000, 2)  ,0);
	m3 = srs(lmul4_antisym (x1buff, 0, 0x2310, 4, 12, zbuff, 0, 0x0000, 3)  ,0);

	x1buff = upd_v(x1buff, 0, m0);
	x1buff = upd_v(x1buff, 1, m1);
	x1buff = upd_v(x1buff, 2, m2);
	x1buff = upd_v(x1buff, 3, m3);

	mask  = lt16(x1buff, nullv);

	x2buff = add16(x1buff , p12_p);
	x3buff = select16(mask, x1buff, 0, 0x76543210, 0xFEDCBA98, x2buff , 0, 0x76543210, 0xFEDCBA98);  // neg to pos

	ma1 = ext_w(x3buff, 0);
	ma2 = ext_w(x3buff, 1);

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);

	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod


	ma1t = srs(mul(ext_w(ma1_mod,0), tsub1),0);
	ma2t = srs(mul(ext_w(ma1_mod,1), tsub2),0);  // tw multiply



	ma1t_conv = srs(mul(srs(mul(ext_w(ma1_mod,0), tsub1c),30),pa_p),0);
	ma2t_conv = srs(mul(srs(mul(ext_w(ma1_mod,1), tsub2c),30),pa_p),0);


	x1buff = sub16(concat(ma1t,ma2t), concat(ma1t_conv,ma2t_conv));


	// next stage starts


	temp_buff2 = shuffle16(x1buff,0,0xD951C840,0xFB73EA62);  // transpose

	m0 = srs(lmul4_sym     (temp_buff2, 0, 0x3210, 4, 12, zbuff, 0, 0x0000, 0) , 0) ;
	m1 = srs(lmul4_antisym (temp_buff2, 0, 0x3210, 4, 12, zbuff, 0, 0x0000, 1) , 0) ;

	m2 = srs(lmul4_sym     (temp_buff2, 0, 0x3210, 4, 12, zbuff, 0, 0x0000, 2)  ,0) ;
	m3 = srs(lmul4_antisym (temp_buff2, 0, 0x3210, 4, 12, zbuff, 0, 0x0000, 3)  ,0) ;

	x1buff = upd_v(x1buff, 0, m0);
	x1buff = upd_v(x1buff, 1, m1);
	x1buff = upd_v(x1buff, 2, m2);
	x1buff = upd_v(x1buff, 3, m3);

	mask  = lt16(x1buff, nullv);

	x2buff = add16(x1buff , p12_p);
	x3buff = select16(mask, x1buff, 0, 0x76543210, 0xFEDCBA98, x2buff , 0, 0x76543210, 0xFEDCBA98);  // neg to pos

	ma1 = ext_w(x3buff, 0);
	ma2 = ext_w(x3buff, 1);

	ma1_conv = srs(mul(srs(mul(ma1,ra_p),32),pa_p),0);
	ma2_conv = srs(mul(srs(mul(ma2,ra_p),32),pa_p),0);

	ma1_mod = sub16(concat(ma1,ma2), concat(ma1_conv,ma2_conv));  // mod

	put_wms(0, ext_v(ma1_mod,0));
	put_wms(0, ext_v(ma1_mod,1));
	put_wms(0, ext_v(ma1_mod,2));
	put_wms(0, ext_v(ma1_mod,3));







}

