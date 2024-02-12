#ifndef CMF_ARCHIVE_HPP
#define CMF_ARCHIVE_HPP

#include <queue>
#include <string>

class Archive {
public:
	virtual Archive& operator << (bool& data) noexcept = 0;
	virtual Archive& operator << (wchar_t& data) noexcept = 0;
	virtual Archive& operator << (uint8_t& data) noexcept = 0;
	virtual Archive& operator << (uint16_t& data) noexcept = 0;
	virtual Archive& operator << (uint32_t& data) noexcept = 0;
	virtual Archive& operator << (uint64_t& data) noexcept = 0;
	virtual Archive& operator << (int8_t& data) noexcept = 0;
	virtual Archive& operator << (int16_t& data) noexcept = 0;
	virtual Archive& operator << (int32_t& data) noexcept = 0;
	virtual Archive& operator << (int64_t& data) noexcept = 0;
	virtual Archive& operator << (float& data) noexcept = 0;
	virtual Archive& operator << (double& data) noexcept = 0;
	virtual Archive& operator << (long double& data) noexcept = 0;
	virtual Archive& operator << (std::string& data) noexcept = 0;
	virtual Archive& operator << (std::vector<bool>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<wchar_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<uint8_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<uint16_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<uint32_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<uint64_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<int8_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<int16_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<int32_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<int64_t>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<float>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<double>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<long double>& data) noexcept = 0;
	virtual Archive& operator << (std::vector<std::string>& data) noexcept = 0;

protected:
	void pushData(uint8_t data) noexcept;
	bool popData(uint8_t& data) noexcept;

private:
	std::queue<uint8_t> byteBuffer;
};

class InArchive : public Archive {
public:
	virtual Archive& operator << (bool& data) noexcept override;
	virtual Archive& operator << (wchar_t& data) noexcept override;
	virtual Archive& operator << (uint8_t& data) noexcept override;
	virtual Archive& operator << (uint16_t& data) noexcept override;
	virtual Archive& operator << (uint32_t& data) noexcept override;
	virtual Archive& operator << (uint64_t& data) noexcept override;
	virtual Archive& operator << (int8_t& data) noexcept override;
	virtual Archive& operator << (int16_t& data) noexcept override;
	virtual Archive& operator << (int32_t& data) noexcept override;
	virtual Archive& operator << (int64_t& data) noexcept override;
	virtual Archive& operator << (float& data) noexcept override;
	virtual Archive& operator << (double& data) noexcept override;
	virtual Archive& operator << (long double& data) noexcept override;
	virtual Archive& operator << (std::string& data) noexcept override;
	virtual Archive& operator << (std::vector<bool>& data) noexcept override;
	virtual Archive& operator << (std::vector<wchar_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint8_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint16_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint32_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint64_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int8_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int16_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int32_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int64_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<float>& data) noexcept override;
	virtual Archive& operator << (std::vector<double>& data) noexcept override;
	virtual Archive& operator << (std::vector<long double>& data) noexcept override;
	virtual Archive& operator << (std::vector<std::string>& data) noexcept override;
};

class OutArchive : public Archive {
public:
	virtual Archive& operator << (bool& data) noexcept override;
	virtual Archive& operator << (wchar_t& data) noexcept override;
	virtual Archive& operator << (uint8_t& data) noexcept override;
	virtual Archive& operator << (uint16_t& data) noexcept override;
	virtual Archive& operator << (uint32_t& data) noexcept override;
	virtual Archive& operator << (uint64_t& data) noexcept override;
	virtual Archive& operator << (int8_t& data) noexcept override;
	virtual Archive& operator << (int16_t& data) noexcept override;
	virtual Archive& operator << (int32_t& data) noexcept override;
	virtual Archive& operator << (int64_t& data) noexcept override;
	virtual Archive& operator << (float& data) noexcept override;
	virtual Archive& operator << (double& data) noexcept override;
	virtual Archive& operator << (long double& data) noexcept override;
	virtual Archive& operator << (std::string& data) noexcept override;
	virtual Archive& operator << (std::vector<bool>& data) noexcept override;
	virtual Archive& operator << (std::vector<wchar_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint8_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint16_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint32_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<uint64_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int8_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int16_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int32_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<int64_t>& data) noexcept override;
	virtual Archive& operator << (std::vector<float>& data) noexcept override;
	virtual Archive& operator << (std::vector<double>& data) noexcept override;
	virtual Archive& operator << (std::vector<long double>& data) noexcept override;
	virtual Archive& operator << (std::vector<std::string>& data) noexcept override;
};

#endif //CMF_ARCHIVE_HPP