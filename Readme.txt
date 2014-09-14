Group Member: Yaolin Zhang

How to run our program?
1. make clean
2. make
3. make r or ./SDVSystem <User Number> <Channel Number> <SDV Channel Number> <Slice Channel Number> <Zipf Shape> <Simulation time> <Flipping behavior>

PS: 
1. If you use make r to run our program, you must configure all the parameters through the Makefile, it looks like "r: @./$(EXEC) 257 150 150 150 0.8 10000 1" in the Makefile.
2. For testing of no blocking, you will need configure slice number as the same with SDV channel number. After running, you can get the average number request, this is the estimate slice number in a SDV system for a given configuration.
3. For testing of blocking, you will need configure slice number smaller than SDV channel number and you can use the average number request that you got from the no-blocking test as the slice number. Then the average number request outputted will be pretty close to the slice number and the blocking probability will pretty small, for example, 0.08%.
4. The log file is called "log.dat".
