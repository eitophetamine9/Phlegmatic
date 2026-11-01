#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace SaveHelpers {

    inline std::string Trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n\"");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n\"");
        return str.substr(first, (last - first + 1));
    }

    inline int SafeParseInt(const std::string& str, int defaultValue = 0) {
        std::string cleaned = Trim(str);
        if (cleaned.empty()) return defaultValue;
        try {
            return std::stoi(cleaned);
        } catch (...) {
            return defaultValue;
        }
    }

    inline float SafeParseFloat(const std::string& str, float defaultValue = 0.0f) {
        std::string cleaned = Trim(str);
        if (cleaned.empty()) return defaultValue;
        try {
            return std::stof(cleaned);
        } catch (...) {
            return defaultValue;
        }
    }

    inline bool SafeParseBool(const std::string& str, bool defaultValue = false) {
        std::string cleaned = Trim(str);
        if (cleaned == "true" || cleaned == "1") return true;
        if (cleaned == "false" || cleaned == "0") return false;
        return defaultValue;
    }

    inline std::string ExtractStringValue(const std::string& line) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) return "";
        size_t firstQuote = line.find('"', colon);
        if (firstQuote == std::string::npos) return "";
        size_t secondQuote = line.find('"', firstQuote + 1);
        if (secondQuote == std::string::npos) return "";
        return line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
    }

    inline int ExtractIntValue(const std::string& line, int defaultValue = 0) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) return defaultValue;
        std::string valStr = line.substr(colon + 1);
        size_t comma = valStr.find(',');
        if (comma != std::string::npos) valStr = valStr.substr(0, comma);
        return SafeParseInt(valStr, defaultValue);
    }

    inline float ExtractFloatValue(const std::string& line, float defaultValue = 0.0f) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) return defaultValue;
        std::string valStr = line.substr(colon + 1);
        size_t comma = valStr.find(',');
        if (comma != std::string::npos) valStr = valStr.substr(0, comma);
        return SafeParseFloat(valStr, defaultValue);
    }

    inline bool ExtractBoolValue(const std::string& line, bool defaultValue = false) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) return defaultValue;
        std::string valStr = line.substr(colon + 1);
        size_t comma = valStr.find(',');
        if (comma != std::string::npos) valStr = valStr.substr(0, comma);
        return SafeParseBool(valStr, defaultValue);
    }

    // Safely parse a JSON array line: e.g. "unlockedWeapons": [true, false, ...] into an array
    template <typename T, size_t N, typename ParserFunc>
    inline void ParseJsonArray(const std::string& line, T (&outArray)[N], ParserFunc parser) {
        size_t openBracket = line.find('[');
        size_t closeBracket = line.find(']');
        if (openBracket == std::string::npos || closeBracket == std::string::npos || closeBracket <= openBracket) {
            return;
        }
        std::string inner = line.substr(openBracket + 1, closeBracket - openBracket - 1);
        std::stringstream ss(inner);
        std::string token;
        size_t idx = 0;
        while (std::getline(ss, token, ',') && idx < N) {
            outArray[idx] = parser(token);
            idx++;
        }
    }
}

