prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm -lmeschach
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: HybridNetwork

HybridNetwork: HybridNetwork.c Gui.o ../../../BlueSpike/Library/Misc/Misc.o DataAcquisition/ConfigDaq/ConfigDaqGui.o DataAcquisition/ConfigDaq/FileHandler.o DataAcquisition/ConfigDaq/FileHandler_v0.o DataAcquisition/ConfigDaq/DaqMwaMap.o DataAcquisition/DaqRtTask.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o DataAcquisition/ConfigDaq/DaqCard.o HybridNetworkRtTask.o DataAcquisition/RecordingData.o DataAcquisition/ButterFilter.o DataAcquisition/CubicInterpolation.o DataAcquisition/SpikeSorting.o DataAcquisition/TemplateMatching.o DataAcquisition/SpikeViewer/SpikeViewerGui.o DataAcquisition/SpikeViewer/FileHandler.o DataAcquisition/SpikeViewer/FileHandler_v0.o DataAcquisition/SpikeViewer/DataHandling/DataHandling.o DataAcquisition/SpikeViewer/DataHandling/v0/DataFormat_v0.o DataAcquisition/SpikeSorter/SpikeSorterGui.o DataAcquisition/SpikeSorter/FileHandler.o DataAcquisition/SpikeSorter/FileHandler_v0.o SNN/NetworkView/NetworkView.o SNN/SnnData.o SNN/SnnRtTask.o ../../../IzNeuronSimulators/Library/Neuron/NeuronTypes.o ../../../IzNeuronSimulators/Library/Neuron/Neuron.o ../../../IzNeuronSimulators/Library/Network/Network.o ../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNeuronCombo.o ../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo.o ../../../IzNeuronSimulators/Library/Combos/NeuronDynamicsCombo.o ../../../IzNeuronSimulators/Library/NeuronDynamicsData/NeuronDynamicsData.o ../../../IzNeuronSimulators/Library/STDPData/STDPData.o ../../../IzNeuronSimulators/Library/EligibilityData/EligibilityData.o ../../../IzNeuronSimulators/Library/SpikeData/SpikeData.o SNN/BufferViewHandler/BufferViewHandler.o SNN/SnnFunctions/SnnFunctions.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.o ../../../IzNeuronSimulators/Library/Synapse/Synapse.o ../../../IzNeuronSimulators/Library/Eligibility/Eligibility.o ../../../IzNeuronSimulators/Library/STDP/STDP.o ../../../IzNeuronSimulators/Library/ParkerSochacki/ParkerSochacki.o ../../../IzNeuronSimulators/Library/ParkerSochacki/PSPrePostResetSTDPElig.o SNN/NeuronDynamicsView/NeuronDynamicsView.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../../IzNeuronSimulators/Library/Axon/Axon.o  ../../../IzNeuronSimulators/Library/Event/Event.o SNN/StdpView/StdpView.o SNN/EligibilityView/EligibilityView.o SNN/BlueSpikeSpikePatternView/BlueSpikeSpikePatternView.o SNN/InSilicoSpikePatternView/InSilicoSpikePatternView.o SNN/WeightHistoryView/WeightHistoryView.o SNN/AxonToLayerWeightView/AxonToLayerWeightView.o ../../../IzNeuronSimulators/Library/Graphs/SpikePatternGraph.o ../../../IzNeuronSimulators/Library/Graphs/NeuronDynamicsGraph.o ../../../IzNeuronSimulators/Library/Graphs/STDPGraph.o ../../../IzNeuronSimulators/Library/Graphs/EligibilityGraph.o ../../../BlueSpike/Library/Misc/Randn.o ../../../IzNeuronSimulators/Library/Combos/STDPCombo.o ../../../IzNeuronSimulators/Library/Graphs/SynapticWeightHistGraph.o ../../../IzNeuronSimulators/Library/Graphs/StatusMarker.o DataAcquisition/BlueSpikeNodes.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< Gui.o ../../../BlueSpike/Library/Misc/Misc.o DataAcquisition/ConfigDaq/ConfigDaqGui.o DataAcquisition/ConfigDaq/FileHandler.o DataAcquisition/ConfigDaq/FileHandler_v0.o DataAcquisition/ConfigDaq/DaqMwaMap.o DataAcquisition/DaqRtTask.o	../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o DataAcquisition/ConfigDaq/DaqCard.o  HybridNetworkRtTask.o DataAcquisition/RecordingData.o DataAcquisition/ButterFilter.o DataAcquisition/CubicInterpolation.o  DataAcquisition/SpikeSorting.o DataAcquisition/TemplateMatching.o DataAcquisition/SpikeViewer/SpikeViewerGui.o DataAcquisition/SpikeViewer/FileHandler.o DataAcquisition/SpikeViewer/FileHandler_v0.o DataAcquisition/SpikeViewer/DataHandling/DataHandling.o DataAcquisition/SpikeViewer/DataHandling/v0/DataFormat_v0.o DataAcquisition/SpikeSorter/SpikeSorterGui.o DataAcquisition/SpikeSorter/FileHandler.o DataAcquisition/SpikeSorter/FileHandler_v0.o SNN/NetworkView/NetworkView.o SNN/SnnData.o SNN/SnnRtTask.o ../../../IzNeuronSimulators/Library/Neuron/NeuronTypes.o ../../../IzNeuronSimulators/Library/Neuron/Neuron.o ../../../IzNeuronSimulators/Library/Network/Network.o ../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNeuronCombo.o ../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo.o  ../../../IzNeuronSimulators/Library/Combos/NeuronDynamicsCombo.o ../../../IzNeuronSimulators/Library/NeuronDynamicsData/NeuronDynamicsData.o ../../../IzNeuronSimulators/Library/STDPData/STDPData.o ../../../IzNeuronSimulators/Library/EligibilityData/EligibilityData.o ../../../IzNeuronSimulators/Library/SpikeData/SpikeData.o SNN/BufferViewHandler/BufferViewHandler.o SNN/SnnFunctions/SnnFunctions.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.o ../../../IzNeuronSimulators/Library/Synapse/Synapse.o ../../../IzNeuronSimulators/Library/Eligibility/Eligibility.o ../../../IzNeuronSimulators/Library/STDP/STDP.o ../../../IzNeuronSimulators/Library/ParkerSochacki/ParkerSochacki.o ../../../IzNeuronSimulators/Library/ParkerSochacki/PSPrePostResetSTDPElig.o SNN/NeuronDynamicsView/NeuronDynamicsView.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../../IzNeuronSimulators/Library/Axon/Axon.o ../../../IzNeuronSimulators/Library/Event/Event.o SNN/StdpView/StdpView.o SNN/EligibilityView/EligibilityView.o SNN/BlueSpikeSpikePatternView/BlueSpikeSpikePatternView.o SNN/InSilicoSpikePatternView/InSilicoSpikePatternView.o SNN/WeightHistoryView/WeightHistoryView.o SNN/AxonToLayerWeightView/AxonToLayerWeightView.o  ../../../IzNeuronSimulators/Library/Graphs/SpikePatternGraph.o ../../../IzNeuronSimulators/Library/Graphs/NeuronDynamicsGraph.o  ../../../IzNeuronSimulators/Library/Graphs/STDPGraph.o ../../../IzNeuronSimulators/Library/Graphs/EligibilityGraph.o ../../../BlueSpike/Library/Misc/Randn.o ../../../IzNeuronSimulators/Library/Combos/STDPCombo.o ../../../IzNeuronSimulators/Library/Graphs/SynapticWeightHistGraph.o ../../../IzNeuronSimulators/Library/Graphs/StatusMarker.o DataAcquisition/BlueSpikeNodes.o
### HybridNetwork

