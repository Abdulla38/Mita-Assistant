#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>
#include <stdio.h>
#include <vosk_api.h>

#define SAMPLE_RATE 16000

// Callback function
void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  // ma_encoder_write_pcm_frames((ma_encoder *)pDevice->pUserData, pInput,
  // frameCount, NULL);
  VoskRecognizer *rec = (VoskRecognizer *)pDevice->pUserData;
  // 16-bit mono → 2 bytes per frame
  int bytes = frameCount * 2;

  if (vosk_recognizer_accept_waveform(rec, pInput, bytes)) {
    printf("%s\n", vosk_recognizer_result(rec));
  } else {
    printf("%s\n", vosk_recognizer_partial_result(rec));
  }

  (void)pOutput; // unused
}

// Implemention i/o audio
int main(int argc, char *argv[]) {
  // Paths
  char *model_path = "src/model";
  char *output_path = "output.wav";

  // Variables for miniaudio
  ma_result result;
  // ma_encoder encoder;
  // ma_encoder_config encoder_config;
  ma_device device;

  // Variables for vosk
  VoskModel *model = vosk_model_new(model_path);
  VoskRecognizer *recognizer = vosk_recognizer_new(model, SAMPLE_RATE);

  // Check model isn't loaded
  if (model == NULL) {
    fprintf(stderr, "ERROR: could not load model data\n");
    return -1;
  }

  // Check recognizer isn't created
  if (recognizer == NULL) {
    fprintf(stderr, "ERROR: could not create recognizer\n");
    return -2;
  }

  // encoder_config = ma_encoder_config_init(ma_encoding_format_wav,
  // ma_format_s16,                                     1, SAMPLE_RATE); result
  // = ma_encoder_init_file(output_path, &encoder_config, &encoder);

  // if (result != MA_SUCCESS) {
  //   fprintf(stderr, "ERROR: could not initialize output file Code: %i.\n",
  //           result);
  //   return result;
  // }

  ma_device_config device_config =
      ma_device_config_init(ma_device_type_capture);

  device_config.capture.format = ma_format_s16;
  device_config.capture.channels = 1;
  device_config.sampleRate = SAMPLE_RATE;
  device_config.dataCallback = data_callback;
  device_config.pUserData = recognizer;

  result = ma_device_init(NULL, &device_config, &device);

  // Check device init
  if (result != MA_SUCCESS) {
    fprintf(stderr, "ERROR: could  not open capture device. Code: %i\n",
            result);
    return result;
  }

  printf("Press Space to stop...");

  // Check device is started
  result = ma_device_start(&device);
  if (result != MA_SUCCESS) {
    ma_device_uninit(&device);
    fprintf(stderr, "ERROR: could not start capture device. Code: %i\n",
            result);
    return result;
  }
  getchar();

  ma_device_uninit(&device);
  printf("%s\n", vosk_recognizer_final_result(recognizer));
  vosk_recognizer_free(recognizer);
  vosk_model_free(model);
  // ma_encoder_uninit(&encoder);
  return 0;
}
