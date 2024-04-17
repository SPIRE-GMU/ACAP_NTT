
#include "kernels.h"

using namespace adf;


class test_graph : public adf::graph{

public:


	adf::port<input>  gr_in;
    adf::port<output> gr_out;
    adf::kernel kl;

    test_graph(){

    	kl = adf::kernel::create(ntt_16);
    	adf::runtime<ratio>(kl) = 0.9 ;
    	source(kl) = "ntt_16.cc" ;

    	adf::connect<stream>(gr_in, kl.in[0]);
    	adf::connect<stream>(kl.out[0], gr_out);

    };

};
