#include "Buttplug.h"

void CButtplug::RequestServerInfo() {
	rapidjson::Document json;
	json.SetArray();
	rapidjson::Document::AllocatorType& allocator = json.GetAllocator();

	rapidjson::Value cmd;
	cmd.SetObject();
	rapidjson::Value payload;
	payload.SetObject();
	payload.AddMember("Id", this->msgId, allocator);
	payload.AddMember("ClientName", rapidjson::StringRef(Vars::Menu::CheatName.c_str()), allocator);
	payload.AddMember("MessageVersion", 1, allocator);

	cmd.AddMember("RequestServerInfo", payload, allocator);
	json.PushBack(cmd, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	json.Accept(writer);

	const char* jsonString = strbuf.GetString();
	this->wsClient.send(this->hdl, jsonString, websocketpp::frame::opcode::text);
}

void CButtplug::ScalarCmd(int deviceIdx, std::vector<BPScalar> scalars) {
	rapidjson::Document json;
	json.SetArray();
	rapidjson::Document::AllocatorType& allocator = json.GetAllocator();
	
	rapidjson::Value cmd;
	cmd.SetObject();
	rapidjson::Value payload;
	payload.SetObject();
	payload.AddMember("Id", this->msgId, allocator);
	payload.AddMember("DeviceIndex", deviceIdx, allocator);

	rapidjson::Value scalarList;
	scalarList.SetArray();
	
	for (BPScalar s :  scalars) {
		rapidjson::Value scalar;
		scalar.SetObject();
		scalar.AddMember("Index", rapidjson::Value().SetInt(s.index), allocator);
		scalar.AddMember("Scalar", rapidjson::Value().SetFloat(s.scalar), allocator);
		scalar.AddMember("ActuatorType", rapidjson::Value().SetString(rapidjson::StringRef(s.actuatorType)), allocator);

		scalarList.PushBack(scalar, allocator);
	};
	payload.AddMember("Scalars", scalarList, allocator);
	cmd.AddMember("ScalarCmd", payload, allocator);
	json.PushBack(cmd, allocator);
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	json.Accept(writer);

	const char* jsonString = strbuf.GetString();
	this->wsClient.send(this->hdl, jsonString, websocketpp::frame::opcode::text);
}