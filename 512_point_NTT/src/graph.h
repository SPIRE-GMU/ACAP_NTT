#include <kernels.h>

extern int32 stage0_tw[192];
extern int32 stage0_twc[192];

extern int32 stage2_tw[48];
extern int32 stage2_twc[48];

extern int32 ntt_last[256];

//extern int32 ntt_0[64];



extern int32 ntt_01[64];


extern int32 ntt_4_1[64];

extern int32 last_tw2[64];
extern int32 last_tw3[64];
extern int32 last_tw4[64];

extern int32 last_tw2c[64];
extern int32 last_tw3c[64];
extern int32 last_tw4c[64];

extern int32 tw512[256];
extern int32 tw512c[256];




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

    	adf::connect<window<64*4>>(in, kl.in[0]);
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

    parameter a0;




    test_grapha21(){

    	kl = adf::kernel::create(ntt_a21);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_a21.cc" ;

    	buf1 = parameter::array(stage2_tw);
    	connect<>(buf1, kl);
    	buf2 = parameter::array(stage2_twc);
    	connect<>(buf2, kl);

    	a0 = parameter::array(ntt_01);
    	connect<>(a0, kl);


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

    test_grapha21  g21;

    test_grapha30  g30;
    test_grapha31  g31;

    test_grapha40  g40;
    test_grapha41  g41;

    total_graph(){



    	adf::connect<>          (in, g00.in);
    	adf::connect<stream> n9 (g00.out[0], g21.in[0]);
    	adf::connect<stream> n10 (g00.out[1], g21.in[1]);


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





    	location<kernel>(g21.kl)       =   tile(x+5, y);
    	location<stack> (g21.kl)       =   bank(x+5, y, 3);
    	location<parameter>(g21.buf1)  =   bank(x+5, y, 2);
        location<parameter>(g21.buf2)  =   bank(x+5, y, 2);
        location<parameter>(g21.a0)    =   bank(x+5, y, 1);
        location< fifo >(n9)  		   =   dma_fifo(aie_tile, x+5, y, 0x0000, 64);
        location< fifo >(n10)  		   =   dma_fifo(aie_tile, x+5, y, 0x0400, 64);

    	location<kernel>(g30.kl)       =   tile(x+6, y);
    	location<stack> (g30.kl)       =   bank(x+6, y, 3);
        location< fifo >(n11)  		   =   dma_fifo(aie_tile, x+6, y, 0x0000, 64);
        location< fifo >(n12)  		   =   dma_fifo(aie_tile, x+6, y, 0x0400, 64);


    	location<kernel>(g31.kl)       =   tile(x+7, y);
    	location<stack> (g31.kl)       =   bank(x+7, y, 3);
        location< fifo >(n13)  		   =   dma_fifo(aie_tile, x+7, y, 0x0000, 64);
        location< fifo >(n14)  		   =   dma_fifo(aie_tile, x+7, y, 0x0400, 64);

    	location<kernel>(g40.kl)       =   tile(x+8, y);
    	location<stack> (g40.kl)       =   bank(x+8, y, 3);
        location< fifo >(n15)  		   =   dma_fifo(aie_tile, x+8, y, 0x0000, 64);
        location< fifo >(n16)  		   =   dma_fifo(aie_tile, x+8, y, 0x0400, 64);

    	location<kernel>(g41.kl)       =   tile(x+9, y);
    	location<stack> (g41.kl)       =   bank(x+9, y, 3);
    	location<parameter>(g41.buf1)  =   bank(x+9, y, 1);
        location< fifo >(n17)  		   =   dma_fifo(aie_tile, x+9, y, 0x0000, 64);
        location< fifo >(n18)  		   =   dma_fifo(aie_tile, x+9, y, 0x0400, 64);



    	fifo_depth(n9)=64;
    	fifo_depth(n10)=64;
    	fifo_depth(n11)=64;
    	fifo_depth(n12)=64;

    	fifo_depth(n13)=64;
    	fifo_depth(n14)=64;
    	fifo_depth(n15)=64;
    	fifo_depth(n16)=64;

    	fifo_depth(n17)=64;
    	fifo_depth(n18)=64;




    };

};

template <int x, int y>
class last_graph : public adf::graph{

public:

	adf::port<input>  in[4];
    adf::port<output> out;
    adf::kernel kl[5];

