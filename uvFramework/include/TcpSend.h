#ifndef _DAS_TCP_SEND_H_
#define _DAS_TCP_SEND_H_

#include <mutex>

#include "uv.h"
#include "CFL_Message.h"

class  CFLMessage;

typedef void(*tcp_send_cb)(void* pdu, int status);

typedef struct uv_send_req{
	uv_write_t	req;
	uv_buf_t	buf;
	void*		pdu;
	tcp_send_cb	cb;
}uv_send_req_t;

typedef enum
{
	CFL_Type_Unknow = 0,
	CFL_Type_Json,
	CFL_type_Binary,
} cfl_type_e;

class CTcpSend
{
public:
	CTcpSend();
	~CTcpSend();

public:
	// 设置uvlib的stream，用于发送数据
	void SetStream(uv_stream_t* stream);
	// 未回调之前，不能释放data数据指针
	void SendData(CFLMessage* pdu, cfl_type_e type, tcp_send_cb cb);

private:
	static void uv_write_callback(uv_write_t *req, int status);

private:
	void on_write_callback_func(uv_write_t *req, int status);

private:
	uv_stream_t*		m_uv_stream;
	std::mutex			m_lock;
};

#endif
