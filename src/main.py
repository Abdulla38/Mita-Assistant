import pvporcupine
from pvrecorder import PvRecorder

import pyaudio
import wave

from config import ACCESS_KEY

CHUNK = 1024

porcupine = pvporcupine.create(
    access_key=ACCESS_KEY,
    keyword_paths=["./src/wake word activation/Hey-Mita_en_linux_v3_0_0.ppn"],
)

recorder = PvRecorder(device_index=5, frame_length=porcupine.frame_length)
recorder.start()
print("Using device: %s" % recorder.selected_device)


def play_sound():
    with wave.open("./src/assets/sounds/VoiceMor.wav", "rb") as wf:
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
        data = wf.readframes(CHUNK)
        while data:  # Requires Python 3.8+ for :=
            stream.write(data)
            data = wf.readframes(CHUNK)
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

    except KeyboardInterrupt:
        print("\nStopping...")
    finally:
        recorder.stop()
        recorder.delete()
        porcupine.delete()


if __name__ == "__main__":
    main()
