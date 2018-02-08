
# uvFramework

#2018-1-23，create by chenliang

采用libuv框架进行开发，使用libuv框架时，需要注意一下三点：

1、预处理器新增宏：

_CRT_SECURE_NO_DEPRECATE
_CRT_NONSTDC_NO_DEPRECATE

2、代码生成，运行库为：多线程(/MT)

3、依赖第三方库：
advapi32.lib
iphlpapi.lib
psapi.lib
shell32.lib
user32.lib
userenv.lib
ws2_32.lib

#2018-1-25, modify by chenliang 

通过对libuv的使用，发现，凡是libuv库接口函数的回调函数，均是通过loop线程回调

出来的,所以不能在回调函数中做复杂的运算，不然会阻塞loop线程，影响libuv的性能，

uv_queue_work的回调函数除外；复杂运算可以通过线程池uv_queue_work来完成；
