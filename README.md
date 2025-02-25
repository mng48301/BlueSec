# BlueSec - Bluetooth Security Assessment Tool

BlueSec is an Arduino-based tool designed to assess and identify potential security vulnerabilities in nearby Bluetooth devices.

## Features

- Scans for nearby Bluetooth devices
- Detects common security vulnerabilities:
    - Known exploits (BlueBorne, KNOB Attack, BIAS Attack, SweynTooth)
    - Vulnerable services
    - Default PIN codes
    - Outdated firmware versions
- Checks encryption modes
- Monitors signal strength for potential evil twin attacks
- Generates comprehensive security assessment reports

## Hardware Requirements

- Arduino board
- Bluetooth module (connected to pins 10, 11)
- Serial connection at 9600 baud rate

## Security Checks

The tool performs the following security assessments:

1. Device Discovery
2. Vulnerability Assessment
     - Service enumeration
     - Default PIN testing
     - Firmware version checking
3. Evil Twin Detection
4. Encryption Analysis
5. Signal Strength Monitoring

## Known Vulnerability Database

Includes detection for:
- Vulnerable services: 0x1800, 0x1801, 0x180F
- Common default PINs
- Known vulnerable firmware versions
- Major Bluetooth exploit signatures

## Usage

1. Connect the Bluetooth module (intended HC 06) to pins 10 (RX) and 11 (TX)
2. Upload the code to your Arduino (Uno R3 recommended)
3. Monitor results through the Serial console at 9600 baud
4. Security assessment reports will generate every 30 seconds

## Warning

This tool is intended for security research and assessment purposes only. Use responsibly and only on devices you own or have permission to test.

## License

@ MIT License 2025
