from typing import Optional
from os import getenv
from dotenv import load_dotenv, find_dotenv

load_dotenv(find_dotenv())

ACCESS_KEY: Optional[str] = getenv("PICOVOICE")
CHAT_GPT: Optional[str] = getenv("GPT_API_KEY")
ELEVENLABS: Optional[str] = getenv("ELEVENLABS")

if not ACCESS_KEY:
    raise ValueError("""Access_key is missing! 
                     Please set the environment variable""")
