C语言
链接静态库 $(CC) -o test_s.bin test.o libET299.a
链接动态库 $(CC) -lssl -lcrypto -o test_d.bin test.o libET299.so.0.3

test_d.bin为test.c调用动态库libET299.so.0.3编译后的可执行文件
test_s.bin为test.c调用静态库libET299.a编译后的可执行文件

将libET299.a和libET299.so.0.3可以放到/usr/lib下


Java语言
ET299JTest.java为测试文件
ET299JTest.class为编译后的可执行文件

ET299jni.jar和libJET299ai20.so放到可访问路径下，如：
libJET299ai20.so放到/usr/local/jdk1.5.0/jre/lib/i386/libJET299ai20.so
ET299jni.jar放到/usr/local/jdk1.5.0/lib/ET299jni.jar 

或者可以使用System.out.println(System.getProperty("java.library.path"));来找到当前系统的java运行路径