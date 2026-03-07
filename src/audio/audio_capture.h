#ifndef AUDIO_CAPTURE
#define AUDIO_CAPTURE

#include <miniaudio/miniaudio.h>

ma_device_config capture_config_init(ma_format format, ma_uint32 channles,
                                     ma_uint32 sample_rate,
                                     ma_device_data_proc data_callback,
                                     void *user_data);
ma_result capture_init(ma_context *p_context, const ma_device_config *p_config,
                       ma_device *p_device);
ma_result capture_start(ma_device *p_device);

void capture_uninit(ma_device *p_device);
#endif // AUDIO_CAPTURE
