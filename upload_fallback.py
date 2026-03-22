import subprocess

try:
    print("Sprawdzam USB...")
    subprocess.check_call(["platformio", "run", "-t", "upload", "-e", "nodemcuUSB"])
except subprocess.CalledProcessError:
    print("USB nie dostępne, próbuję OTA...")
    subprocess.check_call(["platformio", "run", "-t", "upload", "-e", "nodemcuOTA"])