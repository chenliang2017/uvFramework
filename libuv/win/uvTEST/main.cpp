#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include "iLog.h"

#define DEFAULT_PORT 8000
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

typedef struct {
	uv_write_t req;
	uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
	write_req_t *wr = (write_req_t*)req;
	free(wr->buf.base);
	free(wr);
	iLogInfo("free write req");
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
	iLogInfo("alloc buffer");
}

void on_close(uv_handle_t* handle) {
	free(handle);
}

void echo_write(uv_write_t *req, int status) {
	iLogInfo("echo write.");
	if (status) {
		fprintf(stderr, "Write error %s\n", uv_strerror(status));
	}
	free_write_req(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	iLogInfo("echo read.");
	if (nread > 0) {
		write_req_t *req = (write_req_t*)malloc(sizeof(write_req_t));
		req->buf = uv_buf_init(buf->base, nread);
		uv_write((uv_write_t*)req, client, &req->buf, 1, echo_write);
		return;
	}
	if (nread < 0) {
		if (nread != UV__EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*)client, on_close);
	}

	free(buf->base);
}

void accept_func(uv_work_t *req) {
	uv_tcp_t *client = (uv_tcp_t *)(req->data);
	uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
	iLogInfo("accept_func\n");
}

void after_accept_func(uv_work_t *req, int status) {
	iLogInfo("Done after_accept_func\n");
}

void on_new_connection(uv_stream_t *server, int status) {
	if (status < 0) {
		printf("New connection error %s\n", uv_strerror(status));
		return;
	}
	iLogInfo("new connection arrive\n");

	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t*)client) == 0) {

		char ip[32] = { 0 };
		struct sockaddr sockname;
		int namelen = sizeof(sockname);
		uv_tcp_getpeername(client, &sockname, &namelen);


		struct sockaddr_in check_addr = *(struct sockaddr_in*)(&sockname);
		uv_ip4_name(&check_addr, ip, 32);
		printf("ip:%s\n", ip);

		uv_work_t *req = (uv_work_t*)malloc(sizeof(uv_work_t));;
		req->data = (void*)client;
		uv_queue_work(loop, req, accept_func, after_accept_func);
		//uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
	}
	else {
		uv_close((uv_handle_t*)client, on_close);
	}
}

int main() {

	log_handle_t *log = NULL;
	log = LogInit();
	if (log == NULL) {
		printf(">># create iLog handle errno, errno number: %d.\n", errno);
		return -1;
	}
	else {
		LogThreshold(ILOG_TYPE_DEBUG);
		LogSetPath("test.log");
	}


	loop = uv_default_loop();

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
	if (r) {
		printf("Listen error %s\n", uv_strerror(r));
		return 1;
	}
	uv_run(loop, UV_RUN_DEFAULT);

	printf("after uv_run\n");

	return 0;
}
