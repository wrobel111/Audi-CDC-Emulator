# Audi-CDC-Emulator
Emulator zmieniarki CD wraz z funkcją telefonu

Procesor: AtMega328p
Moduł BT: BK3266

# Sterowanie modułem BT
Spis komend AT: <a href="https://github.com/tomaskovacik/BK3254/wiki/Supported-commands-and-event-send-from-module">wiki tomaskovacik</a>.

# Wnioski po kolejnych wersjach:
V1.1
1. dorobić sygnał enable z gniazda jack - dodać przycisk zwierany do masy gdy podłączony AUX
2. stosować tylko BK3266 a nie BK3254
3. zweryfikować sygnał enable do wzmacniaczy
4. zmienić pinout złącza na standard ISP 6pin
5. zmienić pinout uart : Tx Rx Gnd +5v
