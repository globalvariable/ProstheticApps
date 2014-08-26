prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: TrialHandler 

TrialHandler: TrialHandler.c Gui.o GuiTrialHandler.o ../../../BlueSpike/Library/Misc/Misc.o ../../../ExperimentHandlers/Library/Messages/Gui2TrialHand.o TrialHandlerRtTask.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o HandleGui2TrialHandMsgs.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand.o ../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.o HandleTrialDurHand2TrialHandMsgs.o HandleTrialHand2TrialDurHandMsgs.o HandleExpEnviHand2TrialHandMsgs.o HandleMovObjHand2TrialHandMsgs.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.o ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.o ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.o ../../../ExperimentHandlers/Library/Status/TrialDurStatus.o ../../../BlueSpike/Library/Misc/Randn.o HandleTrialHandDur.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o TrialHistory.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o ../../../ExperimentHandlers/Library/Messages/TrialHand2Gui.o ../../../ExperimentHandlers/Library/Status/TrialStatusHistory.o ../../../BlueSpike/Library/Misc/Averaging.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< Gui.o GuiTrialHandler.o ../../../BlueSpike/Library/Misc/Misc.o ../../../ExperimentHandlers/Library/Messages/Gui2TrialHand.o TrialHandlerRtTask.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o HandleGui2TrialHandMsgs.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand.o ../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.o HandleTrialDurHand2TrialHandMsgs.o HandleTrialHand2TrialDurHandMsgs.o HandleExpEnviHand2TrialHandMsgs.o HandleMovObjHand2TrialHandMsgs.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.o ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.o ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.o ../../../ExperimentHandlers/Library/Status/TrialDurStatus.o ../../../BlueSpike/Library/Misc/Randn.o HandleTrialHandDur.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o TrialHistory.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o ../../../ExperimentHandlers/Library/Messages/TrialHand2Gui.o ../../../ExperimentHandlers/Library/Status/TrialStatusHistory.o ../../../BlueSpike/Library/Misc/Averaging.o ### TrialHandler

Gui.o: Gui.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

GuiTrialHandler.o: GuiTrialHandler.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### GuiTrialHandler

TrialHandlerRtTask.o: TrialHandlerRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### TrialHandlerRtTask

HandleGui2TrialHandMsgs.o: HandleGui2TrialHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleGui2TrialHandMsgs

HandleTrialDurHand2TrialHandMsgs.o: HandleTrialDurHand2TrialHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleTrialDurHand2TrialHandMsgs

HandleTrialHandDur.o: HandleTrialHandDur.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleTrialHandDur

HandleTrialHand2TrialDurHandMsgs.o: HandleTrialHand2TrialDurHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleTrialHand2TrialDurHandMsgs

HandleExpEnviHand2TrialHandMsgs.o: HandleExpEnviHand2TrialHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleExpEnviHand2TrialHandMsgs

HandleMovObjHand2TrialHandMsgs.o: HandleMovObjHand2TrialHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleMovObjHand2TrialHandMsgs

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

../../../BlueSpike/Library/Misc/Averaging.o: ../../../BlueSpike/Library/Misc/Averaging.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Averaging

../../../BlueSpike/Library/Misc/Randn.o: ../../../BlueSpike/Library/Misc/Randn.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Randn

../../../ExperimentHandlers/Library/Messages/Gui2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/Gui2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/Gui2TrialHand

../../../ExperimentHandlers/Library/Messages/TrialHand2Gui.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2Gui

../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand

../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand

../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand

../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand

../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand

../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet

../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand

../../../ExperimentHandlers/Library/Status/TrialStatus.o: ../../../ExperimentHandlers/Library/Status/TrialStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/TrialStatus.o

../../../ExperimentHandlers/Library/Status/TrialStatusHistory.o: ../../../ExperimentHandlers/Library/Status/TrialStatusHistory.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/TrialStatusHistory.o

../../../ExperimentHandlers/Library/Status/TrialDurStatus.o: ../../../ExperimentHandlers/Library/Status/TrialDurStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/TrialDurStatus.o

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c	
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o

TrialHandParadigm.o: TrialHandParadigm.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### TrialHandParadigm

TrialHistory.o: TrialHistory.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### TrialHistory

DataHandling/DataHandling.o: DataHandling/DataHandling.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/DataHandling

DataHandling/v0/DataFormat_v0.o: DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/v0/DataFormat_v0

../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand

clean:
	rm -f TrialHandler *.o ../../../BlueSpike/Library/Misc/*.o ../../../ExperimentHandlers/Library/Messages/*.o ../../../ExperimentHandlers/Library/TrialsData/*.o ../../../ExperimentHandlers/Library/Combos/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../../ExperimentHandlers/Library/Status/*.o ../../../BlueSpike/Library/Thresholding/*.o ../../../BlueSpike/Library/LogBuffer/*.o ../../../ExperimentHandlers/Library/Coordinate/*.o DataHandling/*.o DataHandling/v0/*.o 

.PHONY: clean
