# AD9959 Driver

STM32 HAL library driver for **AD9959** — a 4-channel direct digital synthesizer (DDS). Uses software SPI (bit-bang) for communication.

## Features

- 4 independent DDS channels (CH0–CH3)
- Frequency: 1 Hz – 200 MHz (base step ~8.59 kHz)
- Phase: 14-bit resolution (0–16383 ⇒ 0°–360°)
- Amplitude: 10-bit resolution (0–1023)
- Per-channel I/O update
- Software SPI (no hardware SPI required)

## Pin Configuration

Define these in your `main.h` or CubeMX `gpio.h`:

| Signal   | Description        |
|----------|--------------------|
| CS       | SPI chip select    |
| SCLK     | SPI clock          |
| SDIO0–3  | SPI data lines     |
| UPDATE   | I/O update strobe  |
| PS0–3    | Profile select     |
| PDC      | Power-down control |
| RESET    | Master reset       |

## API Reference

```c
void ad9959_init(void);
void ad9959_reset(void);
void ad9959_write_frequency(AD9959_CHANNEL channel, uint32_t freq_khz);
void ad9959_write_phase(AD9959_CHANNEL channel, uint16_t phase);
void ad9959_write_amplitude(AD9959_CHANNEL channel, uint16_t amplitude);
void ad9959_io_update(void);
```

## File List

| File          | Description                |
|---------------|----------------------------|
| `AD9959.c`    | Driver implementation      |
| `AD9959.h`    | Driver header              |
| `README.md`   | This file                  |
| `EXAMPLE.md`  | Usage examples             |
