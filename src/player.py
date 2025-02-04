import pyaudio
import wave


CHUNK = 1024


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
