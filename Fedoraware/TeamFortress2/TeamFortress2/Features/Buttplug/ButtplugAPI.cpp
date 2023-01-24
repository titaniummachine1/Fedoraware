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
	payload["MessageVersion"] = 3;
	this->DoRequest("RequestServerInfo", payload);
}

void CButtplug::Ping() {
	boost::json::object payload;
	this->DoRequest("Ping", payload);
}

void CButtplug::StartScanning() {
	boost::json::object payload;
	this->DoRequest("StartScanning", payload);
}

void CButtplug::StopScanning() {
	boost::json::object payload;
	this->DoRequest("StopScanning", payload);
}

void CButtplug::StopAllDevices() {
	boost::json::object payload;
	this->DoRequest("StopAllDevices", payload);
}

void CButtplug::ScalarCmd(int deviceIdx, int motorIdx, float scalar, const char* actType) {
	boost::json::object payload;
	payload["DeviceIndex"] = deviceIdx;
	boost::json::array scArray = boost::json::array();
	scArray.push_back({
		{"Index", motorIdx},
		{"Scalar",scalar},
		{"ActuatorType", actType}
	});
	payload["Scalars"] = scArray;
	this->DoRequest("ScalarCmd", payload);
}


void CButtplug::RequestDeviceList() {
	boost::json::object payload;
	this->DoRequest("RequestDeviceList", payload);
}

void CButtplug::StopDeviceCmd(int deviceIdx)
{
	boost::json::object payload;
	payload["DeviceIndex"] = deviceIdx;
	this->DoRequest("StopDeviceCmd", payload);
}
