# Hermes
Seriel connection for Odroid-U3 to Arduino

./ConnectionRead/Connection.c is the source code for the serial connection that reads the data from the Arduino
./ConnectionRead/Read.c is the code that's implemented in to open davinci.
./Arduino/arduinoSend/arduinoSend.ino is the source from which the serial connection reads.
./ConnectionRead/Release.c is for realeasing the memory allocated.

./ConnectionSend/Send.c is the source code for the serial connection that send to the Arduino from the odroid
./ConnectionSend/Memory.c is the code running in open davinci to send the data to memory.
./Arduino/self_driving/self_driving.ino is the code running on the Arduino controlling the steering and speed.
./ConnectionSend/Deallocate.c is for release the memory.
