#pragma once

#include <boost/json.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <bitset>
#include "../../SDK.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;


class DeviceMotor;
class CButtplug {
private:
	client wsClient;
	websocketpp::connection_hdl hdl;
	std::thread t;
	
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	void on_open(websocketpp::connection_hdl hdl);

	int msgId = 1;

	void DoRequest(std::string, boost::json::object);
	void PopulateMotors(boost::json::array devices);
public:
	std::vector<DeviceMotor> Motors;
	void Ping();
	void RequestServerInfo();
	void StartScanning();
	void StopScanning();
	void RequestDeviceList();
	void StopDeviceCmd(int deviceIdx);
	void StopAllDevices();
	void ScalarCmd(int deviceIdx, int motorIdx, float scalar, const char* actType);
	//void LinearCmd(int deviceIdx, std::vector<ButtPlug::BPLinear> vectors); These seem like pain in the ass rn someone else can do them :D
	//void RotateCmd(int deviceIdx, std::vector<ButtPlug::BPRotate> rotations);
	void Init();
	void Shutdown();
	void FireGameEvent(FNV1A_t uNameHash, CGameEvent* pEvent);

};

ADD_FEATURE(CButtplug, Buttplug)

class DeviceMotor {
private:
	struct Mode {
		int duration = 500; //milliseconds
		float intensity = 1.f;
		bool enabled = true;
	};
	int DeviceID;
	int MotorID;
	const char* DeviceName;
	std::string ActType;
	Mode modes[4]; // 0: death, 1: damageDone, 2: damageTaken
	float totIntensity;

	

public:
	DeviceMotor(int dID, int mID, const char* dName, std::string aType) : DeviceID(dID), MotorID(mID), DeviceName(dName), ActType(aType) {
	};

	void ActivateMode(int mode) {
		if (!modes[mode].enabled)
			return;

		totIntensity += modes[mode].intensity;
		if (totIntensity > 1)
			totIntensity = 1;
		F::Buttplug.ScalarCmd(this->DeviceID, this->MotorID, totIntensity, this->ActType.c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(modes[mode].duration));
		// erm it might change so i cant just like set stuff 
		totIntensity -= modes[mode].intensity;
		if (totIntensity < 0)
			totIntensity = 0;
		F::Buttplug.ScalarCmd(this->DeviceID, this->MotorID, totIntensity, this->ActType.c_str());
	}
};