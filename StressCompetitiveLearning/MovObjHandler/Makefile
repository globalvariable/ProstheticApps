prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: MovObjHandler 

MovObjHandler: MovObjHandler.c Gui.o ../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.o ../../../BlueSpike/Library/Misc/Misc.o MovObjHandlerRtTask.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o GuiMovObjHandler.o ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.o ../../../ExperimentHandlers/Library/Messages/MovObjDurHand2MovObjHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand.o HandleMovObjHand2MovObjDurHandMsgs.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.o HandleMovObjDurHand2MovObjHandMsgs.o HandleTrialHand2MovObjHandMsgs.o HandleGui2MovObjHandMsgs.o ../../../ExperimentHandlers/Library/MovObjData/MovObjComponentTypes.o ../../../ExperimentHandlers/Library/MovObjData/MovObjData.o ../../../ExperimentHandlers/Library/Status/MovObjStatus.o ../../../ExperimentHandlers/Library/Status/MovObjDurStatus.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o HandleNeuralNet2MovObjHandMsgs.o ../../../IzNeuronSimulators/Library/SpikeData/SpikeData.o HandleSpikeDataBuff.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2Gui.o ../../../ExperimentHandlers/Library/Servo/Servo.o ../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.o ../../../ExperimentHandlers/Library/RS232/RS232Handler.o ../../../BlueSpike/Library/LogBuffer/MessageLogBuffer.o HandleRS232Buffers.o HandleRobotPosition.o ../../../BlueSpike/Library/Thresholding/Thresholding.o MovObjHandParadigm.o ../../../ExperimentHandlers/Library/Coordinate/Cartesian.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o ../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.o ../../../ExperimentHandlers/Library/Robot/ThreeDofRobotHistory.o 
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< Gui.o ../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.o ../../../BlueSpike/Library/Misc/Misc.o MovObjHandlerRtTask.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o GuiMovObjHandler.o ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.o ../../../ExperimentHandlers/Library/Messages/MovObjDurHand2MovObjHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand.o HandleMovObjHand2MovObjDurHandMsgs.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.o HandleMovObjDurHand2MovObjHandMsgs.o HandleTrialHand2MovObjHandMsgs.o HandleGui2MovObjHandMsgs.o ../../../ExperimentHandlers/Library/MovObjData/MovObjComponentTypes.o ../../../ExperimentHandlers/Library/MovObjData/MovObjData.o ../../../ExperimentHandlers/Library/Status/MovObjStatus.o ../../../ExperimentHandlers/Library/Status/MovObjDurStatus.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o HandleNeuralNet2MovObjHandMsgs.o ../../../IzNeuronSimulators/Library/SpikeData/SpikeData.o HandleSpikeDataBuff.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2Gui.o ../../../ExperimentHandlers/Library/Servo/Servo.o ../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.o ../../../ExperimentHandlers/Library/RS232/RS232Handler.o ../../../BlueSpike/Library/LogBuffer/MessageLogBuffer.o HandleRS232Buffers.o HandleRobotPosition.o ../../../BlueSpike/Library/Thresholding/Thresholding.o MovObjHandParadigm.o ../../../ExperimentHandlers/Library/Coordinate/Cartesian.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o ../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.o ../../../ExperimentHandlers/Library/Robot/ThreeDofRobotHistory.o ### MovObjHandler

Gui.o: Gui.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

MovObjHandlerRtTask.o: MovObjHandlerRtTask.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### MovObjHandlerRtTask

GuiMovObjHandler.o: GuiMovObjHandler.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### GuiMovObjHandler

HandleMovObjHand2MovObjDurHandMsgs.o: HandleMovObjHand2MovObjDurHandMsgs.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleMovObjHand2MovObjDurHandMsgs

HandleMovObjDurHand2MovObjHandMsgs.o: HandleMovObjDurHand2MovObjHandMsgs.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleMovObjDurHand2MovObjHandMsgs

HandleTrialHand2MovObjHandMsgs.o: HandleTrialHand2MovObjHandMsgs.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleTrialHand2MovObjHandMsgs

HandleGui2MovObjHandMsgs.o: HandleGui2MovObjHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleGui2MovObjHandMsgs

HandleNeuralNet2MovObjHandMsgs.o: HandleNeuralNet2MovObjHandMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleNeuralNet2MovObjHandMsgs.o

