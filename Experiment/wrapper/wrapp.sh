rm digital.o digital.py _digital.so digital_wrap.c digital_wrap.o
swig -c++ -python digital.i
g++ -std=c++17 -c -fpic digital_wrap.cxx ../digital.cpp -I/usr/include/python3.5m
g++ -shared digital.o digital_wrap.o -o _digital.so
mkdir python
cp _digital.so python/
cp digital.py python/

##attention
# change -I/usr/include/python3.5m with your python version
# you can find it by 'python-config --cflags' command
