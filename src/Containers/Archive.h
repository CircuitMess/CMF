#ifndef CMF_ARCHIVE_H
#define CMF_ARCHIVE_H

#include <queue>
#include <string>

class Archive {
public:
	Archive(const std::vector<uint8_t>& queue) noexcept;
	Archive(const std::queue<uint8_t>& queue) noexcept;
	Archive(std::queue<uint8_t>&& queue) noexcept;

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
	virtual Archive& operator << (std::wstring& data) noexcept = 0;
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
	virtual Archive& operator << (std::vector<std::wstring>& data) noexcept = 0;

protected:
	void pushDataPoint(uint8_t data) noexcept;
	bool popDataPoint(uint8_t& data) noexcept;

	template<typename T>
	void pushData(T data) noexcept = delete;

	template<typename T>
	void pushData(T data) noexcept requires (std::is_fundamental<T>::value) {
		uint8_t* dataAsBuffer = reinterpret_cast<uint8_t*>(&data);
		if(dataAsBuffer == nullptr){
			return;
		}

		for(size_t i = 0; i < sizeof(T); ++i){
			pushDataPoint(dataAsBuffer[i]);
		}
	}

	template<typename T>
	bool popData(T& data) noexcept = delete;

	template<typename T>
	bool popData(T& data) noexcept requires (std::is_fundamental<T>::value) {
		uint8_t* dataAsBuffer = reinterpret_cast<uint8_t*>(&data);
		if(dataAsBuffer == nullptr){
			return false;
		}

		for(size_t i = 0; i < sizeof(T); ++i){
			if(!popDataPoint(dataAsBuffer[i])){
				return false;
			}
		}

		return true;
	}

private:
	std::queue<uint8_t> byteBuffer;
};

class InArchive : public Archive {
public:
	InArchive(const std::vector<uint8_t>& queue) noexcept;
	InArchive(const std::queue<uint8_t>& queue) noexcept;
	InArchive(std::queue<uint8_t>&& queue) noexcept;

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
	virtual Archive& operator << (std::wstring& data) noexcept override;
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
	virtual Archive& operator << (std::vector<std::wstring>& data) noexcept override;
};

class OutArchive : public Archive {
public:
	OutArchive(const std::vector<uint8_t>& queue) noexcept;
	OutArchive(const std::queue<uint8_t>& queue) noexcept;
	OutArchive(std::queue<uint8_t>&& queue) noexcept;

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
	virtual Archive& operator << (std::wstring& data) noexcept override;
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
	virtual Archive& operator << (std::vector<std::wstring>& data) noexcept override;
};

#endif //CMF_ARCHIVE_H