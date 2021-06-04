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

##更好的截图体验
通常截图软件还会使用热键，例如 QQ 的 Ctrl + Alt + A 。
如果需要在 Qt 中使用全局热键，这儿有一个优秀的库：
https://github.com/Skycoder42/QHotkey
本人亲测，非常好用~
