import pvporcupine
from pvrecorder import PvRecorder
from os import getenv
from dotenv import load_dotenv, find_dotenv

load_dotenv(find_dotenv())

access_key = getenv("PICOVOICE")

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


while True:
    pcm = recorder.read()
    keyword_index = porcupine.process(pcm)
    if keyword_index == 0:
        print("Hi.")
porcupine.delete()
