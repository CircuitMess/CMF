#include "Archive.hpp"

void Archive::pushData(uint8_t data) noexcept {
	byteBuffer.push(data);
}

bool Archive::popData(uint8_t& data) noexcept {
	if(byteBuffer.empty()){
		return false;
	}

	data = byteBuffer.front();
	byteBuffer.pop();

	return true;
}

virtual Archive& InArchive::operator << (bool& data) noexcept {

}

virtual Archive& InArchive::operator << (wchar_t& data) noexcept {

}

virtual Archive& InArchive::operator << (uint8_t& data) noexcept {

}

virtual Archive& InArchive::operator << (uint16_t& data) noexcept {

}

virtual Archive& InArchive::operator << (uint32_t& data) noexcept {

}

virtual Archive& InArchive::operator << (uint64_t& data) noexcept {

}

virtual Archive& InArchive::operator << (int8_t& data) noexcept {

}

virtual Archive& InArchive::operator << (int16_t& data) noexcept {

}

virtual Archive& InArchive::operator << (int32_t& data) noexcept {

}

virtual Archive& InArchive::operator << (int64_t& data) noexcept {

}

virtual Archive& InArchive::operator << (float& data) noexcept {

}

virtual Archive& InArchive::operator << (double& data) noexcept {

}

virtual Archive& InArchive::operator << (long double& data) noexcept {

}

virtual Archive& InArchive::operator << (std::string& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<bool>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<wchar_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<uint8_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<uint16_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<uint32_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<uint64_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<int8_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<int16_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<int32_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<int64_t>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<float>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<double>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<long double>& data) noexcept {

}

virtual Archive& InArchive::operator << (std::vector<std::string>& data) noexcept {

}

virtual Archive& OutArchive::operator << (bool& data) noexcept {

}

virtual Archive& OutArchive::operator << (wchar_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (uint8_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (uint16_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (uint32_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (uint64_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (int8_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (int16_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (int32_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (int64_t& data) noexcept {

}

virtual Archive& OutArchive::operator << (float& data) noexcept {

}

virtual Archive& OutArchive::operator << (double& data) noexcept {

}

virtual Archive& OutArchive::operator << (long double& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::string& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<bool>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<wchar_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<uint8_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<uint16_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<uint32_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<uint64_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<int8_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<int16_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<int32_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<int64_t>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<float>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<double>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<long double>& data) noexcept {

}

virtual Archive& OutArchive::operator << (std::vector<std::string>& data) noexcept {

}
