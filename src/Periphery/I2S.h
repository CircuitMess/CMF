#ifndef CMF_TEMPLATE_I2S_H
#define CMF_TEMPLATE_I2S_H

#include "Object/Object.h"
#include <driver/i2s_std.h>

class I2S : public Object {
	GENERATED_BODY(I2S, Object)

public:
	I2S(i2s_port_t port = I2S_NUM_AUTO, i2s_std_config_t config = {});

	~I2S() override;

	/**
	 * Write data to I2S channel.
	 * @param data data pointer
	 * @param size number of bytes to be written
	 * @return number of bytes successfully written
	 */
	size_t write(uint8_t* data, size_t bytes);

private:
	i2s_chan_handle_t tx_chan;
};


#endif //CMF_TEMPLATE_I2S_H
