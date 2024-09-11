# WT32-ETH01 Web Updater using Ethernet

This project demonstrates how to set up the WT32-ETH01 (ESP32) module to use Ethernet instead of WiFi for hosting a web-based firmware updater. The device uses a static IP address and provides a simple web interface for firmware updates.

## Requirements

- **Hardware:**
  - WT32-ETH01 v1.4
  - Ethernet cable
  - Power supply (USB or 5V)
  
- **Software:**
  - [Arduino IDE](https://www.arduino.cc/en/software)
  - ESP32 board support package for Arduino ([Installation Guide](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md))

## Pin Configuration

The WT32-ETH01 requires the following pin configuration for Ethernet:

- **MDC Pin**: GPIO23
- **MDIO Pin**: GPIO18
- **PHY Power Pin**: GPIO16
- **PHY Address**: 1

## Installation

1. **Install ESP32 Board Package:**
   - Open the Arduino IDE.
   - Go to `File` -> `Preferences`.
   - Add this URL in the "Additional Boards Manager URLs": `https://dl.espressif.com/dl/package_esp32_index.json`
   - Go to `Tools` -> `Board` -> `Boards Manager`.
   - Search for "ESP32" and install the package by Espressif Systems.

2. **Clone or download this repository:**
   - Clone this project or download the zip and extract it.

3. **Open the Project in Arduino IDE:**
   - Open `OTWwebupdater.ino` in the Arduino IDE.

4. **Configure the Static IP (Optional):**
   - Modify the IP addresses in the sketch if needed.
     - `local_IP`: Set this to the desired static IP for your device (default is `192.168.0.50`).
     - `gateway`: Set this to your network gateway (default is `192.168.0.1`).
     - `subnet`: Set the subnet mask (default is `255.255.255.0`).
     - `primaryDNS` and `secondaryDNS`: Set DNS addresses (default is Google's DNS).

5. **Upload the Sketch:**
   - Connect the WT32-ETH01 to your computer using a USB-to-serial converter.
   - Select your board and the correct COM port in `Tools` -> `Port`.
   - Upload the sketch to your device.

6. **Connect Ethernet:**
   - Connect the WT32-ETH01 to your router or network switch using an Ethernet cable.

## Usage

1. **Access the Web Interface:**
   - Once the device is connected to the network, open a web browser and navigate to the static IP address assigned in the sketch (default: `http://192.168.0.50`).
   
2. **Login Page:**
   - Use the following credentials to log in:
     - **Username**: `admin`
     - **Password**: `admin`
   
3. **Firmware Update:**
   - After logging in, you will see an upload form for firmware updates.
   - Choose a binary file (`.bin`) and upload it to update the device firmware.
   - The device will automatically reboot after the update.

## Troubleshooting

- **No IP Address Assigned**: Ensure that your router or network supports DHCP or that you've correctly set up a static IP.
- **Cannot Access Web Interface**: Double-check the static IP configuration and confirm that the Ethernet cable is properly connected.

## License

This project is licensed under the MIT License.
