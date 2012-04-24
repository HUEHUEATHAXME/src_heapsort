all: compile random sorted reversed plot plot_compare

random:
	./heapsort_random 262144 heapsort_random.data

sorted:
	./heapsort_sorted 262144 heapsort_sorted.data

reversed:
	./heapsort_reversed 262144 heapsort_reversed.data

compile:
	gcc -o heapsort_random testsort.c heapsort.c time.c
	gcc -o heapsort_sorted testsort_sorted.c heapsort.c time.c
	gcc -o heapsort_reversed testsort_reversed.c heapsort.c time.c

plot_compare:
	gnuplot sortperf_compare.plot
	gv sortperf_compare.ps

plot:
	gnuplot sortperf_heapsort.plot
	gv sortperf_heapsort.ps

clean:
	rm -f *.o
	rm -f *~
	rm -f ./heapsort_random ./heapsort_sorted ./heapsort_reversed
	rm -f ./heapsort_random.data ./heapsort_sorted.data ./heapsort_reversed.data
	rm -f *.ps
