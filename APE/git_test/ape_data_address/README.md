


###########环境依赖
python3.5



############目录结构描述
├── Readme.md                   // help
├── ape_data.sh                 // sh运行脚本
├── umdx86                      // loadable文件解析成bin文件
├── bin2arrar                   // bin文件转数组配置
│   ├── tran.py
│   ├── ape_lenet或者ape_resnet50  // 存放umdx86解析的bin文件，文件名可自行在sh脚本中修改


############执行步骤
-> 注： 我这里在bin2arrar文件夹默认创建存放bin的文件夹是ape_resnet50，可自行在ape_data.sh中更改
-> 将需要的loadable文件放入umdx86下
-> ./ape_data.sh
-> 获取ape_data.sh运行后的txt文件(文件名是随机生成的)，如下：
    File "oZR5ka0Xxf6e1WH6_rTKf1Quh.txt", line 1
        Convolution 0
-> cd dla_depicter-no_offset/build
-> 将需要的loadable文件放入该目录下
-> python generate_cmd.py oZR5ka0Xxf6e1WH6_rTKf1Quh.txt cmd_data.c
-> cp cmd_data.c  ../../bin2arrar
-> 生成的配置都在bin2arrar中

#############version
20200817
