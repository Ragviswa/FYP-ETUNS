unit:
	g++ units.cpp Unary.cpp -o main
	./main
	rm main

unitfile:
	g++ units.cpp Unary.cpp -o main
	./main > test.txt
	rm main

verilog:
	iverilog -g2012 -o UnaryTestbench ./UnaryUnitsFolder/UnaryTestbench/UnaryTestbench.sv ./UnaryUnitsFolder/UnaryMultiplier/UnaryMultiplierBounds.sv
	vvp UnaryTestbench

clean:
	echo "Cleaning Directory..."
	rm 
	rm -f transcript
	echo "Done!"