Gui.o: Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

HybridNetworkRtTask.o: HybridNetworkRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HybridNetworkRtTask


#################  DataAcquisition  ###########################

DataAcquisition/DaqRtTask.o: DataAcquisition/DaqRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/DaqRtTask

DataAcquisition/RecordingData.o: DataAcquisition/RecordingData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/RecordingData

DataAcquisition/ButterFilter.o: DataAcquisition/ButterFilter.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/ButterFilter

DataAcquisition/CubicInterpolation.o: DataAcquisition/CubicInterpolation.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/CubicInterpolation

DataAcquisition/SpikeSorting.o: DataAcquisition/SpikeSorting.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/SpikeSorting

DataAcquisition/TemplateMatching.o: DataAcquisition/TemplateMatching.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/TemplateMatching

DataAcquisition/BlueSpikeNodes.o: DataAcquisition/BlueSpikeNodes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/BlueSpikeNodes

#################  DataAcquisition/ConfigDaq  ###########################

DataAcquisition/ConfigDaq/ConfigDaqGui.o: DataAcquisition/ConfigDaq/ConfigDaqGui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ConfigDaq/ConfigDaqGui

DataAcquisition/ConfigDaq/FileHandler.o: DataAcquisition/ConfigDaq/FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ConfigDaq/FileHandler

