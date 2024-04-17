#include "graph.h"

class TEST: public graph {

public:

	input_plio  plin;
	output_plio plout;

	//graph object instant
	test_graph g;



    TEST(){

		plin = input_plio::create("in00a",  plio_32_bits, "data/in00a.txt", 1250);

		plout = output_plio::create("out00a",  plio_32_bits, "data/out00a.txt", 1250);



		connect<>(plin.out[0], g.gr_in);


		connect<>(g.gr_out, plout.in[0]);



		}

};
