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
	this->t.join();
	this->t.~thread();
}

void CButtplug::on_open(websocketpp::connection_hdl hdl) {
	this->hdl = hdl;
	this->RequestServerInfo();
}

void CButtplug::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
	boost::json::object json= boost::json::parse(msg->get_payload()).as_array()[0].as_object();
	if (boost::json::value* msg = json.if_contains("DeviceList"); msg != nullptr) {
		this->PopulateMotors(msg->as_object()["Devices"].as_array());

	}

	if (boost::json::value* msg = json.if_contains("ServerInfo"); msg != nullptr) { // This means like its loaded and it will cry if stuff is sent before
		this->RequestDeviceList();
	}
}

void CButtplug::PopulateMotors(boost::json::array devices) {
	this->Motors.clear();
	for (boost::json::value device : devices) {
		boost::json::object obj = device.as_object();
		int deviceIdx = obj["DeviceIndex"].as_int64();
		const char* deviceName = obj["DeviceName"].as_string().c_str();
		if (boost::json::value* motors = obj["DeviceMessages"].as_object().if_contains("ScalarCmd"); motors != nullptr) {
			for (int i = 0; i < motors->as_array().size(); i++) {
				boost::json::object motorObj = motors->as_array()[i].as_object();
				boost::json::value actType = motorObj["ActuatorType"];
				boost::json::string str = actType.get_string();
				
				this->Motors.push_back(DeviceMotor(deviceIdx, i, deviceName, std::string(str.c_str()))); // erm i think i need to copy value so to std::string it is!!!
			}
		}
	}
}

void CButtplug::FireGameEvent(FNV1A_t uNameHash, CGameEvent* pEvent) {

	if (uNameHash == FNV1A::HashConst("player_death")) {
		const int attacker = Utils::GetPlayerForUserID(pEvent->GetInt("attacker"));
		if (attacker != I::EngineClient->GetLocalPlayer()) {
			return;
		}
		for (DeviceMotor m : this->Motors) {
			std::thread mThred (&DeviceMotor::ActivateMode, m, 0);
			mThred.detach();
		}
	}
}