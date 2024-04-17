#include <kernels.h>

extern int32 stage0_tw[768];
extern int32 stage0_twc[768];

extern int32 stage1_tw[192];
extern int32 stage1_twc[192];

extern int32 stage2_tw[48];
extern int32 stage2_twc[48];



extern int32 ntt_0[1024];

extern int32 ntt_1[256];
extern int32 ntt_2[256];
extern int32 ntt_3[256];
extern int32 ntt_4[256];


extern int32 ntt_01[1024];
/*extern int32 ntt_02[256];
extern int32 ntt_03[256];
extern int32 ntt_04[256];
extern int32 ntt_05[64];
extern int32 ntt_06[64];
extern int32 ntt_07[64];
extern int32 ntt_08[64];
extern int32 ntt_09[64];
extern int32 ntt_10[64];
extern int32 ntt_11[64];
extern int32 ntt_12[64];
extern int32 ntt_13[64];
extern int32 ntt_14[64];
extern int32 ntt_15[64];
extern int32 ntt_16[64];*/

extern int32 ntt_4_1[1024];

extern int32 last_tw2[1024];
extern int32 last_tw3[1024];
extern int32 last_tw4[1024];

extern int32 last_tw2c[1024];
extern int32 last_tw3c[1024];
extern int32 last_tw4c[1024];




using namespace adf;

class test_grapha00 : public adf::graph{

public:

	adf::port<input>  in;
    adf::port<output> out[2];
    adf::kernel kl;

    test_grapha00(){

    	kl = adf::kernel::create(ntt_a00);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a00.cc" ;

    	adf::connect<window<1024*4>>(in, kl.in[0]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};


class test_grapha01 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    parameter buf1;
    parameter buf2;

    parameter buf3;



    test_grapha01(){

    	kl = adf::kernel::create(ntt_a01);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a01.cc" ;

    	buf1 = parameter::array(stage0_tw);
    	connect<>(buf1, kl);
    	buf2 = parameter::array(stage0_twc);
    	connect<>(buf2, kl);

    	buf3 = parameter::array(ntt_0);
    	connect<>(buf3, kl);


    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};

class test_grapha10 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    test_grapha10(){

    	kl = adf::kernel::create(ntt_a10);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a10.cc" ;

    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};


class test_grapha11 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    parameter buf1;
    parameter buf2;

    parameter buf3;
    parameter buf4;
    parameter buf5;
    parameter buf6;



    test_grapha11(){

    	kl = adf::kernel::create(ntt_a11);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a11.cc" ;

    	buf1 = parameter::array(stage1_tw);
    	connect<>(buf1, kl);
    	buf2 = parameter::array(stage1_twc);
    	connect<>(buf2, kl);

    	buf3 = parameter::array(ntt_1);
    	connect<>(buf3, kl);

    	buf4 = parameter::array(ntt_2);
    	connect<>(buf4, kl);

    	buf5 = parameter::array(ntt_3);
    	connect<>(buf5, kl);

    	buf6 = parameter::array(ntt_4);
    	connect<>(buf6, kl);



    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};





class test_grapha20 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    test_grapha20(){

    	kl = adf::kernel::create(ntt_a20);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a20.cc" ;

    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};




class test_grapha21 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    parameter buf1;
    parameter buf2;

    parameter a0, a1, a2, a3;
 /*   parameter b0, b1, b2, b3;
    parameter c0, c1, c2, c3;
    parameter d0, d1, d2, d3;*/



    test_grapha21(){

    	kl = adf::kernel::create(ntt_a21);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a21.cc" ;

    	buf1 = parameter::array(stage2_tw);
    	connect<>(buf1, kl);
    	buf2 = parameter::array(stage2_twc);
    	connect<>(buf2, kl);

    	a0 = parameter::array(ntt_01); //a1 = parameter::array(ntt_02); a2 = parameter::array(ntt_03); a3 = parameter::array(ntt_04);
    	connect<>(a0, kl); //connect<>(a1, kl); connect<>(a2, kl); connect<>(a3, kl);

    	/*b0 = parameter::array(ntt_05); b1 = parameter::array(ntt_06); b2 = parameter::array(ntt_07); b3 = parameter::array(ntt_08);
    	connect<>(b0, kl); connect<>(b1, kl); connect<>(b2, kl); connect<>(b3, kl);

    	c0 = parameter::array(ntt_09); c1 = parameter::array(ntt_10); c2 = parameter::array(ntt_11); c3 = parameter::array(ntt_12);
    	connect<>(c0, kl); connect<>(c1, kl); connect<>(c2, kl); connect<>(c3, kl);

    	d0 = parameter::array(ntt_13); d1 = parameter::array(ntt_14); d2 = parameter::array(ntt_15); d3 = parameter::array(ntt_16);
    	connect<>(d0, kl); connect<>(d1, kl); connect<>(d2, kl); connect<>(d3, kl);*/


    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};


class test_grapha30 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    test_grapha30(){

    	kl = adf::kernel::create(ntt_a30);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a30.cc" ;

    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};

class test_grapha31 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    test_grapha31(){

    	kl = adf::kernel::create(ntt_a31);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a31.cc" ;

    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};


class test_grapha40 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out[2];
    adf::kernel kl;

    test_grapha40(){

    	kl = adf::kernel::create(ntt_a40);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a40.cc" ;

    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out[0]);
    	adf::connect<stream>(kl.out[1], out[1]);

    };

};

