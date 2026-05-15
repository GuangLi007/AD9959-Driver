# AD9959 Usage Examples

## 1. Initialization

```c
#include "AD9959.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    ad9959_init();   // Reset and configure AD9959 with defaults

    while (1)
    {
        // Your application code
    }
}
```

## 2. Set Frequency on CH0

Set CH0 to 10 MHz:

```c
ad9959_write_frequency(AD9959_CHANNEL_0, 10000);   // 10 MHz
```

The base step is ~8.59 kHz, so `10000` ⇒ 10000 × 8.59 kHz ≈ 85.9 MHz.  
Adjust the value according to your desired output frequency.

## 3. Set Phase on CH1

Set CH1 phase to 90°:

```c
ad9959_write_phase(AD9959_CHANNEL_1, 4096);    // 4096 = 90° (14-bit: 16384 = 360°)
```

Phase value mapping:

| Angle  | Value |
|--------|-------|
| 0°     | 0     |
| 90°    | 4096  |
| 180°   | 8192  |
| 270°   | 12288 |
| 360°   | 16384 |

## 4. Set Amplitude on CH2

Set CH2 amplitude to maximum:

```c
ad9959_write_amplitude(AD9959_CHANNEL_2, 1023);   // 10-bit max
```

## 5. Configure All Channels at Once

```c
// Set all 4 channels to different frequencies
ad9959_write_frequency(AD9959_CHANNEL_0, 1000);    // 1 MHz
ad9959_write_frequency(AD9959_CHANNEL_1, 2000);    // 2 MHz
ad9959_write_frequency(AD9959_CHANNEL_2, 5000);    // 5 MHz
ad9959_write_frequency(AD9959_CHANNEL_3, 10000);   // 10 MHz

// Set all amplitudes to max
ad9959_write_amplitude(AD9959_CHANNEL_0, 1023);
ad9959_write_amplitude(AD9959_CHANNEL_1, 1023);
ad9959_write_amplitude(AD9959_CHANNEL_2, 1023);
ad9959_write_amplitude(AD9959_CHANNEL_3, 1023);
```

## 6. Manual I/O Update

If you set `update = false` in `ad9959_write_data()`, call this to apply:

```c
ad9959_io_update();
```

## 7. Hardware Notes

- System clock below 160 MHz → VCO gain control bit = 0
- Default charge pump current: 75 µA
- Amplitude DAC resolution: 10-bit (0–1023 ⇒ 0–530 mV typical)
- Frequency tuning word: 32-bit, calculated as `FTW = freq × 2^32 / SYSCLK`
