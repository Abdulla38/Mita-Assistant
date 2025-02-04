import pvporcupine
from pvrecorder import PvRecorder
from config import ACCESS_KEY


class WakeWordDetector:
    def __init__(self, keyword_path, device_index=5):
        self.porcupine = pvporcupine.create(
            access_key=ACCESS_KEY, keyword_paths=[keyword_path]
        )
        self.recorder = PvRecorder(
            device_index=device_index, frame_length=self.porcupine.frame_length
        )

    def start(self):
        self.recorder.start()
        print("Using device:", self.recorder.selected_device)

    def detect(self):
        """Read audio and check wake worrd"""
        pcm = self.recorder.read()
        return self.porcupine.process(pcm) == 0

    def stop(self):
        self.recorder.stop()
        self.recorder.delete()
        self.porcupine.delete()
