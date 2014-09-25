#ifndef RT_TASKS_CONFIG_H
#define RT_TASKS_CONFIG_H

// System Time
#define SYSTIME_PERIODIC_PERIOD 							2000000		//2 ms
#define SYSTIME_PERIODIC_TASK_NAME						nam2num("SYSTIM")
#define SYSTIME_PERIODIC_TASK_PRIORITY 					1
#define SYSTIME_PERIODIC_STACK_SIZE 						10000
#define SYSTIME_PERIODIC_MSG_SIZE 							1000
#define SYSTIME_PERIODIC_CPU_ID 							0
#define SYSTIME_PERIODIC_CPU_THREAD_ID					0
#define SYSTIME_PERIODIC_CPU_THREAD_TASK_ID				0	
#define SYSTIME_PERIODIC_SIGNAL 							0
#define SYSTIME_PERIODIC_POLICY 							SCHED_FIFO
#define SYSTIME_PERIODIC_POSITIVE_JITTER_THRES	 		SYSTIME_PERIODIC_PERIOD / 10
#define SYSTIME_PERIODIC_NEGATIVE_JITTER_THRES	 		SYSTIME_PERIODIC_PERIOD / 10
#define SYSTIME_PERIODIC_RUN_TIME_THRES					50000

// TrialHandler
#define TRIAL_HANDLER_PERIOD 							SYSTIME_PERIODIC_PERIOD		//2 ms
#define TRIAL_HANDLER_TASK_NAME						nam2num("TRIALHND")
#define TRIAL_HANDLER_TASK_PRIORITY					300
#define TRIAL_HANDLER_STACK_SIZE 						10000
#define TRIAL_HANDLER_MSG_SIZE 						1000
#define TRIAL_HANDLER_POLICY 							SCHED_FIFO
#define TRIAL_HANDLER_CPU_ID							SYSTIME_PERIODIC_CPU_ID
#define TRIAL_HANDLER_CPU_THREAD_ID					SYSTIME_PERIODIC_CPU_THREAD_ID
#define TRIAL_HANDLER_CPU_THREAD_TASK_ID			SYSTIME_PERIODIC_CPU_THREAD_TASK_ID+1
#define TRIAL_HANDLER_POSITIVE_JITTER_THRES	 		SYSTIME_PERIODIC_POSITIVE_JITTER_THRES
#define TRIAL_HANDLER_NEGATIVE_JITTER_THRES	 		SYSTIME_PERIODIC_POSITIVE_JITTER_THRES
#define TRIAL_HANDLER_RUN_TIME_THRES					50000

// TrialDurationHandler
#define TRIAL_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_DURATION_HANDLER_TASK_NAME					nam2num("TRIALDUR")
#define TRIAL_DURATION_HANDLER_TASK_PRIORITY				301
#define TRIAL_DURATION_HANDLER_STACK_SIZE 					10000
#define TRIAL_DURATION_HANDLER_MSG_SIZE 						1000
#define TRIAL_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define TRIAL_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define TRIAL_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID			TRIAL_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES
#define TRIAL_DURATION_HANDLER_RUN_TIME_THRES				50000

// ExpEnviHandler
#define EXP_ENVI_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define EXP_ENVI_HANDLER_TASK_NAME					nam2num("ENVIHND")
#define EXP_ENVI_HANDLER_TASK_PRIORITY				302
#define EXP_ENVI_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_HANDLER_MSG_SIZE 					1000
#define EXP_ENVI_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define EXP_ENVI_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID		TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define EXP_ENVI_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define EXP_ENVI_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES
#define EXP_ENVI_HANDLER_RUN_TIME_THRES				50000

// ExpEnviDurationHandler
#define EXP_ENVI_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define EXP_ENVI_DURATION_HANDLER_TASK_NAME					nam2num("ENVIDHND")
#define EXP_ENVI_DURATION_HANDLER_TASK_PRIORITY				303
#define EXP_ENVI_DURATION_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_DURATION_HANDLER_MSG_SIZE 						1000
#define EXP_ENVI_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID			EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID + 1 
#define EXP_ENVI_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define EXP_ENVI_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES
#define EXP_ENVI_DURATION_HANDLER_RUN_TIME_THRES				50000

