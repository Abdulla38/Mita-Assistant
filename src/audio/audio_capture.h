#ifndef AUDIO_CAPTURE
#define AUDIO_CAPTURE

#include <miniaudio/miniaudio.h>

// Class AudioCapture
typedef struct {
  ma_device device;
  int status;
} AudioCapture;

// Create object
AudioCapture *capture_create(ma_format format, ma_uint32 channles,
                             ma_uint32 sample_rate,
                             ma_device_data_proc data_callback,
                             void *user_data);

// Methods
int capture_start(AudioCapture *ctx);
int capture_stop(AudioCapture *ctx);
void capture_uninit(AudioCapture *ctx);
#endif // AUDIO_CAPTURE
