#include "HybridNetwork.h"



int main( int argc, char *argv[])
{
	cpu_set_t  mask;
	CPU_ZERO(&mask);
	CPU_SET(HYBRID_NETWORK_USER_SPACE_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU+HYBRID_NETWORK_USER_SPACE_CPU_THREAD_ID, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask))  	// if pid = 0 (first argument), calling process is handled to set affinity.
		return print_message(ERROR_MSG ,"HybridNetwork", "HybridNetwork", "main","! sched_setaffinity(0, sizeof(mask), &mask).");		

	gtk_init(&argc, &argv);

	if (! initialize_snn_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "HybridNetwork", "main","! initialize_snn_data().");		

	if (! start_periodic_task()) // to evaluate system clock
		return print_message(ERROR_MSG ,"HybridNetwork", "HybridNetwork", "main","! start_periodic_task().");		

	create_gui();
	gtk_main();
	return 0;
}

