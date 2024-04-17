#include <kernels.h>



extern int32 last_tw2[16];
extern int32 last_tw3[16];
extern int32 last_tw4[16];

extern int32 last_tw2c[16];
extern int32 last_tw3c[16];
extern int32 last_tw4c[16];

using namespace adf;



class test_ntt_16 : public adf::graph{

public:

	adf::port<input>  in;
    adf::port<output> out;
    adf::kernel kl;

    parameter buf1;

    test_ntt_16(){

    	kl = adf::kernel::create(ntt_16);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_16.cc" ;


    	adf::connect<stream>(in, kl.in[0]);
    	adf::connect<stream>(kl.out[0], out);


    };

};

template <int x, int y>
class total_graph : public adf::graph{

public:

	adf::port<input>  in;
    adf::port<output> out;

    test_ntt_16  g41;

    total_graph(){



    	adf::connect<stream> n1 (in, g41.in);
    	adf::connect<stream> n2 (g41.out, out);

    	location<kernel>(g41.kl)       =   tile(x+9, y);
    	location<stack> (g41.kl)       =   bank(x+9, y, 3);


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
       // location< fifo >(n1)  		  =   dma_fifo(aie_tile, x, y+1, 0x0000, 64);
        //location< fifo >(n2)  		  =   dma_fifo(aie_tile, x, y+1, 0x0400, 64);


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

    	location<kernel>(kl[5])       =   tile(x+4, y+1);
    	location<stack> (kl[5])       =   bank(x+4, y+1, 3);


    	location<kernel>(kl[6])       =   tile(x+4, y-1);
    	location<stack> (kl[6])       =   bank(x+4, y-1, 3);




    	fifo_depth(n1)=36;
    	fifo_depth(n2)=36;
    	fifo_depth(n3)=36;
    	fifo_depth(n4)=36;

    	fifo_depth(n5)=36;
    	fifo_depth(n6)=36;
    	fifo_depth(n7)=36;
    	fifo_depth(n8)=36;

    	fifo_depth(n9)=36;
    	fifo_depth(n10)=36;
    	fifo_depth(n11)=36;
    	fifo_depth(n12)=36;

    	fifo_depth(n13)=36;
    	fifo_depth(n14)=36;
    	fifo_depth(n15)=36;
    	fifo_depth(n16)=36;


    };

};

class four_k_graph : public adf::graph{

public:

	adf::port<input>  in[4] ;
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


