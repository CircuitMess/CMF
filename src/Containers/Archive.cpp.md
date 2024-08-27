## Table of Contents 
* [Archive Class](#archive-class)
  * [Constructors](#constructors)
  * [Methods](#methods)
* [InArchive Class](#inarchive-class)
  * [Constructors](#inarchive-constructors)
  * [Methods](#inarchive-methods)
* [OutArchive Class](#outarchive-class)
  * [Constructors](#outarchive-constructors)
  * [Methods](#outarchive-methods)

## Archive Class
The `Archive` class is designed to act as a generic container for binary data. It provides a common interface for both writing and reading data, allowing for serialization and deserialization of various data types.

### Constructors
| Constructor | Description |
|---|---|
| `Archive(const std::vector<uint8_t>& queue) noexcept` | Constructs an `Archive` object from a constant vector of bytes.  The data is copied into the internal queue.  |
| `Archive(const std::queue<uint8_t>& queue) noexcept` | Constructs an `Archive` object from a constant queue of bytes.  The data is copied into the internal queue. |
| `Archive(std::queue<uint8_t>&& queue) noexcept` | Constructs an `Archive` object from a queue of bytes, moving the data into the internal queue.  The input queue is left empty. |

### Methods
| Method | Description |
|---|---|
| `void toByteArray(std::vector<uint8_t>& buffer) const noexcept` | Converts the internal data queue to a vector of bytes. The data is copied to the provided buffer. |
| `void pushDataPoint(uint8_t data) noexcept` | Adds a single byte to the end of the data queue. |
| `bool popDataPoint(uint8_t& data) noexcept` | Removes and returns the first byte from the data queue. Returns `true` if successful, `false` if the queue is empty. |


## InArchive Class
The `InArchive` class inherits from the `Archive` class and provides methods for writing data to the archive. It utilizes operator overloading to simplify the writing process.

### Constructors
| Constructor | Description |
|---|---|
| `InArchive(const std::vector<uint8_t>& queue) noexcept` | Constructs an `InArchive` object from a constant vector of bytes.  The data is copied into the internal queue.  |
| `InArchive(const std::queue<uint8_t>& queue) noexcept` | Constructs an `InArchive` object from a constant queue of bytes.  The data is copied into the internal queue. |
| `InArchive(std::queue<uint8_t>&& queue) noexcept` | Constructs an `InArchive` object from a queue of bytes, moving the data into the internal queue.  The input queue is left empty. |

### Methods
| Method | Description |
|---|---|
| `Archive& operator << (bool& data) noexcept` | Writes the value of `data` to the archive.  |
| `Archive& operator << (wchar_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint8_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint16_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint32_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint64_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int8_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int16_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int32_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int64_t& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (float& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (double& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (long double& data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (std::string& data) noexcept` | Writes the value of `data` to the archive.  The string length is first written, followed by the individual characters. |
| `Archive& operator<<(std::wstring& data) noexcept` | Writes the value of `data` to the archive.  The string length is first written, followed by the individual characters. |
| `Archive& operator << (std::vector<bool>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual boolean values. |
| `Archive& operator << (std::vector<wchar_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual characters. |
| `Archive& operator << (std::vector<uint8_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual bytes. |
| `Archive& operator << (std::vector<uint16_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<uint32_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<uint64_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<int8_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<int16_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<int32_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<int64_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<float>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<double>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<long double>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (std::vector<std::string>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual strings. |
| `Archive& operator<<(std::vector<std::wstring>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual strings. |
| `Archive& operator << (bool data) noexcept` | Writes the value of `data` to the archive.  |
| `Archive& operator << (wchar_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint8_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint16_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint32_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (uint64_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int8_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int16_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int32_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (int64_t data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (float data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (double data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (long double data) noexcept` | Writes the value of `data` to the archive. |
| `Archive& operator << (const std::string& data) noexcept` | Writes the value of `data` to the archive.  The string length is first written, followed by the individual characters. |
| `Archive& operator << (const std::wstring& data) noexcept` | Writes the value of `data` to the archive.  The string length is first written, followed by the individual characters. |
| `Archive& operator << (const std::vector<bool>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual boolean values. |
| `Archive& operator << (const std::vector<wchar_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual characters. |
| `Archive& operator << (const std::vector<uint8_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual bytes. |
| `Archive& operator << (const std::vector<uint16_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<uint32_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<uint64_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<int8_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<int16_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<int32_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<int64_t>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<float>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<double>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<long double>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual values. |
| `Archive& operator << (const std::vector<std::string>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual strings. |
| `Archive& operator << (const std::vector<std::wstring>& data) noexcept` | Writes the value of `data` to the archive.  The vector size is first written, followed by the individual strings. |

## OutArchive Class
The `OutArchive` class inherits from the `Archive` class and provides methods for reading data from the archive. It utilizes operator overloading to simplify the reading process.

### Constructors
| Constructor | Description |
|---|---|
| `OutArchive(const std::vector<uint8_t>& queue) noexcept` | Constructs an `OutArchive` object from a constant vector of bytes.  The data is copied into the internal queue.  |
| `OutArchive(const std::queue<uint8_t>& queue) noexcept` | Constructs an `OutArchive` object from a constant queue of bytes.  The data is copied into the internal queue. |
| `OutArchive(std::queue<uint8_t>&& queue) noexcept` | Constructs an `OutArchive` object from a queue of bytes, moving the data into the internal queue.  The input queue is left empty. |

### Methods
| Method | Description |
|---|---|
| `Archive& operator << (bool& data) noexcept` | Reads the value of `data` from the archive.  |
| `Archive& operator << (wchar_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (uint8_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (uint16_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (uint32_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (uint64_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (int8_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (int16_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (int32_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (int64_t& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (float& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (double& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (long double& data) noexcept` | Reads the value of `data` from the archive. |
| `Archive& operator << (std::string& data) noexcept` | Reads the value of `data` from the archive.  The string length is first read, followed by the individual characters. |
| `Archive& operator<<(std::wstring& data) noexcept` | Reads the value of `data` from the archive.  The string length is first read, followed by the individual characters. |
| `Archive& operator << (std::vector<bool>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual boolean values. |
| `Archive& operator << (std::vector<wchar_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual characters. |
| `Archive& operator << (std::vector<uint8_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual bytes. |
| `Archive& operator << (std::vector<uint16_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<uint32_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<uint64_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<int8_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<int16_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<int32_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<int64_t>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<float>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<double>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<long double>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual values. |
| `Archive& operator << (std::vector<std::string>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual strings. |
| `Archive& operator<<(std::vector<std::wstring>& data) noexcept` | Reads the value of `data` from the archive.  The vector size is first read, followed by the individual strings. |