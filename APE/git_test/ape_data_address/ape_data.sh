#! /bin/sh
#loadable to bin file
cd ./bin2arrar
rm -r ape_resnet50 *.c *.h
mkdir ape_resnet50
cd ../umdx86
make clean
make runtime


cp /home/flw/桌面/test_tools/ape_data_address/umdx86/out/apps/runtime/nvdla_runtime/nvdla_runtime /home/flw/umdx86
cp /home/flw/桌面/test_tools/ape_data_address/umdx86/out/core/src/runtime/libnvdla_runtime/libnvdla_runtime.so /home/flw/umdx86

./nvdla_runtime  --loadable resnet50_large.nvdla --image 0.jpg

mv *.bin ../bin2arrar/ape_resnet50

#bin to arrar data
cd ../bin2arrar
python tran.py ape_resnet50

#generate cmd_data.c
cd ../dla_depicter-no_offset/build
rm cmd_data.c
./nvdla_depicter --input lenet_large.nvdla --output lenet_large_output

