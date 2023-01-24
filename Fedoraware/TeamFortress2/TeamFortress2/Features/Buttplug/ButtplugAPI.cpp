#include "Buttplug.h"
#include <boost/json.hpp>

void CButtplug::DoRequest(std::string msgType, boost::json::object payload) {
	boost::json::array json;
	payload["Id"] = this->msgId;
	json.push_back(boost::json::object{ {msgType,payload} });
	this->wsClient.send(this->hdl, boost::json::serialize(json), websocketpp::frame::opcode::text);
	this->msgId++;
}

void CButtplug::RequestServerInfo() {
	boost::json::object payload;
	payload["ClientName"] = "Fedoraware";
	payload["MessageVersion"] = 2;
	this->DoRequest("RequestServerInfo", payload);
}

void CButtplug::Ping() {
	boost::json::object payload;
	this->DoRequest("Ping", payload);
}

void CButtplug::ScalarCmd(int deviceIdx, std::vector<ButtPlug::BPScalar> scalars) {
	boost::json::object payload;
	payload["DeviceIndex"] = deviceIdx;
	boost::json::array scArray = boost::json::array();
	for (ButtPlug::BPScalar sc : scalars) {
		scArray.push_back({
			{"Index", sc.index},
			{"Scalar",sc.scalar},
			{"ActuatorType", sc.actuatorType}	
		});
	}
	payload["Scalars"] = scArray;
	this->DoRequest("ScalarCmd", payload);
}

void CButtplug::RequestDeviceList() {
	boost::json::object payload;
	this->DoRequest("RequestDeviceList", payload);
}