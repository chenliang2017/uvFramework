
# uvFramework

#2018-1-23��create by chenliang

����libuv��ܽ��п�����ʹ��libuv���ʱ����Ҫע��һ�����㣺

1��Ԥ�����������꣺

_CRT_SECURE_NO_DEPRECATE
_CRT_NONSTDC_NO_DEPRECATE

2���������ɣ����п�Ϊ�����߳�(/MT)

3�������������⣺
advapi32.lib
iphlpapi.lib
psapi.lib
shell32.lib
user32.lib
userenv.lib
ws2_32.lib

#2018-1-25, modify by chenliang 

ͨ����libuv��ʹ�ã����֣�����libuv��ӿں����Ļص�����������ͨ��loop�̻߳ص�

������,���Բ����ڻص������������ӵ����㣬��Ȼ������loop�̣߳�Ӱ��libuv�����ܣ�

uv_queue_work�Ļص��������⣻�����������ͨ���̳߳�uv_queue_work����ɣ�
