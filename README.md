# 蒙特卡洛光线追踪

## 项目依赖

* stb_image
* tiny_obj_loader
* tinyxml2

## 实现功能

* 蒙特卡罗方法实现光线追踪
* Blinn-Phong光照模型
* BVH加速结构
* 直接光照的多重重要性采样
* 支持光线折射

## 命令行参数

* -spp：每个像素的采样数，默认为64
* -dir：场景文件夹所在的目录，默认为“example-scenes-cg21\”
* -scene：场景名，需保证场景文件夹、mtl文件和obj文件除后缀名外拥有同样名字，默认为"cornell-box"
* -o：输出文件路径，默认为“场景名.bmp"

## 运行结果

### cornell-box

#### 16spp
![](image/cornell-box16.png)

#### 64spp

![](image/cornell-box64.png)

#### 256spp

![](image/cornell-box256.png)

#### 1024spp

![](image/cornell-box1024.png)

#### 4096spp

![](image/cornell-box4096.png)

### bedroom

#### 16spp

![](image/bedroom16.png)

#### 64spp

![](image/bedroom64.png)

#### 256spp

![](image/bedroom256.png)

#### 1024spp

![](image/bedroom1024.png)

#### 4096spp

![](image/bedroom4096.png)

### veach-mis

#### 16spp

![](image/veach-mis16.png)

#### 64spp

![](image/veach-mis64.png)

#### 256spp

![](image/veach-mis256.png)

#### 1024spp

![](image/veach-mis1024.png)

#### 4096spp

![](image/veach-mis4096.png)

