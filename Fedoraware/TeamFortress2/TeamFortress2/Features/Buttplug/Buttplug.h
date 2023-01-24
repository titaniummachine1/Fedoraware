#pragma once
#include "ButtplugStructs.h"

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp> 

#include "../../SDK.h"
#include <initializer_list>

#include "../../SDK/Discord/src/rapidjson/fwd.h"
#include "../../SDK/Discord/src/rapidjson/rapidjson.h"
#include "../../SDK/Discord/src/rapidjson/document.h"
#include "../../SDK/Discord/src/rapidjson/writer.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

class CButtplug {
private:
	client wsClient;
	websocketpp::connection_hdl hdl;
	std::thread t;
	

	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	void on_open(websocketpp::connection_hdl hdl);

	int msgId = 1;

	void DoRequest(std::string, boost::json::object);
	void PingLoop();
public:
	void Ping();
	void RequestServerInfo();
	void StartScanning();
	void StopScanning();
	void RequestDeviceList();
	void StopDeviceCmd(int deviceIdx);
	void StopAllDevices();
	void ScalarCmd(int deviceIdx, std::vector<ButtPlug::BPScalar> scalars);
	void LinearCmd(int deviceIdx, ButtPlug::BPLinear vectors...);
	void RotateCmd(int deviceIdx, ButtPlug::BPRotate rotations...);
	void Init();
	void Shutdown();
};

ADD_FEATURE(CButtplug, Buttplug)
