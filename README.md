# QtScreenCapture
基于Qt5的GUI截图组件。

##开发环境
QtCreator + vs2019 + Qt5.12.10
在上测试通过，其他系统没试过，目测没啥大问题。

## 集成方法
下载解压代码后，将QtScreenCapture拷贝到工程文件目录
在pro工程文件中添加以下代码
`include(QtScreenCapture/ScreenCapture.pri)`
也可以拷贝到其他路径，需要在代码中更改对应的路径。

##使用方法
添加头文件
`#include <ScreenCapture/screencapture.h>`

调用抓图函数
```
auto image = ScreenCapture::capture();
// coding...
```