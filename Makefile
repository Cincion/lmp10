hermite: main.o splines.o points.o aproxhermite.o gaus/libge.a
	$(CC) -o hermite  main.o splines.o points.o aproxhermite.o -L gaus -l ge -lm -Wall -ansi -pedantic

aprox: main.o splines.o points.o aproksymator_na_bazie.o gaus/libge.a
	$(CC) -o aprox  main.o splines.o points.o aproksymator_na_bazie.o -L gaus -l ge

intrp: main.o splines.o points.o interpolator.o gaus/libge.a
	$(CC) -o intrp  main.o splines.o points.o interpolator.o -L gaus -l ge

prosta: main.o splines.o points.o prosta.o
	$(CC) -o prosta  main.o splines.o points.o prosta.o	

aproksymator_na_bazie.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_na_bazie.c

aproxhermite.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproxhermite.c -Wall -ansi -pedantic

interpolator.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c interpolator.c

.PHONY: clean test cc

test:
	./hermite -s spl -p test/dane.2 -g myplot -f 40 -t 100 -n 300	

gen: 
	./test/gen 100 0 100 ./test/dane.2

cc:
	-rm *.o aprox intrp prosta aproxhermite
