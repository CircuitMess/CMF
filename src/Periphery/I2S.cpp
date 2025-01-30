#include "I2S.h"

I2S::I2S(i2s_port_t port, i2s_std_config_t config){
	i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(port, I2S_ROLE_MASTER);
	i2s_new_channel(&chan_cfg, &tx_chan, nullptr);

	ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan, &config));

	i2s_channel_enable(tx_chan);
}

I2S::~I2S(){
	i2s_channel_disable(tx_chan);
}

size_t I2S::write(uint8_t* data, size_t bytes){
	size_t bytesWritten = 0;
	ESP_ERROR_CHECK(i2s_channel_write(tx_chan, data, bytes, &bytesWritten, portMAX_DELAY));

	return bytesWritten;
}
