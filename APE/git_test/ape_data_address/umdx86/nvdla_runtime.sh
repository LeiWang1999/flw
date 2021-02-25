#! /bin/sh

make clean
make runtime

cp /home/flw/umdx86/out/apps/runtime/nvdla_runtime/nvdla_runtime /home/flw/umdx86
cp /home/flw/umdx86/out/core/src/runtime/libnvdla_runtime/libnvdla_runtime.so /home/flw/umdx86

./nvdla_runtime  --loadable resnet50_large.nvdla --image 0.jpg
