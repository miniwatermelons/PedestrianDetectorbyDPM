# PedestrianDetectorbyDPM
Detector Pedestrian by DPM algorithm (opencv)

Requires:
Opencv 3.4.0
Opencv_contrib 3.4.0
Visual Studio 2017

环境配置方法：
参考该博客
https://www.cnblogs.com/aiwuzhi/p/7334514.html

用Opencv中的DPM算法，检测行人

使用方法：
编译本项目后，有三个参数：
model_path ： 使用的模型，使用的模型在Opencv_contrib的包里
image_dir  ： 图片所在目录
image_list ： 所处理文件名的文件

注意 PedestrianDetectorbyDPM/Project1/WindowsShow.cpp文件中
const string model_path = "D:\\test\\dpm\\inriaperson.xml"; 是固定的
需要设置你的inriaperson.xml所在目录

编译完成后，可以只行exe文件
例子： PedestrianDetectorbyDPM.exe D:\dataset D:\test\files
第一个参数给image_dir，第二个参数给image_list