    parameter buf1;
    parameter buf2;
    parameter buf3;

    parameter buf4;
    parameter buf5;
    parameter buf6;

    parameter buf7;



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

    	buf7 = parameter::array(ntt_last);
    	connect<>(buf6, kl[4]);



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

    	adf::connect<stream> n11 (kl[4].out[0], out);



    	location<kernel>(kl[0])       =   tile(x, y+1);
    	location<stack> (kl[0])       =   bank(x, y+1, 3);

    	location<kernel>(kl[1])       =   tile(x, y-1);
    	location<stack> (kl[1])       =   bank(x, y-1, 3);



    	location<kernel>(kl[2])       =   tile(x+1, y);
    	location<stack> (kl[2])       =   bank(x+1, y, 3);
    	location<parameter>(buf1)  	  =   bank(x+1, y+1, 0);
        location<parameter>(buf2)     =   bank(x+1, y+1, 1);
        location<parameter>(buf3)     =   bank(x+1, y+1, 2);
    	location<parameter>(buf4)     =   bank(x+1, y+1, 3);
        location<parameter>(buf5)     =   bank(x+1, y, 2);
        location<parameter>(buf6)     =   bank(x+1, y, 1);

    	location<kernel>(kl[3])       =   tile(x+2, y);
    	location<stack> (kl[3])       =   bank(x+2, y, 3);


    	location<kernel>(kl[4])       =   tile(x+3, y);
    	location<stack> (kl[4])       =   bank(x+3, y, 3);


    	fifo_depth(n1)=64;
    	fifo_depth(n2)=64;
    	fifo_depth(n3)=64;
    	fifo_depth(n4)=64;

    	fifo_depth(n5)=64;
    	fifo_depth(n6)=64;
    	fifo_depth(n7)=64;
    	fifo_depth(n8)=64;

    	fifo_depth(n9)=64;
    	fifo_depth(n10)=64;
    	fifo_depth(n11)=64;


    };

};

class acc_1_graph : public adf::graph{

public:

	adf::port<input>  in[8] ;
    adf::port<output> out[2];

    total_graph<1,7>  ga1;
    total_graph<1,6>  gb1;
    total_graph<1,5>  gc1;
    total_graph<1,4>  gd1;

    last_graph<13,6>  gl1;


    /////////////////////////////////////////


    total_graph<1,3>  ga2;
    total_graph<1,2>  gb2;
    total_graph<1,1>  gc2;
    total_graph<1,0>  gd2;

    last_graph<13,2>  gl2;

    adf::kernel kz;

    parameter buf1;
    parameter buf2;


    acc_1_graph(){


    	kz = adf::kernel::create(ntt_512_last);
    	adf::runtime<ratio>(kz) = 0.9 ;
    	source(kz) = "ntt_512_last.cc" ;

    	buf1 = parameter::array(tw512);
    	connect<>(buf1, kz);

    	buf2 = parameter::array(tw512c);
    	connect<>(buf2, kz);


    	adf::connect<> (in[0], ga1.in);
    	adf::connect<> (in[1], gb1.in);
    	adf::connect<> (in[2], gc1.in);
    	adf::connect<> (in[3], gd1.in);

    	adf::connect<> (in[4], ga2.in);
    	adf::connect<> (in[5], gb2.in);
    	adf::connect<> (in[6], gc2.in);
    	adf::connect<> (in[7], gd2.in);


    	adf::connect<> (ga1.out, gl1.in[0]);
    	adf::connect<> (gb1.out, gl1.in[1]);
    	adf::connect<> (gc1.out, gl1.in[2]);
    	adf::connect<> (gd1.out, gl1.in[3]);

    	adf::connect<> (ga2.out, gl2.in[0]);
    	adf::connect<> (gb2.out, gl2.in[1]);
    	adf::connect<> (gc2.out, gl2.in[2]);
    	adf::connect<> (gd2.out, gl2.in[3]);



    	adf::connect<stream> (gl1.out, kz.in[0]);
    	adf::connect<stream> (gl2.out, kz.in[1]);

    	adf::connect<stream> (kz.out[0], out[0]);
    	adf::connect<stream> (kz.out[1], out[1]);


    	location<kernel>(kz)       =   tile(18, 1);
    	location<stack> (kz)       =   bank(18, 1, 3);

  };




};


