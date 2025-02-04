from .detection import WakeWordDetector
from .player import play_sound


def main():
    detector = WakeWordDetector(keyword_path="./wake/Hey-Mita_en_linux_v3_0_0.ppn")
    detector.start()
    try:
        while True:
            if detector.detect():
                play_sound()
    except KeyboardInterrupt:
        print("\nStopping...")
    finally:
        detector.stop()


if __name__ == "__main__":
    main()