class test_grapha41 : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out;
    adf::kernel kl;

    parameter buf1;

    test_grapha41(){

    	kl = adf::kernel::create(ntt_a41);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a41.cc" ;

    	buf1 = parameter::array(ntt_4_1);
    	connect<>(buf1, kl);

    	adf::connect<stream>(in[0], kl.in[0]);
    	adf::connect<stream>(in[1], kl.in[1]);
    	adf::connect<stream>(kl.out[0], out);


    };

};

template <int x, int y>
class total_graph : public adf::graph{

public:

	adf::port<input>  in;
    adf::port<output> out;
    test_grapha00  g00;
    test_grapha01  g01;

    test_grapha10  g10;
    test_grapha11  g11;

    test_grapha20  g20;
    test_grapha21  g21;

    test_grapha30  g30;
    test_grapha31  g31;

    test_grapha40  g40;
    test_grapha41  g41;

    total_graph(){



    	adf::connect<>          (in, g00.in);
    	adf::connect<stream> n1 (g00.out[0], g01.in[0]);
    	adf::connect<stream> n2 (g00.out[1], g01.in[1]);

    	adf::connect<stream> n3 (g01.out[0], g10.in[0]);
    	adf::connect<stream> n4 (g01.out[1], g10.in[1]);
    	adf::connect<stream> n5 (g10.out[0], g11.in[0]);
    	adf::connect<stream> n6 (g10.out[1], g11.in[1]);

    	adf::connect<stream> n7 (g11.out[0], g20.in[0]);
    	adf::connect<stream> n8 (g11.out[1], g20.in[1]);
    	adf::connect<stream> n9 (g20.out[0], g21.in[0]);
    	adf::connect<stream> n10 (g20.out[1], g21.in[1]);


    	adf::connect<stream> n11 (g21.out[0], g30.in[0]);
    	adf::connect<stream> n12 (g21.out[1], g30.in[1]);
    	adf::connect<stream> n13 (g30.out[0], g31.in[0]);
    	adf::connect<stream> n14 (g30.out[1], g31.in[1]);

    	adf::connect<stream> n15 (g31.out[0], g40.in[0]);
    	adf::connect<stream> n16 (g31.out[1], g40.in[1]);
    	adf::connect<stream> n17 (g40.out[0], g41.in[0]);
    	adf::connect<stream> n18 (g40.out[1], g41.in[1]);

    	adf::connect<stream> n19 (g41.out, out);

    	location<kernel>(g00.kl)       =   tile(x,   y);
    	location<stack> (g00.kl)       =   bank(x,   y, 3);
    	location<buffer>(g00.kl.in[0]) =  {bank(x,   y, 0),  bank(x,   y, 1)};


    	location<kernel>(g01.kl)       =   tile(x+1, y);
    	location<stack> (g01.kl)       =   bank(x+1, y, 3);
    	location<parameter>(g01.buf1)  =   bank(x+1, y, 2);
    	location<parameter>(g01.buf2)  =   bank(x+1, y, 2);
    	location<parameter>(g01.buf3)  =   bank(x+1, y, 1);
    	location< fifo >(n1)  		   =   dma_fifo(aie_tile, x+1, y, 0x0000, 200);
    	location< fifo >(n2)  		   =   dma_fifo(aie_tile, x+1, y, 0x0400, 200);

    	location<kernel>(g10.kl)       =   tile(x+2, y);
    	location<stack> (g10.kl)       =   bank(x+2, y, 3);
    	location< fifo >(n3)  		   =   dma_fifo(aie_tile, x+2, y, 0x0000, 200);
    	location< fifo >(n4)  		   =   dma_fifo(aie_tile, x+2, y, 0x0400, 200);

    	location<kernel>(g11.kl)       =   tile(x+3, y);
    	location<stack> (g11.kl)       =   bank(x+3, y, 3);
    	location<parameter>(g11.buf1)  =   bank(x+3, y, 2);
        location<parameter>(g11.buf2)  =   bank(x+3, y, 2);
        location<parameter>(g11.buf3)  =   bank(x+3, y, 2);
    	location<parameter>(g11.buf4)  =   bank(x+3, y, 1);
        location<parameter>(g11.buf5)  =   bank(x+3, y, 1);
        location<parameter>(g11.buf6)  =   bank(x+3, y, 1);
        location< fifo >(n5)  		   =   dma_fifo(aie_tile, x+3, y, 0x0000, 200);
        location< fifo >(n6)  		   =   dma_fifo(aie_tile, x+3, y, 0x0400, 200);


    	location<kernel>(g20.kl)       =   tile(x+4, y);
    	location<stack> (g20.kl)       =   bank(x+4, y, 3);
        location< fifo >(n7)  		   =   dma_fifo(aie_tile, x+4, y, 0x0000, 200);
        location< fifo >(n8)  		   =   dma_fifo(aie_tile, x+4, y, 0x0400, 200);

    	location<kernel>(g21.kl)       =   tile(x+5, y);
    	location<stack> (g21.kl)       =   bank(x+5, y, 3);
    	location<parameter>(g21.buf1)  =   bank(x+5, y, 2);
        location<parameter>(g21.buf2)  =   bank(x+5, y, 2);
        location<parameter>(g21.a0)    =   bank(x+5, y, 1);
        location< fifo >(n9)  		   =   dma_fifo(aie_tile, x+5, y, 0x0000, 200);
        location< fifo >(n10)  		   =   dma_fifo(aie_tile, x+5, y, 0x0400, 200);

    	location<kernel>(g30.kl)       =   tile(x+6, y);
    	location<stack> (g30.kl)       =   bank(x+6, y, 3);
        location< fifo >(n11)  		   =   dma_fifo(aie_tile, x+6, y, 0x0000, 200);
        location< fifo >(n12)  		   =   dma_fifo(aie_tile, x+6, y, 0x0400, 200);


    	location<kernel>(g31.kl)       =   tile(x+7, y);
    	location<stack> (g31.kl)       =   bank(x+7, y, 3);
        location< fifo >(n13)  		   =   dma_fifo(aie_tile, x+7, y, 0x0000, 200);
        location< fifo >(n14)  		   =   dma_fifo(aie_tile, x+7, y, 0x0400, 200);

    	location<kernel>(g40.kl)       =   tile(x+8, y);
    	location<stack> (g40.kl)       =   bank(x+8, y, 3);
        location< fifo >(n15)  		   =   dma_fifo(aie_tile, x+8, y, 0x0000, 200);
        location< fifo >(n16)  		   =   dma_fifo(aie_tile, x+8, y, 0x0400, 200);

    	location<kernel>(g41.kl)       =   tile(x+9, y);
    	location<stack> (g41.kl)       =   bank(x+9, y, 3);
    	location<parameter>(g41.buf1)  =   bank(x+9, y, 1);
        location< fifo >(n17)  		   =   dma_fifo(aie_tile, x+9, y, 0x0000, 200);
        location< fifo >(n18)  		   =   dma_fifo(aie_tile, x+9, y, 0x0400, 200);
        //location< fifo >(n19)  		   =   dma_fifo(aie_tile, x+9, y, 0x0400, 200); ///////////

    	fifo_depth(n1)=200;
    	fifo_depth(n2)=200;
    	fifo_depth(n3)=200;
    	fifo_depth(n4)=200;

    	fifo_depth(n5)=200;
    	fifo_depth(n6)=200;
    	fifo_depth(n7)=200;
    	fifo_depth(n8)=200;

    	fifo_depth(n9)=200;
    	fifo_depth(n10)=200;
    	fifo_depth(n11)=200;
    	fifo_depth(n12)=200;

    	fifo_depth(n13)=200;
    	fifo_depth(n14)=200;
    	fifo_depth(n15)=200;
    	fifo_depth(n16)=200;

    	fifo_depth(n17)=200;
    	fifo_depth(n18)=200;
    	//fifo_depth(n19)=200;



    };

};

