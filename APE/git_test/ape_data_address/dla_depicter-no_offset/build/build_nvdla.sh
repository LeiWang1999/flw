#! /bin/sh

make clean
make

./nvdla_depicter --input lenet_large.nvdla
