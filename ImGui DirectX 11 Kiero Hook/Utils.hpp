#pragma once
#include <numbers>
#include <cmath>
#include <WinUser.h>
#include "Utils.hpp"
#include <ostream>
#include <cstdint>
#include <cstddef>
#include <map>
#include <string>
#include "SCPSL/Math/math.h"

namespace Utils {
		bool startsWith(const std::string& str, const std::string& prefix) {
			return str.compare(0, prefix.length(), prefix) == 0;
		}
	bool endsWith(const std::string& str, const std::string& suffix) {
		if (str.length() < suffix.length()) {
			return false;
		}
		return str.substr(str.length() - suffix.length()) == suffix;
	}
	std::string FtoStr(float value) {
		std::string strValue = std::to_string(value);
		size_t dotIndex = strValue.find('.');
		if (dotIndex != std::string::npos) {
			strValue.erase(dotIndex + 2, std::string::npos);
		}
		return strValue;
	}
	bool Vec2IsLess(Vector2 first, Vector2 secound) {
		if (first.x >= secound.x || first.y >= secound.y)
			return true;
		return false;
	}
	bool Vec2IsGreater(Vector2 first, Vector2 secound) {
		if (first.x <= secound.x || first.y <= secound.y)
			return true;
		return false;
	}
	bool IsFocused() {
		return true;
	}
}