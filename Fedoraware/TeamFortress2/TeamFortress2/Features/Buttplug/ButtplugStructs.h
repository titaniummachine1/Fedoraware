#pragma once
#include <string>
#include <boost/json.hpp>
namespace ButtPlug {
	struct BPScalar {

		int index;
		float scalar;
		const char* actuatorType;

		BPScalar(int i, float s, const char* a) : index(i), scalar(s), actuatorType(a) {};
	};

	struct BPLinear {
		int index;
		int duration;
		float position;

		BPLinear(int i, int d, float p) : index(i), duration(d), position(p) {};
	};

	struct BPRotate {
		int index;
		float speed;
		bool clockwise;

		BPRotate(int i, float s, bool c) : index(i), speed(s), clockwise(c) {};
	};
};