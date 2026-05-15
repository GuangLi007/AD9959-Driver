# AD9959 Driver

STM32 HAL library driver for **AD9959** — a 4-channel direct digital synthesizer (DDS). Uses software SPI (bit-bang) for communication.

4 通道 DDS 频率合成器驱动，软件 SPI 模拟。

---

## Features / 功能

- 4 independent DDS channels (CH0–CH3) / 4 路独立通道
- Frequency: 1 Hz – 200 MHz (base step ~8.59 kHz) / 频率范围
- Phase: 14-bit resolution (0–16383 ⇒ 0°–360°) / 14 位相位
- Amplitude: 10-bit resolution (0–1023) / 10 位幅度
- Software SPI, no hardware SPI required / 纯 IO 模拟，无需硬件 SPI

---

## Pin Configuration / 引脚定义

Define these in `main.h` or CubeMX `gpio.h`:

| Signal   | Description          | 说明         |
|----------|----------------------|-------------|
| CS       | SPI chip select      | 片选         |
| SCLK     | SPI clock            | 时钟         |
| SDIO0–3  | SPI data lines       | 数据线       |
| UPDATE   | I/O update strobe    | IO 更新触发  |
| PS0–3    | Profile select       | 配置选择     |
| PDC      | Power-down control   | 掉电控制     |
| RESET    | Master reset         | 主复位       |

---

## API Reference / API 说明

```c
void ad9959_init(void);                                              // 初始化
void ad9959_reset(void);                                             // 复位
void ad9959_write_frequency(AD9959_CHANNEL channel, uint32_t freq);  // 写频率
void ad9959_write_phase(AD9959_CHANNEL channel, uint16_t phase);     // 写相位
void ad9959_write_amplitude(AD9959_CHANNEL channel, uint16_t amp);   // 写幅度
void ad9959_io_update(void);                                         // IO 更新
```

---

## Files / 文件

| File            | Description            | 说明         |
|-----------------|------------------------|-------------|
| `AD9959.c`      | Driver implementation  | 驱动实现     |
| `AD9959.h`      | Driver header          | 驱动头文件   |
| `README.md`     | Bilingual doc          | 双语说明     |
| `EXAMPLE.md`    | Bilingual examples     | 双语示例     |
