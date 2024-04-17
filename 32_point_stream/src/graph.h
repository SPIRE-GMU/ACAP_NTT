
#include "kernels.h"

using namespace adf;


class test_graph : public adf::graph{

public:


	adf::port<input>  in;
    adf::port<output> out;
    adf::kernel kl;

    test_graph(){

    	kl = adf::kernel::create(ntt_16);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_16.cc" ;

    	adf::connect<stream>(in, kl.in[0]);
    	adf::connect<stream>(kl.out[0], out);

    };

};


class last_graph : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out;
    adf::kernel kl;


    last_graph(){

    	kl = adf::kernel::create(ntt_last);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_last.cc" ;




    	adf::connect<stream> n1 (in[0], kl.in[0]);
    	adf::connect<stream> n2 (in[1], kl.in[1]);

    	adf::connect<stream> n3 (kl.out[0], out);



    };

};



class total_graph : public adf::graph{

public:

	adf::port<input>  in[2];
    adf::port<output> out;

    test_graph ga;
    test_graph gb;
    last_graph gl;


    total_graph(){



    	adf::connect<> n1 (in[0], ga.in);
    	adf::connect<> n2 (in[1], gb.in);

    	adf::connect<> (ga.out, gl.in[0]);
    	adf::connect<> (gb.out, gl.in[1]);

    	adf::connect<> (gl.out, out);

    };

};
