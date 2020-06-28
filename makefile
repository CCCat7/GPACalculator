cc = g++
prom = GPACalculator
src = GPACalculator.cpp
     
$(prom): $(src)
	$(cc) -o $(prom) $(src)
