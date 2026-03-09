#define MINIAUDIO_IMPLEMENTATION
#include "audio/audio_capture.h"
#include "speech/speech_recognition.h"
#include <stdio.h>

#define SAMPLE_RATE 16000

// Callback function
void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  SpeechContext *rec = (SpeechContext *)pDevice->pUserData;
  // 16-bit mono → 2 bytes per frame
  int bytes = frameCount * 2;

  if (speech_process_audio(rec, pInput, bytes)) {
    printf("%s\n", speech_get_result(rec));
  } else {
    printf("%s\n", speech_get_partial(rec));
  }

  (void)pOutput; // unused
}

// Implemention i/o audio
int main(int argc, char *argv[]) {
  // Paths
  char *model_path = "src/model";

  // Variables for speech recognition
  SpeechContext *speech_ctx = speech_init(model_path, SAMPLE_RATE);

  // Variables for miniaudio
  ma_result result;
  ma_device device;

  // Check cotext is loaded
  if (speech_ctx->model == NULL) {
    fprintf(stderr,
            "ERROR: could not load model or recognizer not initialized.\n");
    return -1;
  }
  ma_device_config device_config = capture_config_init(
      ma_format_s16, 1, SAMPLE_RATE, data_callback, speech_ctx);

  result = capture_init(NULL, &device_config, &device);

  // Check device init
  if (result != MA_SUCCESS) {
    fprintf(stderr, "ERROR: could  not open capture device. Code: %i\n",
            result);
    return result;
  }

  printf("Press Space to stop...");

  // Check device is started
  result = capture_start(&device);
  if (result != MA_SUCCESS) {
    capture_uninit(&device);
    fprintf(stderr, "ERROR: could not start capture device. Code: %i\n",
            result);
    return result;
  }
  getchar();

  capture_uninit(&device);
  speech_free(speech_ctx);
  return 0;
}