HandleSpikeDataBuff.o: HandleSpikeDataBuff.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleSpikeDataBuff.o

HandleRS232Buffers.o: HandleRS232Buffers.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleRS232Buffers

HandleRobotPosition.o: HandleRobotPosition.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleRobotPosition

../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.o: ../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand

../../../ExperimentHandlers/Library/Messages/MovObjHand2Gui.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2Gui

../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand

../../../ExperimentHandlers/Library/Messages/MovObjDurHand2MovObjHand.o: ../../../ExperimentHandlers/Library/Messages/MovObjDurHand2MovObjHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjDurHand2MovObjHand

../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand

../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet

../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand

../../../ExperimentHandlers/Library/MovObjData/MovObjComponentTypes.o: ../../../ExperimentHandlers/Library/MovObjData/MovObjComponentTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/MovObjData/MovObjComponentTypes

../../../ExperimentHandlers/Library/MovObjData/MovObjData.o: ../../../ExperimentHandlers/Library/MovObjData/MovObjData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/MovObjData/MovObjData

../../../ExperimentHandlers/Library/MovObjData/MovObjDirectionTypes.o: ../../../ExperimentHandlers/Library/MovObjData/MovObjDirectionTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/MovObjData/MovObjDirectionTypes

../../../ExperimentHandlers/Library/Status/MovObjStatus.o: ../../../ExperimentHandlers/Library/Status/MovObjStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/MovObjStatus

../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.o: ../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.o

../../../ExperimentHandlers/Library/Status/MovObjDurStatus.o: ../../../ExperimentHandlers/Library/Status/MovObjDurStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/MovObjDurStatus

../../../IzNeuronSimulators/Library/SpikeData/SpikeData.o: ../../../IzNeuronSimulators/Library/SpikeData/SpikeData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../IzNeuronSimulators/Library/SpikeData/SpikeData

../../../ExperimentHandlers/Library/Servo/Servo.o: ../../../ExperimentHandlers/Library/Servo/Servo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Servo/Servo

../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.o: ../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Robot/ThreeDofRobot

../../../ExperimentHandlers/Library/Robot/ThreeDofRobotHistory.o: ../../../ExperimentHandlers/Library/Robot/ThreeDofRobotHistory.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Robot/ThreeDofRobotHistory

../../../ExperimentHandlers/Library/Coordinate/Cartesian.o: ../../../ExperimentHandlers/Library/Coordinate/Cartesian.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Coordinate/Cartesian

../../../BlueSpike/Library/LogBuffer/MessageLogBuffer.o: ../../../BlueSpike/Library/LogBuffer/MessageLogBuffer.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/LogBuffer/MessageLogBuffer

../../../BlueSpike/Library/Thresholding/Thresholding.o: ../../../BlueSpike/Library/Thresholding/Thresholding.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Thresholding/Thresholding

MovObjHandParadigm.o: MovObjHandParadigm.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### MovObjHandParadigm

DataHandling/DataHandling.o: DataHandling/DataHandling.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/DataHandling

DataHandling/v0/DataFormat_v0.o: DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/v0/DataFormat_v0

../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../../BlueSpikeLibrary/Misc/Misc

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c	
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o

../../../ExperimentHandlers/Library/RS232/RS232Handler.o: ../../../ExperimentHandlers/Library/RS232/RS232Handler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/RS232/RS232Handler

clean:
	rm -f MovObjHandler *.o ../../../BlueSpike/Library/Misc/*.o ../../../ExperimentHandlers/Library/Messages/*.o ../../../ExperimentHandlers/Library/TrialsData/*.o ../../../ExperimentHandlers/Library/Combos/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../../ExperimentHandlers/Library/Status/*.o ../../../ExperimentHandlers/Library/RS232/*.o ../../../ExperimentHandlers/Library/MovObjData/*.o ../../../IzNeuronSimulators/Library/SpikeData/*.o ../../../ExperimentHandlers/Library/Servo/*.o ../../../ExperimentHandlers/Library/Robot/*.o ../../../BlueSpike/Library/Thresholding/*.o ../../../BlueSpike/Library/LogBuffer/*.o ../../../ExperimentHandlers/Library/Coordinate/*.o DataHandling/*.o DataHandling/v0/*.o 

.PHONY: clean
