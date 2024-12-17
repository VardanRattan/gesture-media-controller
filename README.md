# Gesture Media Controller 🎮

Control your media playback with intuitive tilt and shake gestures using an **ESP32** and **MPU6050**. This project transforms your gestures into Bluetooth commands, allowing seamless interaction with paired devices.

---

## Features 🚀
- **Right Tilt** → Next Track  
- **Left Tilt** → Previous Track  
- **Shake** → Play/Pause Media  
- Lightweight and portable setup
- Compatible with most Bluetooth-enabled devices

---

## Description 📋
This project uses the **ESP32** and **MPU6050** sensor to detect tilt and shake gestures.

By interpreting accelerometer and gyroscope data, the system detects motions such as tilts and shakes. These gestures are converted into **media control commands** (e.g., play, pause, next track) and sent to a connected Bluetooth device using the ESP32's BLE Keyboard functionality.

The setup is ideal for anyone seeking an affordable and DIY-friendly gesture-control solution for media playback.

---

## Circuit Diagram 📐
Here's how to connect the components:

1. **MPU6050 Connections**:  
   - VCC → ESP32 3.3V  
   - GND → ESP32 GND  
   - SCL → ESP32 GPIO 22  
   - SDA → ESP32 GPIO 21    

### Visualization of Circuit  
```plaintext
[ESP32]                           [MPU6050]
GND    -------------------------    GND
3.3V   -------------------------    VCC
GPIO 22 -------------------------    SCL
GPIO 21 -------------------------    SDA
```

---

## Prerequisites 📦

### Hardware:
- ESP32 Development Board
- MPU6050 (Accelerometer + Gyroscope Sensor)
- Jumper Wires, Breadboard, or PCB
- Power Supply (e.g., USB or battery pack)

### Software:
- pArduino IDE](https://www.arduino.cc/) (1.8.19 or newer)
- Libraries (install via Library Manager in Arduino IDE):
  - Wire
  - BleKeyboard

---

## Installation ⚙️

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/gesture-media-controller.git
   ```

2. Open the `gesture_media_controller.ino` file in the Arduino IDE.

3. Install required libraries:
   - Go to Sketch → Include Library → Manage Libraries
   - Search for and install BleKeyboard

4. Connect your ESP32 to the computer and upload the code.

---

## Configuration 🛠️

You can modify the following parameters in the code to customize behavior:

### Shake Sensitivity:
```cpp
const float shakeThreshold = 1.5;  // Lower = more sensitive
```

### Tilt Dead Zone (to avoid overlapping gestures):
```cpp
const float tiltDeadZone = 10.0;
```

### Cooldown Times:
```cpp
const int tiltCooldown = 1000;  // ms
const int shakeDebounce = 500; // ms
```

---

## How It Works 💡

### MPU6050 Initialization:
- The accelerometer and gyroscope data are continuously read
- Roll and pitch are calculated to detect tilt angles

### Gesture Detection:
#### Tilts:
- Right tilt (roll > 30°) → Next Track
- Left tilt (roll < -30°) → Previous Track

#### Shakes:
- Sudden motion above a set threshold → Play/Pause

### Bluetooth Communication:
- Commands are sent to the connected device using the BLE Keyboard library

---

## Future Improvements 🔮
- Add gestures like swipe up/down for volume control
- Implement power-saving modes for extended battery life
- Support for more complex gestures or combinations
- Enhance sensitivity calibration with an app interface

---

## Contributing 🤝
We welcome contributions! Feel free to:
- Fork this repository
- Submit pull requests with enhancements or fixes
- Open issues for bugs or suggestions

---

## License 📜
This project is licensed under the MIT [License](https://github.com/VardanRattan/gesture-media-controller/blob/main/LICENSE). See the LICENSE file for details.

---

## Acknowledgments 🙌
- [BleKeyboard Library](https://github.com/T-vK/ESP32-BLE-Keyboard) by T-vK
- Thanks to the open-source community for inspiration and resources
