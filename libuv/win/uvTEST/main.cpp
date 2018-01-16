#include <stdio.h>
#include <stdlib.h>
#include "uv.h"

void print_infomation()
{
	int version = uv_version();
	const char* version_name = uv_version_string();
	printf("version:%d(%s).\n", version, version_name);

	int err = 10060;
	const char* errstr = uv_strerror(UV__EACCES);
	const char* errname = uv_err_name(UV__EACCES);
	printf("errstr:%s,errname:%s\n", errstr, errname);
}

void wait_for_a_while(uv_idle_t* handle) {
	static int counter = 0;;

	int tmp = uv_loop_alive(uv_default_loop());
	uint64_t stamp = uv_now(uv_default_loop());

	printf("counter:%d, lopper alive:%d, data:%d, stamp:%ld\n", counter, tmp, *(int *)(handle->data), stamp);

	//Sleep(10);

	if (counter++ >= 100)
		uv_idle_stop(handle);
}

void wait_for_a_while1(uv_idle_t* handle) {
	static int counter1 = 0;;

	int tmp = uv_loop_alive(uv_default_loop());

	printf("counter1:%d, lopper alive:%d\n", counter1, tmp);

	if (counter1++ >= 100)
		uv_idle_stop(handle);
}

int main()
{
	print_infomation();

	int tmp1 = 1;
	uv_idle_t idler;
	idler.data = &tmp1;

	uv_idle_init(uv_default_loop(), &idler);
	uv_idle_start(&idler, wait_for_a_while);

	//int tmp2 = 2;
	//uv_idle_t idler1;
	//idler1.data = &tmp2;
	//uv_idle_init(uv_default_loop(), &idler1);
	//uv_idle_start(&idler1, wait_for_a_while);

	printf("Idling...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	int tmp = uv_loop_alive(uv_default_loop());

	uv_loop_close(uv_default_loop());


	system("pause");
	return 0;
}
