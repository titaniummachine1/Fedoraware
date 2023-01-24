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
	t = std::thread(std::bind(&client::run, &wsClient));	
}

void CButtplug::Shutdown() {
	this->wsClient.stop();
	t.join();
}

void CButtplug::on_open(websocketpp::connection_hdl hdl) {
	I::Cvar->ConsolePrintf("Connection open\n");
	this->hdl = hdl;
	this->RequestServerInfo();
}

void CButtplug::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
	/*boost::json::object json= boost::json::parse(msg->get_payload()).as_array()[0].as_object();
	if (boost::json::value* msg = json.if_contains("ServerInfo"); msg != nullptr) {
		I::Cvar->ConsolePrintf("%s\n", boost::json::serialize(*msg));
	}*/
	I::Cvar->ConsolePrintf("%s", msg->get_payload().c_str());
}