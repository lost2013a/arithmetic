C����
���Ӿ�̬�� $(CC) -o test_s.bin test.o libET299.a
���Ӷ�̬�� $(CC) -lssl -lcrypto -o test_d.bin test.o libET299.so.0.3

test_d.binΪtest.c���ö�̬��libET299.so.0.3�����Ŀ�ִ���ļ�
test_s.binΪtest.c���þ�̬��libET299.a�����Ŀ�ִ���ļ�

��libET299.a��libET299.so.0.3���Էŵ�/usr/lib��


Java����
ET299JTest.javaΪ�����ļ�
ET299JTest.classΪ�����Ŀ�ִ���ļ�

ET299jni.jar��libJET299ai20.so�ŵ��ɷ���·���£��磺
libJET299ai20.so�ŵ�/usr/local/jdk1.5.0/jre/lib/i386/libJET299ai20.so
ET299jni.jar�ŵ�/usr/local/jdk1.5.0/lib/ET299jni.jar 

���߿���ʹ��System.out.println(System.getProperty("java.library.path"));���ҵ���ǰϵͳ��java����·��