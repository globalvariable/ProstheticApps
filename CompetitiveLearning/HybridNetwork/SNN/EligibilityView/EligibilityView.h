#ifndef ELIGIBILITY_VIEW_H
#define ELIGIBILITY_VIEW_H

#define NUM_OF_ELIGIBILITY_GRAPHS	2

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../BufferViewHandler/BufferViewHandler.h"
#include "../../../../../IzNeuronSimulators/Library/Graphs/EligibilityGraph.h"
#include "../../../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo.h"

bool create_eligibility_view_gui(void);
unsigned int get_num_eligibility_graphs_w_scroll(void);
EligibilityGraphScrollLimited** get_eligibility_graphs_w_scroll_ptr(void);


#endif