DataAcquisition/ConfigDaq/FileHandler_v0.o: DataAcquisition/ConfigDaq/FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ConfigDaq/FileHandler_v0

DataAcquisition/ConfigDaq/DaqMwaMap.o: DataAcquisition/ConfigDaq/DaqMwaMap.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ConfigDaq/DaqMwaMap

DataAcquisition/ConfigDaq/DaqCard.o: DataAcquisition/ConfigDaq/DaqCard.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ConfigDaq/DaqCard

#################  DataAcquisition/SpikeViewer  ###########################

DataAcquisition/SpikeViewer/SpikeViewerGui.o: DataAcquisition/SpikeViewer/SpikeViewerGui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/SpikeViewer/SpikeViewerGui

DataAcquisition/SpikeViewer/FileHandler.o: DataAcquisition/SpikeViewer/FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### DataAcquisition/SpikeViewer/FileHandler

DataAcquisition/SpikeViewer/FileHandler_v0.o: DataAcquisition/SpikeViewer/FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### DataAcquisition/SpikeViewer/FileHandler_v0

DataAcquisition/SpikeViewer/DataHandling/DataHandling.o: DataAcquisition/SpikeViewer/DataHandling/DataHandling.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### DataAcquisition/SpikeViewer/DataHandling/DataHandling.o

DataAcquisition/SpikeViewer/DataHandling/v0/DataFormat_v0.o: DataAcquisition/SpikeViewer/DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### DataAcquisition/SpikeViewer/DataHandling/v0/DataFormat_v0.o

#################  DataAcquisition/SpikeSorter  ############################

DataAcquisition/SpikeSorter/SpikeSorterGui.o: DataAcquisition/SpikeSorter/SpikeSorterGui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/SpikeSorter/SpikeSorterGui

DataAcquisition/SpikeSorter/FileHandler.o: DataAcquisition/SpikeSorter/FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/SpikeSorter/FileHandler

DataAcquisition/SpikeSorter/FileHandler_v0.o: DataAcquisition/SpikeSorter/FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataAcquisition/SpikeSorter/FileHandler_v0


#################  SNN  ###################################

SNN/SnnData.o: SNN/SnnData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SNN/SnnData

SNN/SnnRtTask.o: SNN/SnnRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SNN/SnnRtTask

SNN/NetworkView/NetworkView.o: SNN/NetworkView/NetworkView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SNN/NetworkView/NetworkView

SNN/NeuronDynamicsView/NeuronDynamicsView.o: SNN/NeuronDynamicsView/NeuronDynamicsView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SNN/NeuronDynamicsView/NeuronDynamicsView

SNN/StdpView/StdpView.o: SNN/StdpView/StdpView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### StdpView

SNN/EligibilityView/EligibilityView.o: SNN/EligibilityView/EligibilityView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### EligibilityView

SNN/BlueSpikeSpikePatternView/BlueSpikeSpikePatternView.o: SNN/BlueSpikeSpikePatternView/BlueSpikeSpikePatternView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BlueSpikeSpikePatternView

SNN/InSilicoSpikePatternView/InSilicoSpikePatternView.o: SNN/InSilicoSpikePatternView/InSilicoSpikePatternView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BlueSpikeSpikePatternView

