prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: ExpEnviHandler 

ExpEnviHandler: ExpEnviHandler.c Gui.o ../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.o ../../../BlueSpike/Library/Misc/Misc.o ExpEnviHandlerRtTask.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o GuiExpEnviHandler.o ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.o ../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.o ExpEnviDurationHandlerRtTask.o HandleExpEnviHand2ExpEnviDurHandMsgs.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.o ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviComponentTypes.o ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.o HandleGui2ExpEnviHandMsgs.o HandleExpEnviDurHand2ExpEnviHandMsgs.o HandleTrialHand2ExpEnviHandMsgs.o HandleExpEnviHandDur.o ../../../ExperimentHandlers/Library/Status/ExpEnviStatus.o ../../../ExperimentHandlers/Library/Status/ExpEnviDurStatus.o ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate.o ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatus.o HandleRS232Buffers.o ../../../ExperimentHandlers/Library/RS232/RS232Handler.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory.o 
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< Gui.o ../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.o ../../../BlueSpike/Library/Misc/Misc.o ExpEnviHandlerRtTask.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o GuiExpEnviHandler.o ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.o ../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.o ExpEnviDurationHandlerRtTask.o HandleExpEnviHand2ExpEnviDurHandMsgs.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.o ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviComponentTypes.o ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.o HandleGui2ExpEnviHandMsgs.o HandleExpEnviDurHand2ExpEnviHandMsgs.o HandleTrialHand2ExpEnviHandMsgs.o HandleExpEnviHandDur.o ../../../ExperimentHandlers/Library/Status/ExpEnviStatus.o ../../../ExperimentHandlers/Library/Status/ExpEnviDurStatus.o ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate.o ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatus.o HandleRS232Buffers.o ../../../ExperimentHandlers/Library/RS232/RS232Handler.o ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory.o ### ExpEnviHandler

Gui.o: Gui.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

GuiExpEnviHandler.o: GuiExpEnviHandler.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### GuiExpEnviHandler

ExpEnviHandlerRtTask.o: ExpEnviHandlerRtTask.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ExpEnviHandlerRtTask

ExpEnviDurationHandlerRtTask.o: ExpEnviDurationHandlerRtTask.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ExpEnviDurationHandlerRtTask

HandleGui2ExpEnviHandMsgs.o: HandleGui2ExpEnviHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleGui2ExpEnviHandMsgs

HandleExpEnviDurHand2ExpEnviHandMsgs.o: HandleExpEnviDurHand2ExpEnviHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleExpEnviDurHand2ExpEnviHandMsgs

HandleTrialHand2ExpEnviHandMsgs.o: HandleTrialHand2ExpEnviHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleTrialHand2ExpEnviHandMsgs

HandleExpEnviHand2ExpEnviDurHandMsgs.o: HandleExpEnviHand2ExpEnviDurHandMsgs.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleExpEnviHand2ExpEnviDurHandMsgs

HandleExpEnviHandDur.o: HandleExpEnviHandDur.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleExpEnviHandDur

HandleRS232Buffers.o: HandleRS232Buffers.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleRS232Buffers

../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.o: ../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand

../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui.o: ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui

../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand

../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.o: ../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand

../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.o: ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand

../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviComponentTypes.o: ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviComponentTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviComponentTypes

../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.o: ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData

../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate.o: ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate

../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory.o: ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory

../../../ExperimentHandlers/Library/Status/ExpEnviStatus.o: ../../../ExperimentHandlers/Library/Status/ExpEnviStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/ExpEnviStatus

../../../ExperimentHandlers/Library/Status/ExpEnviDurStatus.o: ../../../ExperimentHandlers/Library/Status/ExpEnviDurStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/ExpEnviDurStatus

../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatus.o: ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatus

../../../ExperimentHandlers/Library/RS232/RS232Handler.o: ../../../ExperimentHandlers/Library/RS232/RS232Handler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/RS232/RS232Handler

DataHandling/DataHandling.o: DataHandling/DataHandling.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/DataHandling

DataHandling/v0/DataFormat_v0.o: DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/v0/DataFormat_v0

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../../BlueSpikeLibrary/Misc/Misc

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c	
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o

../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand

clean:
	rm -f ExpEnviHandler *.o ../../../BlueSpike/Library/Misc/*.o ../../../ExperimentHandlers/Library/Messages/*.o ../../../ExperimentHandlers/Library/TrialsData/*.o ../../../ExperimentHandlers/Library/Combos/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../../ExperimentHandlers/Library/Status/*.o ../../../ExperimentHandlers/Library/RS232/*.o ../../../ExperimentHandlers/Library/ExpEnviData/*.o ../../../ExperimentHandlers/Library/MovObjData/*.o ../../../IzNeuronSimulators/Library/SpikeData/*.o ../../../ExperimentHandlers/Library/Servo/*.o ../../../ExperimentHandlers/Library/Robot/*.o ../../../BlueSpike/Library/Thresholding/*.o ../../../ExperimentHandlers/Library/Coordinate/*.o DataHandling/*.o DataHandling/v0/*.o 

.PHONY: clean
