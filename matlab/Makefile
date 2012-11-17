# makefile to build the mex-file of the yael matlab interface
# Require to construct a yael archive specifically adapted to yael 

CC=gcc 


include ../makefile.inc

# Here you can chose between Matlab and Octave
TARGET=matlab
# TARGET=octave


CFLAGS=-O3 -fPIC -m64 -g -I.. -fopenmp -DHAVE_ARPACK

ifeq "$(TARGET)" "matlab"


    # Matlab on Linux 64 bit is linked with a BLAS version where 
    # Fortran integers are 64 bit, so Yael has to be recompiled

    ifeq "$(YAELCONF)" "linux64"
        CFLAGS += -DFINTEGER=long 
    else 
    ifeq "$(YAELCONF)" "mac64"
        CFLAGS += -DFINTEGER=long 
    else 
        CFLAGS += -DFINTEGER=int # should be int in 32-bit mode
    endif
    endif

    YAELAR=yael4matlab.a

    LDFLAGS=-g $(YAELAR) LDFLAGS="\$$LDFLAGS -lmwblas -lmwlapack -lmwarpack -fopenmp"
    MEX=mex -g

else

    # modify this script to compile code to a octave-compilant mex file
    MEX=./octave_mex.sh
    YAELAR=yael4octave.a
    LDFLAGS=$(YAELAR)
endif


all: $(YAELAR) yael_kmeans yael_gmm yael_fisher yael_nn yael_kmin yael_kmax yael_fvecs_normalize yael_L2sqr yael_cross_distances yael_eigs yael_svds


LIBOBJ=vector.o matrix.o eigs.o nn.o kmeans.o gmm.o sorting.o binheap.o machinedeps.o vlad.o
YAELSRC=../yael/binheap.c ../yael/eigs.c ../yael/gmm.c ../yael/kmeans.c ../yael/machinedeps.c ../yael/matrix.c ../yael/nn.c ../yael/sorting.c ../yael/vector.c ../yael/vlad.c



binheap.o: ../yael/binheap.c ../yael/binheap.h
	gcc -o $@ -c $(CFLAGS) ../yael/binheap.c

eigs.o: ../yael/eigs.c ../yael/eigs.h
	gcc -o $@ -c $(CFLAGS) ../yael/eigs.c

gmm.o: ../yael/gmm.c ../yael/gmm.h
	gcc -o $@ -c $(CFLAGS) ../yael/gmm.c

kmeans.o: ../yael/kmeans.c ../yael/kmeans.h
	gcc -o $@ -c $(CFLAGS) ../yael/kmeans.c

machinedeps.o: ../yael/machinedeps.c ../yael/machinedeps.h
	gcc -o $@ -c $(CFLAGS) ../yael/machinedeps.c

matrix.o: ../yael/matrix.c ../yael/matrix.h
	gcc -o $@ -c $(CFLAGS) ../yael/matrix.c

nn.o: ../yael/nn.c ../yael/nn.h
	gcc -o $@ -c $(CFLAGS) ../yael/nn.c 

sorting.o: ../yael/sorting.c ../yael/sorting.h
	gcc -o $@ -c $(CFLAGS) ../yael/sorting.c

vector.o: ../yael/vector.c ../yael/vector.h
	gcc -o $@ -c $(CFLAGS) ../yael/vector.c

vlad.o: ../yael/vlad.c ../yael/vlad.h
	gcc -o $@ -c $(CFLAGS) ../yael/vlad.c

# dependencies generated with 
# for i in ../yael/*.c ; do cpp -MM $i ; done

binheap.o: ../yael/binheap.c ../yael/binheap.h ../yael/sorting.h
eigs.o: ../yael/eigs.c ../yael/eigs.h ../yael/vector.h ../yael/sorting.h \
 ../yael/machinedeps.h
gmm.o: ../yael/gmm.c ../yael/vector.h ../yael/matrix.h ../yael/kmeans.h \
 ../yael/nn.h ../yael/gmm.h ../yael/sorting.h ../yael/machinedeps.h
kmeans.o: ../yael/kmeans.c ../yael/vector.h ../yael/sorting.h \
 ../yael/matrix.h ../yael/kmeans.h ../yael/nn.h ../yael/machinedeps.h
machinedeps.o: ../yael/machinedeps.c ../yael/machinedeps.h
matrix.o: ../yael/matrix.c ../yael/vector.h ../yael/matrix.h \
 ../yael/sorting.h ../yael/machinedeps.h ../yael/eigs.h
nn.o: ../yael/nn.c ../yael/machinedeps.h ../yael/vector.h ../yael/nn.h \
 ../yael/binheap.h ../yael/sorting.h
sorting.o: ../yael/sorting.c ../yael/sorting.h ../yael/machinedeps.h \
 ../yael/binheap.h ../yael/vector.h
vector.o: ../yael/vector.c ../yael/vector.h
vlad.o: ../yael/vlad.c ../yael/vlad.h ../yael/nn.h ../yael/vector.h \
 ../yael/sorting.h

# the big archive 

yael4matlab.a: $(LIBOBJ)
	ar r yael4matlab.a $(LIBOBJ) 

yael4octave.a: 
	ar r $@ $(addprefix ../yael/,$(LIBOBJ))


yael_kmeans: yael_kmeans.c $(YAELAR)
	$(MEX) yael_kmeans.c -I.. $(LDFLAGS) 

yael_gmm: yael_gmm.c $(YAELAR)
	$(MEX) yael_gmm.c -I.. $(LDFLAGS)

yael_fisher: yael_fisher.c $(YAELAR)
	$(MEX) yael_fisher.c -I.. $(LDFLAGS)

yael_nn: yael_nn.c $(YAELAR)
	$(MEX) yael_nn.c -I.. $(LDFLAGS) 

yael_kmin: yael_kmin.c $(YAELAR)
	$(MEX) yael_kmin.c -I.. $(LDFLAGS)

yael_kmax: yael_kmax.c $(YAELAR)
	$(MEX) yael_kmax.c -I..  $(LDFLAGS)

yael_fvecs_normalize: yael_fvecs_normalize.c $(YAELAR)
	$(MEX) yael_fvecs_normalize.c -I.. $(LDFLAGS)

yael_L2sqr: yael_L2sqr.c $(YAELAR)
	$(MEX) yael_L2sqr.c -I.. $(LDFLAGS)

yael_cross_distances: yael_cross_distances.c $(YAELAR)
	$(MEX) yael_cross_distances.c -I.. $(LDFLAGS)

yael_eigs: yael_eigs.c $(YAELAR)
	$(MEX) yael_eigs.c -I.. $(LDFLAGS)

yael_svds: yael_svds.c $(YAELAR)
	$(MEX) yael_svds.c -I.. $(LDFLAGS)

clean:
	rm -f  *.a *.mexa64 *.mexmaci64 *.mex *.o *~