SNN/WeightHistoryView/WeightHistoryView.o: SNN/WeightHistoryView/WeightHistoryView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### WeightHistoryView

SNN/AxonToLayerWeightView/AxonToLayerWeightView.o: SNN/AxonToLayerWeightView/AxonToLayerWeightView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### AxonToLayerWeightView

SNN/BufferViewHandler/BufferViewHandler.o: SNN/BufferViewHandler/BufferViewHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SNN/BufferViewHandler/BufferViewHandler

SNN/SnnFunctions/SnnFunctions.o: SNN/SnnFunctions/SnnFunctions.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SNN/SnnFunctions/SnnFunctions

##########################################################

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../../BlueSpikeLibrary/Misc/Misc

../../../BlueSpike/Library/Misc/Randn.o: ../../../BlueSpike/Library/Misc/Randn.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../../BlueSpikeLibrary/Misc/Randn

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c	
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o

../../../IzNeuronSimulators/Library/Neuron/NeuronTypes.o: ../../../IzNeuronSimulators/Library/Neuron/NeuronTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Neuron/NeuronTypes

../../../IzNeuronSimulators/Library/Neuron/Neuron.o: ../../../IzNeuronSimulators/Library/Neuron/Neuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Neuron/Neuron

../../../IzNeuronSimulators/Library/Network/Network.o: ../../../IzNeuronSimulators/Library/Network/Network.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Network/Network

../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNeuronCombo.o: ../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNeuronCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNeuronCombo

../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo.o: ../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo

../../../IzNeuronSimulators/Library/Combos/NeuronDynamicsCombo.o: ../../../IzNeuronSimulators/Library/Combos/NeuronDynamicsCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../IzNeuronSimulators/Library/Combos/NeuronDynamicsCombo

../../../IzNeuronSimulators/Library/NeuronDynamicsData/NeuronDynamicsData.o: ../../../IzNeuronSimulators/Library/NeuronDynamicsData/NeuronDynamicsData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/NeuronDynamicsData/NeuronDynamicsData

../../../IzNeuronSimulators/Library/STDPData/STDPData.o: ../../../IzNeuronSimulators/Library/STDPData/STDPData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/STDPData/STDPData

../../../IzNeuronSimulators/Library/EligibilityData/EligibilityData.o: ../../../IzNeuronSimulators/Library/EligibilityData/EligibilityData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/EligibilityData/EligibilityData

../../../IzNeuronSimulators/Library/SpikeData/SpikeData.o: ../../../IzNeuronSimulators/Library/SpikeData/SpikeData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../IzNeuronSimulators/Library/SpikeData/SpikeData

../../../IzNeuronSimulators/Library/Synapse/Synapse.o: ../../../IzNeuronSimulators/Library/Synapse/Synapse.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			###../../../IzNeuronSimulators/Library/Synapse/Synapse

../../../IzNeuronSimulators/Library/Eligibility/Eligibility.o: ../../../IzNeuronSimulators/Library/Eligibility/Eligibility.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Eligibility/Eligibility

../../../IzNeuronSimulators/Library/STDP/STDP.o: ../../../IzNeuronSimulators/Library/STDP/STDP.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/STDP/STDP

../../../IzNeuronSimulators/Library/ParkerSochacki/ParkerSochacki.o: ../../../IzNeuronSimulators/Library/ParkerSochacki/ParkerSochacki.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/ParkerSochacki/ParkerSochacki

../../../IzNeuronSimulators/Library/ParkerSochacki/PSPrePostResetSTDPElig.o: ../../../IzNeuronSimulators/Library/ParkerSochacki/PSPrePostResetSTDPElig.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/ParkerSochacki/PSPrePostResetSTDPElig

../../../ExperimentHandlers/Library/Status/TrialStatus.o: ../../../ExperimentHandlers/Library/Status/TrialStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/TrialStatus

../../../IzNeuronSimulators/Library/Axon/Axon.o: ../../../IzNeuronSimulators/Library/Axon/Axon.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Axon/Axon

