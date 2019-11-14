all: main.c huff.c huff.h bitfile.c bitfile.h
	gcc -Wall -O0 -g -o huff main.c bitfile.c huff.c -I. -lm
	gcc -Wall -O0 -g -o huff_be main.c bitfile.c huff.c -I. -lm

test: showcodes encode decode

showcodes:
	$(info Test Showcodes: )
	bash -c "./huff showcodes modern-prometheus-unix.txt > showcodes.student"	
	bash -c "if diff -q showcodes.expected showcodes.student; then echo test passed; else echo test failed; fi;"
	bash -c "echo"

encode:
	$(info Test Encode: )
	bash -c "echo test with a tiny file"
	bash -c "./huff_be encode modern-prometheus-unix.txt tiny-encoding.input tiny-encoding.student"
	bash -c "if diff -q tiny-encoding.expected tiny-encoding.student; then echo test tiny passed; else if diff -q tiny-encoding.expected-be tiny-encoding.student; then echo You are nearly there, just flip the order of your bits in your bytes to get the correct answer!!!; else echo test tiny failed; fi; fi;"
	bash -c "echo default encoding test"
	bash -c "./huff_be encode modern-prometheus-unix.txt encode.input encode.student"
	bash -c "if diff -q encode.expected encode.student; then echo test default passed; else if diff -q encode.expected-be encode.student; then echo You are nearly there, just flip the order of your bits in your bytes to get the correct answer!!!; else echo default tiny failed; fi; fi;"
	bash -c "echo"

decode:
	$(info Test Decode: )
	bash -c "./huff decode modern-prometheus-unix.txt decode.input decode.student"
	bash -c "if diff -q decode.expected decode.student; then echo test passed; else echo test failed; fi;"
	bash -c "echo"

clean:
	rm huff

compile: main.c huff.c huff.h
	gcc -Wall -g -o huff main.c huff.c -I. -lm

.SILENT: test encode decode showcodes
