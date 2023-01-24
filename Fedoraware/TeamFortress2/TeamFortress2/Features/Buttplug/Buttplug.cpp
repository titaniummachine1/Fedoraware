#include "Buttplug.h"

void CButtplug::Init() {
	wsClient.init_asio();
	wsClient.set_message_handler(std::bind(&CButtplug::on_message, this, std::placeholders::_1, std::placeholders::_2));
	wsClient.set_open_handler(std::bind(&CButtplug::on_open, this, std::placeholders::_1));

	websocketpp::lib::error_code err;
	client::connection_ptr con = wsClient.get_connection("ws://127.0.0.1:12345", err);
	if (err) {
		I::Cvar->ConsolePrintf("Error occured starting websocket connection: %s\n", err.value());
		return;
	}
	wsClient.connect(con);
	std::thread t (std::bind(&client::run, &wsClient));
	
	t.detach();
}



void CButtplug::on_open(websocketpp::connection_hdl hdl) {
	I::Cvar->ConsolePrintf("Connection open\n");
	this->hdl = hdl;
	this->RequestServerInfo();
}

void CButtplug::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
	I::Cvar->ConsolePrintf("WS MSG: %s\n", msg->get_payload());
}