// MovObjHandler
#define MOV_OBJ_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_HANDLER_TASK_NAME					nam2num("MOBJHND")
#define MOV_OBJ_HANDLER_TASK_PRIORITY				304
#define MOV_OBJ_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_HANDLER_MSG_SIZE 					1000
#define MOV_OBJ_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID			EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES
#define MOV_OBJ_HANDLER_RUN_TIME_THRES				50000

// MovObjDurationHandler
#define MOV_OBJ_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_DURATION_HANDLER_TASK_NAME					nam2num("MOBDHND")
#define MOV_OBJ_DURATION_HANDLER_TASK_PRIORITY					305
#define MOV_OBJ_DURATION_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_DURATION_HANDLER_MSG_SIZE 						1000
#define MOV_OBJ_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define MOV_OBJ_DURATION_HANDLER_CPU_THREAD_TASK_ID			MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES
#define MOV_OBJ_DURATION_HANDLER_RUN_TIME_THRES	 			50000

// Trial Handler 2 Neural Network Messages Handler
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD 					TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME				nam2num("TRTONN")
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY			101
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE 				10000
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE		 		1000
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY			 		SCHED_FIFO
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID					TRIAL_HANDLER_CPU_ID
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID			TRIAL_HANDLER_CPU_THREAD_ID
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID	MOV_OBJ_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_RUN_TIME_THRES	 	50000

// BlueSpike
#define BLUESPIKE_DAQ_PERIOD 							SAMPLING_INTERVAL*NUM_OF_SCAN		//1 ms
#define BLUESPIKE_DAQ_TASK_NAME						nam2num("BLUDAQ")
#define BLUESPIKE_DAQ_TASK_PRIORITY 					0	// highest priority
#define BLUESPIKE_DAQ_STACK_SIZE 						10000
#define BLUESPIKE_DAQ_MSG_SIZE 						1000
#define BLUESPIKE_DAQ_CPU_ID 							1
#define BLUESPIKE_DAQ_CPU_THREAD_ID					0
#define BLUESPIKE_DAQ_CPU_THREAD_TASK_ID			0
#define BLUESPIKE_DAQ_SIGNAL 							0
#define BLUESPIKE_DAQ_POLICY 							SCHED_FIFO
#define BLUESPIKE_DAQ_POSITIVE_JITTER_THRES	 		SYSTIME_PERIODIC_PERIOD / 10
#define BLUESPIKE_DAQ_NEGATIVE_JITTER_THRES	 		SYSTIME_PERIODIC_PERIOD / 10
#define BLUESPIKE_DAQ_RUN_TIME_THRES	 			200000

// InSilicoSimulators
#define SNN_SIM_PERIOD						2000000		//1 ms
#define SNN_SIM_TASK_NAME					"SNN"
#define SNN_SIM_TASK_PRIORITY				202
#define SNN_SIM_STACK_SIZE 					10000
#define SNN_SIM_MSG_SIZE 					1000
#define SNN_SIM_POLICY 						SCHED_FIFO
#define SNN_SIM_CPU_ID						2
#define SNN_SIM_CPU_THREAD_ID				0
#define SNN_SIM_POSITIVE_JITTER_THRES	 	200000		// 200 us
#define SNN_SIM_NEGATIVE_JITTER_THRES	200000
#define SNN_SIM_RUN_TIME_THRES	 		50000

#define SNN_SIM_NUM_OF_DEDICATED_CPUS	2	// cpus startiing from SNN_SIM_CPU_ID are dedicated to SNN simulations


#define HYBRID_NETWORK_USER_SPACE_CPU_ID				0
#define HYBRID_NETWORK_USER_SPACE_CPU_THREAD_ID		0

#define SNN_SIM_USER_SPACE_CPU_ID			0
#define SNN_SIM_USER_SPACE_CPU_THREAD_ID	0

#define NUM_OF_TASK_EXECUTIONS_4_PERFOMANCE_EVAL	3000






#endif
