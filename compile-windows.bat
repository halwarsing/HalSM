cd source
gcc -o ../build/HalSM.o -fPIC -ffreestanding -c HalSM.c
cd ../build
ar rcs -o ../library/LibHalSM.a HalSM.o
gcc -shared -o ../library/libhalsm.so HalSM.o -ffreestanding
cd ..