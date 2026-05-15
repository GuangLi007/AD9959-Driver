# AD9959 Usage Examples / 使用示例

## 1. Initialization / 初始化

```c
#include "AD9959.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    ad9959_init();   // Reset and configure AD9959 with defaults / 复位并初始化

    while (1)
    {
        // Your application code / 用户代码
    }
}
```

## 2. Set Frequency / 设置频率

Set CH0 to a frequency. The value is multiplied by base step ~8.59 kHz.  
设置 CH0 频率，值为倍率 × 约 8.59 kHz。

```c
ad9959_write_frequency(AD9959_CHANNEL_0, 10000);   // 10000 × 8.59 kHz ≈ 85.9 MHz
```

**Frequency range / 频率范围**: 1 – 200 MHz

## 3. Set Phase / 设置相位

Set CH1 phase / 设置 CH1 相位（14位，16384 = 360°）:

```c
ad9959_write_phase(AD9959_CHANNEL_1, 4096);    // 4096 = 90°
```

| Angle / 角度 | Value / 值 |
|-------------|-----------|
| 0°          | 0         |
| 90°         | 4096      |
| 180°        | 8192      |
| 270°        | 12288     |
| 360°        | 16384     |

## 4. Set Amplitude / 设置幅度

Set CH2 to max amplitude / 设置 CH2 幅度最大（10位，0–1023）:

```c
ad9959_write_amplitude(AD9959_CHANNEL_2, 1023);   // 10-bit max / 最大值
```

## 5. Configure All Channels / 配置所有通道

```c
// Set different frequencies / 设置不同频率
ad9959_write_frequency(AD9959_CHANNEL_0, 1000);    // ~8.6 MHz
ad9959_write_frequency(AD9959_CHANNEL_1, 2000);    // ~17.2 MHz
ad9959_write_frequency(AD9959_CHANNEL_2, 5000);    // ~43.0 MHz
ad9959_write_frequency(AD9959_CHANNEL_3, 10000);   // ~85.9 MHz

// Set all amplitudes to max / 全部幅度设为最大
ad9959_write_amplitude(AD9959_CHANNEL_0, 1023);
ad9959_write_amplitude(AD9959_CHANNEL_1, 1023);
ad9959_write_amplitude(AD9959_CHANNEL_2, 1023);
ad9959_write_amplitude(AD9959_CHANNEL_3, 1023);
```

## 6. Manual I/O Update / 手动 IO 更新

If you passed `update = false` to `ad9959_write_data()`, call this to apply / 如果写入时关闭了自动更新，调用此函数使其生效：

```c
ad9959_io_update();
```

## 7. Hardware Notes / 硬件说明

- System clock < 160 MHz → VCO gain bit = 0 / 系统时钟低于 160 MHz 时 VCO 增益位为 0
- Default charge pump current / 默认电荷泵电流: 75 µA
- Amplitude DAC: 10-bit / 幅度 DAC 10 位 (0–1023 ⇒ 0–530 mV)
- Frequency tuning word / 频率调谐字: 32-bit, `FTW = freq × 2^32 / SYSCLK`
