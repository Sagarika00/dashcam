from gtts import gTTS
import os
import hashlib
from time import time

def text2speech(tokens):
    sent_token = " ".join(tokens)
    tts = gTTS(text=sent_token, lang='en')
    hd = compute_hash(sent_token)
    filename = hd[0:8] + ".mp3"
    tts.save(filename)
    return filename

def compute_hash(word):
    m = hashlib.md5()
    m.update(bytes(word + "_" + str(time()), encoding='utf8'))
    return m.hexdigest()