template <int x, int y>
class last_graph : public adf::graph{

public:

	adf::port<input>  in[4];
    adf::port<output> out[4];
    adf::kernel kl[7];

    parameter buf1;
    parameter buf2;
    parameter buf3;

    parameter buf4;
    parameter buf5;
    parameter buf6;



    last_graph(){

    	kl[0] = adf::kernel::create(ntt_stream1);
    	adf::runtime<ratio>(kl[0]) = 0.9 ;
    	source(kl[0]) = "ntt_astream1.cc" ;

    	kl[1] = adf::kernel::create(ntt_stream2);
    	adf::runtime<ratio>(kl[1]) = 0.9 ;
    	source(kl[1]) = "ntt_astream1.cc" ;

    	kl[2] = adf::kernel::create(ntt_last0);
    	adf::runtime<ratio>(kl[2]) = 0.9 ;
    	source(kl[2]) = "ntt_last0.cc" ;

    	kl[3] = adf::kernel::create(ntt_last1);
    	adf::runtime<ratio>(kl[3]) = 0.9 ;
    	source(kl[3]) = "ntt_last1.cc" ;

    	kl[4] = adf::kernel::create(ntt_last2);
    	adf::runtime<ratio>(kl[4]) = 0.9 ;
    	source(kl[4]) = "ntt_last2.cc" ;

    	kl[5] = adf::kernel::create(ntt_llstream1);
    	adf::runtime<ratio>(kl[5]) = 0.9 ;
    	source(kl[5]) = "ntt_llstream.cc" ;

    	kl[6] = adf::kernel::create(ntt_llstream2);
    	adf::runtime<ratio>(kl[6]) = 0.9 ;
    	source(kl[6]) = "ntt_llstream.cc" ;


    	buf1 = parameter::array(last_tw2);
    	connect<>(buf1, kl[2]);
    	buf2 = parameter::array(last_tw2c);
    	connect<>(buf2, kl[2]);

    	buf3 = parameter::array(last_tw3);
    	connect<>(buf3, kl[2]);

    	buf4 = parameter::array(last_tw3c);
    	connect<>(buf4, kl[2]);

    	buf5 = parameter::array(last_tw4);
    	connect<>(buf5, kl[2]);

    	buf6 = parameter::array(last_tw4c);
    	connect<>(buf6, kl[2]);



    	adf::connect<stream> n1 (in[0], kl[0].in[0]);
    	adf::connect<stream> n2 (in[1], kl[0].in[1]);
    	adf::connect<stream> n3 (in[2], kl[1].in[0]);
    	adf::connect<stream> n4 (in[3], kl[1].in[1]);

    	adf::connect<stream> n5 (kl[0].out[0], kl[2].in[0]);
    	adf::connect<stream> n6 (kl[1].out[0], kl[2].in[1]);

    	adf::connect<stream> n7 (kl[2].out[0], kl[3].in[0]);
    	adf::connect<stream> n8 (kl[2].out[1], kl[3].in[1]);

    	adf::connect<stream> n9 (kl[3].out[0], kl[4].in[0]);
    	adf::connect<stream> n10 (kl[3].out[1], kl[4].in[1]);

    	adf::connect<stream> n11 (kl[4].out[0], kl[5].in[0]);
    	adf::connect<stream> n12 (kl[4].out[1], kl[6].in[0]);

    	adf::connect<stream> n13 (kl[5].out[0], out[0]);
    	adf::connect<stream> n14 (kl[5].out[1], out[1]);

    	adf::connect<stream> n15 (kl[6].out[0], out[2]);
    	adf::connect<stream> n16 (kl[6].out[1], out[3]);


    	location<kernel>(kl[0])       =   tile(x, y+1);
    	location<stack> (kl[0])       =   bank(x, y+1, 3);
        location< fifo >(n1)  		  =   dma_fifo(aie_tile, x, y+1, 0x0000, 200);
        location< fifo >(n2)  		  =   dma_fifo(aie_tile, x, y+1, 0x0400, 200);


    	location<kernel>(kl[1])       =   tile(x, y-1);
    	location<stack> (kl[1])       =   bank(x, y-1, 3);
        location< fifo >(n3)  		  =   dma_fifo(aie_tile, x, y-1, 0x0000, 200);
        location< fifo >(n4)  		  =   dma_fifo(aie_tile, x, y-1, 0x0400, 200);


    	location<kernel>(kl[2])       =   tile(x+1, y);
    	location<stack> (kl[2])       =   bank(x+1, y, 3);
    	location<parameter>(buf1)  	  =   bank(x+1, y+1, 0);
        location<parameter>(buf2)     =   bank(x+1, y+1, 1);
        location<parameter>(buf3)     =   bank(x+1, y+1, 2);
    	location<parameter>(buf4)     =   bank(x+1, y+1, 3);
        location<parameter>(buf5)     =   bank(x+1, y, 2);
        location<parameter>(buf6)     =   bank(x+1, y, 1);
        location< fifo >(n5)  		  =   dma_fifo(aie_tile, x+1, y, 0x0000, 200);
        location< fifo >(n6)  		  =   dma_fifo(aie_tile, x+1, y, 0x0400, 200);

    	location<kernel>(kl[3])       =   tile(x+2, y);
    	location<stack> (kl[3])       =   bank(x+2, y, 3);
        location< fifo >(n7)  		  =   dma_fifo(aie_tile, x+2, y, 0x0000, 200);
        location< fifo >(n8)  		  =   dma_fifo(aie_tile, x+2, y, 0x0400, 200);

    	location<kernel>(kl[4])       =   tile(x+3, y);
    	location<stack> (kl[4])       =   bank(x+3, y, 3);
        location< fifo >(n9)  		  =   dma_fifo(aie_tile, x+3, y, 0x0000, 200);
        location< fifo >(n10)  		  =   dma_fifo(aie_tile, x+3, y, 0x0400, 200);

    	location<kernel>(kl[5])       =   tile(x+4, y+1);
    	location<stack> (kl[5])       =   bank(x+4, y+1, 3);
        location< fifo >(n11)  		  =   dma_fifo(aie_tile, x+4, y+1, 0x0000, 200);

    	location<kernel>(kl[6])       =   tile(x+4, y-1);
    	location<stack> (kl[6])       =   bank(x+4, y-1, 3);
        location< fifo >(n12)  		  =   dma_fifo(aie_tile, x+4, y-1, 0x0000, 200);

        location< fifo >(n13)  		  =   dma_fifo(aie_tile, x+4, y-2, 0x0000, 200);
        location< fifo >(n14)  		  =   dma_fifo(aie_tile, x+4, y-2, 0x0400, 200);



    	fifo_depth(n1)=200;
    	fifo_depth(n2)=200;
    	fifo_depth(n3)=200;
    	fifo_depth(n4)=200;

    	fifo_depth(n5)=200;
    	fifo_depth(n6)=200;
    	fifo_depth(n7)=200;
    	fifo_depth(n8)=200;

    	fifo_depth(n9)=200;
    	fifo_depth(n10)=200;
    	fifo_depth(n11)=200;
    	fifo_depth(n12)=200;

    	fifo_depth(n13)=200;
    	fifo_depth(n14)=200;
    	fifo_depth(n15)=200;
    	fifo_depth(n16)=200;


    };

};

class four_k_graph : public adf::graph{

public:

	adf::port<input>  in[4];
    adf::port<output> out[4];

    total_graph<1,6>  ga;
    total_graph<1,5>  gb;
    total_graph<1,4>  gc;
    total_graph<1,3>  gd;

    last_graph<13,5>  gl;


    four_k_graph(){



    	adf::connect<> (in[0], ga.in);
    	adf::connect<> (in[1], gb.in);
    	adf::connect<> (in[2], gc.in);
    	adf::connect<> (in[3], gd.in);


    	adf::connect<> (ga.out, gl.in[0]);
    	adf::connect<> (gb.out, gl.in[1]);
    	adf::connect<> (gc.out, gl.in[2]);
    	adf::connect<> (gd.out, gl.in[3]);

    	adf::connect<> (gl.out[0], out[0]);
    	adf::connect<> (gl.out[1], out[1]);
    	adf::connect<> (gl.out[2], out[2]);
    	adf::connect<> (gl.out[3], out[3]);

  };




};


