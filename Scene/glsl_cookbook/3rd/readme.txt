WIN:
1.将OpenGL目录copy到C:\

2.将OpenGL/dll中的动态库copy到system32（32bit）或 SysWOW64(64bit)

批处理文件已完成此工作

LINUX:
1.将OpenGL拷贝到/opt
cp -rfa OpenGL /opt

2.链接GLU
ln -s /usr/lib/i386-linux-gnu/libGLU.so.1 /usr/lib/libGLU.so

3.将OpenGL/lib/Linux中的库拷贝到/usr/lib
cp -a * /usr/lib

脚本已完成此工作

