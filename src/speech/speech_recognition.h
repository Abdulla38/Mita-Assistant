#ifndef SPEECH_RECOGNITION
#define SPEECH_RECOGNITION

#include <vosk_api.h>

typedef struct {
  VoskModel *model;
  VoskRecognizer *recognizer;
} SpeechContext;

SpeechContext *speech_init(const char *model_path);

int speech_process_audio(SpeechContext *ctx, const void *data, int size);

const char *speech_get_result(SpeechContext *ctx);

const char *speech_get_partial(SpeechContext *ctx);
void speech_free(SpeechContext *ctx);

#endif // SPEECH_RECOGNITION