../../../IzNeuronSimulators/Library/Event/Event.o: ../../../IzNeuronSimulators/Library/Event/Event.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Event/Event

../../../IzNeuronSimulators/Library/Graphs/SpikePatternGraph.o: ../../../IzNeuronSimulators/Library/Graphs/SpikePatternGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)  		###	../../../IzNeuronSimulators/Library/Graphs/SpikePatternGraph

../../../IzNeuronSimulators/Library/Graphs/NeuronDynamicsGraph.o: ../../../IzNeuronSimulators/Library/Graphs/NeuronDynamicsGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Graphs/NeuronDynamicsGraph

../../../IzNeuronSimulators/Library/Graphs/STDPGraph.o: ../../../IzNeuronSimulators/Library/Graphs/STDPGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Graphs/STDPGraph

../../../IzNeuronSimulators/Library/Graphs/EligibilityGraph.o: ../../../IzNeuronSimulators/Library/Graphs/EligibilityGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Graphs/EligibilityGraph

../../../IzNeuronSimulators/Library/Combos/STDPCombo.o: ../../../IzNeuronSimulators/Library/Combos/STDPCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Combos/STDPCombo

../../../IzNeuronSimulators/Library/Graphs/SynapticWeightHistGraph.o: ../../../IzNeuronSimulators/Library/Graphs/SynapticWeightHistGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Graphs/SynapticWeightHistGraph

../../../IzNeuronSimulators/Library/Graphs/StatusMarker.o: ../../../IzNeuronSimulators/Library/Graphs/StatusMarker.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../IzNeuronSimulators/Library/Graphs/StatusMarker

##################### Messages #######################

../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui

../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet

../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet

clean:
	rm -f HybridNetwork *.o DataAcquisition/*.o DataAcquisition/ConfigDaq/*.o DataAcquisition/SpikeViewer/*.o  DataAcquisition/SpikeSorter/*.o DataAcquisition/NeuRecHandler/*.o SNN/*.o SNN/AxonToLayerWeightView/*.o SNN/BlueSpikeSpikePatternView/*.o SNN/BufferViewHandler/*.o SNN/DataHandling/*.o SNN/DataHandling/v0/*.o SNN/EligibilityView/*.o SNN/InSilicoSpikePatternView/*.o SNN/NetworkView/*.o  SNN/NeuronDynamicsView/*.o  SNN/SnnFunctions/*.o  SNN/StdpView/*.o  SNN/WeightHistoryView/*.o       ../../../BlueSpike/Library/Misc/*.o  ../../../BlueSpike/Library/Misc/*.o  DataAcquisition/*.o ../../../IzNeuronSimulators/Library/Neuron/*.o ../../../IzNeuronSimulators/Library/Network/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../../IzNeuronSimulators/Library/Combos/*.o ../../../IzNeuronSimulators/Library/NeuronDynamicsData/*.o ../../../IzNeuronSimulators/Library/STDPData/*.o ../../../IzNeuronSimulators/Library/EligibilityData/*.o ../../../IzNeuronSimulators/Library/SpikeData/SpikeData.o SNN/BufferViewHandler/*.o SNN/SnnFunctions/*.o ../../../ExperimentHandlers/Library/Messages/*.o ../../../IzNeuronSimulators/Library/Synapse/*.o ../../../IzNeuronSimulators/Library/Eligibility/*.o ../../../IzNeuronSimulators/Library/STDP/*.o ../../../IzNeuronSimulators/Library/ParkerSochacki/*.o SNN/NeuronDynamicsView/*.o ../../../ExperimentHandlers/Library/Status/*.o ../../../IzNeuronSimulators/Library/Axon/*.o ../../../IzNeuronSimulators/Library/Event/*.o SNN/StdpView/*.o SNN/EligibilityView/*.o SNN/BlueSpikeSpikePatternView/*.o SNN/InSilicoSpikePatternView/*.o SNN/WeightHistoryView/*.o SNN/AxonToLayerWeightView/*.o ../../../IzNeuronSimulators/Library/Graphs/*.o

.PHONY: clean



