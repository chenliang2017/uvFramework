#include "TcpSend.h"
#include "iLog.h"

CTcpSend::CTcpSend()
{
	m_uv_stream = NULL;
}

CTcpSend::~CTcpSend()
{

}

void CTcpSend::SetStream(uv_stream_t* stream)
{
	std::lock_guard<std::mutex> lock(m_lock);

	m_uv_stream = stream;
}

void CTcpSend::SendData(CFLMessage* pdu, cfl_type_e type, tcp_send_cb cb)
{
	std::lock_guard<std::mutex> lock(m_lock);
	if (m_uv_stream == NULL){
		iLogInfo("can't send data, socket null.");
		return;
	}

	//TODO, FIX ME, ²ÉÓÃÄÚ´æ³Ø
	uv_send_req_t* uv_send_req = (uv_send_req_t*)malloc(sizeof(uv_send_req_t));
	uv_send_req->cb = cb;
	uv_send_req->pdu = (void*)pdu;
	uv_send_req->req.data = (void*)this;

	switch (type)
	{
	case CFL_Type_Json:
		uv_send_req->buf = uv_buf_init((char*)pdu->getMessage(), pdu->getMessageLength());
		break;
	case CFL_type_Binary:
		uv_send_req->buf = uv_buf_init((char*)pdu->getBinary(), pdu->getBinaryLength());
		break;
	default:
		iLogError("unknow pdu type:%d", type);
		return;
	}

	uv_write((uv_write_t*)uv_send_req, m_uv_stream, &uv_send_req->buf, 1, uv_write_callback);
}

void CTcpSend::uv_write_callback(uv_write_t *req, int status)
{
	uv_send_req_t* wr = (uv_send_req_t*)req;
	CTcpSend* sender = (CTcpSend*)(wr->req.data);
	if (sender){
		sender->on_write_callback_func(req, status);
	}
}

void CTcpSend::on_write_callback_func(uv_write_t *req, int status)
{
	uv_send_req_t* wr = (uv_send_req_t*)req;
	wr->cb(wr->pdu, status);
	free(wr);

	if (status){
		const char* msg = uv_strerror(status);
		iLogError("send message error, error message: %s", msg);
	}
}
