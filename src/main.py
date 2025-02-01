import pvporcupine
from pvrecorder import PvRecorder
from vosk import Model, KaldiRecognizer

import pyaudio
import wave

import json
import array

from os import getenv
from dotenv import load_dotenv, find_dotenv

load_dotenv(find_dotenv())

access_key = getenv("PICOVOICE")
model = Model("./src/model")
recognizer = KaldiRecognizer(model, 16000)
CHUNK = 1024

if not access_key:
    raise ValueError("""Access_key is missing! 
                     Please set the environment variable""")

porcupine = pvporcupine.create(
    access_key=access_key,
    keyword_paths=["./src/wake word activation/Hey-Mita_en_linux_v3_0_0.ppn"],
)

recorder = PvRecorder(device_index=5, frame_length=porcupine.frame_length)
recorder.start()
print("Using device: %s" % recorder.selected_device)


def play_sound():
    with wave.open("./src/sounds/VoiceMor.wav", "rb") as wf:
        # Instantiate PyAudio and initialize PortAudio system resources (1)
        p = pyaudio.PyAudio()
        # Open stream (2)
        stream = p.open(
            format=p.get_format_from_width(wf.getsampwidth()),
            channels=wf.getnchannels(),
            rate=wf.getframerate(),
            output=True,
        )
        # Play samples from the wave file (3)
        while len(data := wf.readframes(CHUNK)):  # Requires Python 3.8+ for :=
            stream.write(data)
            # Close stream (4)
            stream.close()
            # Release PortAudio system resources (5)
            p.terminate()


def main():
    try:
        while True:
            pcm = recorder.read()
            keyword_index = porcupine.process(pcm)
            if keyword_index == 0:
                print("Hi.")
                play_sound()
            pcm_bytes = array.array("h", pcm).tobytes()
            if recognizer.AcceptWaveform(pcm_bytes):
                result = json.loads(recognizer.Result())
                print(result.get("text", ""))

    except KeyboardInterrupt:
        print("\nStopping...")
    finally:
        recorder.stop()
        recorder.delete()
        porcupine.delete()


if __name__ == "__main__":
    main()
