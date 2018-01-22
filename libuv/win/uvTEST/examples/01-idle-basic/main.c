#include <stdio.h>
#include "uv.h"

// idle callbacks are processed after all other types of callbacks are processed
void idle_cb(uv_idle_t* handle) {
  static int64_t count = -1;
  count++;
  // probably better to write to stdout stream to have those show up immediately
  if ((count % 10000) == 0) printf(".");
  if (count >= 1000000) uv_idle_stop(handle);
}

void timer_cb(uv_timer_t *handle)
{
	static int64_t ccc = 1;
	printf("counter:%d, timer out,in 10ms\n", ccc++);
	if (ccc > 100){
		uv_timer_stop(handle);
	}
}

int main() 
{
	char* errstr = uv_strerror(UV__EBADF);
	char* errname = uv_err_name(UV__EBADF);
	int err = uv_translate_sys_error(10053);
	printf("errstr:%s, errname:%s, err:%d\n", errstr, errname, err);

	unsigned int version = uv_version(); 
	char* verstr = uv_version_string();
	printf("version:%d, verstr:%s\n", version, verstr);

  uv_idle_t idle_handle;
  uv_timer_t time_handle;

  uv_loop_t *loop = uv_default_loop();

  uv_idle_init(loop, &idle_handle);
  uv_idle_start(&idle_handle, idle_cb);

  uv_timer_init(loop, &time_handle);
  uv_timer_start(&time_handle,timer_cb , 10, 10);

  printf("\nidling ...\n");
  uv_run(loop, UV_RUN_DEFAULT);

  uv_loop_close(loop);
  system("pause");
  return 0;
}
