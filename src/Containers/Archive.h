#ifndef CMF_ARCHIVE_H
#define CMF_ARCHIVE_H

#include <queue>
#include <string>

/**
 * @brief
 */
class Archive {
public:
	/**
	 * @brief 
	 */
	Archive() noexcept = default;

	/**
	 * @brief 
	 * @param queue 
	 */
	Archive(const std::vector<uint8_t>& queue) noexcept;

	/**
	 * @brief 
	 * @param queue 
	 */
	Archive(const std::queue<uint8_t>& queue) noexcept;

	/**
	 * @brief 
	 * @param queue 
	 */
	Archive(std::queue<uint8_t>&& queue) noexcept;

	/**
	 * @brief 
	 */
	virtual ~Archive() noexcept = default;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (bool& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (wchar_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint8_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint16_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint32_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint64_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int8_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int16_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int32_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int64_t& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (float& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (double& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (long double& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::string& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::wstring& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<bool>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<wchar_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint8_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint16_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint32_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint64_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int8_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int16_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int32_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int64_t>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<float>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<double>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<long double>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<std::string>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<std::wstring>& data) noexcept = 0;

	/**
	 * @brief 
	 * @param buffer 
	 */
	void toByteArray(std::vector<uint8_t>& buffer) const noexcept;

protected:
	/**
	 * @brief 
	 * @param data 
	 */
	void pushDataPoint(uint8_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	bool popDataPoint(uint8_t& data) noexcept;

	/**
	 * @brief 
	 * @tparam T 
	 * @param data 
	 */
	template<typename T>
	void pushData(T data) noexcept = delete;

	/**
	 * @brief 
	 * @tparam T 
	 * @param data 
	 */
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

	/**
	 * @brief 
	 * @tparam T 
	 * @param data 
	 * @return 
	 */
	template<typename T>
	bool popData(T& data) noexcept = delete;

	/**
	 * @brief 
	 * @tparam T 
	 * @param data 
	 * @return 
	 */
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

/**
 * @brief 
 */
class InArchive : public Archive {
public:
	/**
	 * @brief 
	 */
	InArchive() noexcept = default;

	/**
	 * @brief 
	 * @param queue 
	 */
	InArchive(const std::vector<uint8_t>& queue) noexcept;

	/**
	 * @brief 
	 * @param queue 
	 */
	InArchive(const std::queue<uint8_t>& queue) noexcept;

	/**
	 * @brief 
	 * @param queue 
	 */
	InArchive(std::queue<uint8_t>&& queue) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (bool& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (wchar_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint8_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint16_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint32_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint64_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int8_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int16_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int32_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int64_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (float& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (double& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (long double& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::string& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::wstring& data) noexcept override;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<bool>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<wchar_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint8_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint16_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint32_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint64_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int8_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int16_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int32_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int64_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<float>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<double>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<long double>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<std::string>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<std::wstring>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (bool data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (wchar_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint8_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint16_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint32_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint64_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int8_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int16_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int32_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int64_t data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (float data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (double data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (long double data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::string& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::wstring& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<bool>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<wchar_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<uint8_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<uint16_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<uint32_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<uint64_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<int8_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<int16_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<int32_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<int64_t>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<float>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<double>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<long double>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<std::string>& data) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (const std::vector<std::wstring>& data) noexcept;
};

/**
 * @brief 
 */
class OutArchive : public Archive {
public:
	/**
	 * @brief 
	 */
	OutArchive() noexcept = default;

	/**
	 * @brief 
	 * @param queue 
	 */
	OutArchive(const std::vector<uint8_t>& queue) noexcept;

	/**
	 * @brief 
	 * @param queue 
	 */
	OutArchive(const std::queue<uint8_t>& queue) noexcept;

	/**
	 * @brief 
	 * @param queue 
	 */
	OutArchive(std::queue<uint8_t>&& queue) noexcept;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (bool& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (wchar_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint8_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint16_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint32_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (uint64_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int8_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int16_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int32_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (int64_t& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (float& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (double& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (long double& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::string& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::wstring& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<bool>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<wchar_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint8_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint16_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint32_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<uint64_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int8_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int16_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int32_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<int64_t>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<float>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<double>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<long double>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<std::string>& data) noexcept override final;

	/**
	 * @brief 
	 * @param data 
	 * @return 
	 */
	virtual Archive& operator << (std::vector<std::wstring>& data) noexcept override final;
};

#endif //CMF_ARCHIVE_H