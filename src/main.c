#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>
#include <stdio.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 1024

// Callback function
void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  ma_encoder_write_pcm_frames((ma_encoder *)pDevice->pUserData, pInput,
                              frameCount, NULL);
  (void)pOutput;
}

// Implemention i/o audio
int main(int argc, char *argv[]) {
  // Init main variables
  ma_result result;
  ma_encoder encoder;
  ma_encoder_config encoder_config;
  ma_device device;

  char *output_path = "output.wav";

  encoder_config = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32,
                                          2, SAMPLE_RATE);
  result = ma_encoder_init_file(output_path, &encoder_config, &encoder);

  if (result != MA_SUCCESS) {
    fprintf(stderr, "ERROR: could not initialize output file Code: %i.\n",
            result);
    return result;
  }

  ma_device_config device_config =
      ma_device_config_init(ma_device_type_capture);

  device_config.capture.format = encoder.config.format;
  device_config.capture.channels = encoder.config.channels;
  device_config.sampleRate = SAMPLE_RATE;
  device_config.dataCallback = data_callback;
  device_config.pUserData = &encoder;

  result = ma_device_init(NULL, &device_config, &device);

  // Check device init
  if (result != MA_SUCCESS) {
    fprintf(stderr, "ERROR: could  not open capture device. Code: %i\n",
            result);
    return result;
  }

  // Check device is started
  result = ma_device_start(&device);
  if (result != MA_SUCCESS) {
    ma_device_uninit(&device);
    fprintf(stderr, "ERROR: could not start capture device. Code: %i\n",
            result);
    return result;
  }

  printf("Press Enter to stop recording...\n");
  getchar();

  ma_device_uninit(&device);
  ma_encoder_uninit(&encoder);
  return 0;
}
