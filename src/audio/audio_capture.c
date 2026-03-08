#include "audio_capture.h"
#include <miniaudio/miniaudio.h>

ma_device_config capture_config_init(ma_format format, ma_uint32 channles,
                                     ma_uint32 sample_rate,
                                     ma_device_data_proc data_callback,
                                     void *user_data) {

  ma_device_config device_config =
      ma_device_config_init(ma_device_type_capture);

  device_config.capture.format = format;
  device_config.capture.channels = channles;
  device_config.sampleRate = sample_rate;
  device_config.dataCallback = data_callback;
  device_config.pUserData = user_data;

  return device_config;
}

ma_result capture_init(ma_context *p_context, const ma_device_config *p_config,
                       ma_device *p_device) {
  ma_result result = ma_device_init(p_context, p_config, p_device);
  return result;
}

ma_result capture_start(ma_device *p_device) {
  ma_result result = ma_device_start(p_device);
  return result;
}

void capture_uninit(ma_device *p_device) { ma_device_uninit(p_device); }
