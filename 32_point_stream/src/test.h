#include "graph.h"

class TEST: public graph {

public:

	input_plio  plin[2];
	output_plio plout;

	//graph object instant
	total_graph g;



    TEST(){

		plin[0] = input_plio::create("in00a",  plio_32_bits, "data/in00a.txt", 1250);
		plin[1] = input_plio::create("in00b",  plio_32_bits, "data/in00b.txt", 1250);

		plout   = output_plio::create("out00a",  plio_32_bits, "data/out00a.txt", 1250);



		connect<>(plin[0].out[0], g.in[0]);
		connect<>(plin[1].out[0], g.in[1]);


		connect<>(g.out, plout.in[0]);



		}

};
