# Upload to GitHub

## Create repository

Create a new GitHub repository named:

`serving-robot-nodemcu`

Then run:

```bash
cd serving-robot-nodemcu
git init
git add .
git commit -m "Initial serving robot NodeMCU project"
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/serving-robot-nodemcu.git
git push -u origin main
```

Replace `YOUR_USERNAME` with your GitHub username.

## PlatformIO

Open the folder in VS Code with PlatformIO installed.

Build:

```bash
pio run
```

Upload:

```bash
pio run --target upload
```

Serial monitor:

```bash
pio device monitor
```

## Arduino IDE

Install:
1. ESP8266 board package
2. LiquidCrystal_I2C
3. Keypad
4. MFRC522

Open `src/main.cpp` or copy the firmware into an Arduino sketch and adapt the pin definitions to your exact hardware.
