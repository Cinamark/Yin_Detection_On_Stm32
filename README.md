# STM32 ADC2026 – YIN Pitch Detection

**Real-time audio pitch detection on the STM32L432KC Nucleo development board.**

This repository contains the code accompanying the **"PIC-king up the Pieces"** talk from ADC 2026. The project explores implementing the YIN pitch detection algorithm on an STM32 microcontroller, using its built-in Analogue-to-Digital Converter (ADC) to capture audio signals.

The goal is to achieve continuous, real-time pitch detection using efficient buffering and processing techniques.

---

## Table of Contents

- [Project Overview](#project-overview)
- [TODO](#todo)
- [Development Ideas](#development-ideas)
  - [DMA and Circular Buffer](#1-dma-and-circular-buffer)
  - [ADC Interrupts](#2-adc-interrupts)
  - [Timer-Triggered ADC](#3-timer-triggered-adc)
  - [Bare-Metal Register Configuration](#4-bare-metal-register-configuration)
- [CubeMX or Bare-Metal?](#cubemx-or-bare-metal)
- [Contributing](#contributing)
- [References](#references)

---

## Project Overview

| Property | Description |
|:--|:--|
| Microcontroller | STM32L432KC |
| Development Board | NUCLEO-L432KC |
| Peripheral | Analogue-to-Digital Converter (ADC) |
| Algorithm | YIN Pitch Detection |
| Application | Real-time audio processing |
| Display | Standard SSD1306 I2C display |

---

## TODO

The main objective is to implement the following functionality:

1. Read audio samples from the Analogue-to-Digital Converter (ADC).
2. Store incoming samples in a buffer utilising Direct Memory Access.
3. Perform YIN pitch detection on **half of the buffer**.
4. Allow the ADC to simultaneously fill the other half of the buffer.
5. Repeat this process continuously to achieve real-time pitch detection.

This is where **YOU COME IN!**

There are multiple ways to approach this implementation, and I'd encourage contributors to explore different methods.

---

## Development Ideas

Please create **separate Git branches** for your implementations so that different approaches can be developed, tested and compared independently.

### 1. DMA and Circular Buffer

- Configure the ADC to continuously sample the incoming audio signal.
- Use Direct Memory Access (DMA) to transfer samples into memory.
- Implement half-transfer and transfer-complete interrupts.
- Perform YIN on one half of the buffer while DMA fills the other half.

### 2. ADC Interrupts

- Use ADC conversion-complete interrupts to retrieve individual samples.
- Store incoming samples in a circular buffer.
- Trigger YIN processing once a sufficient number of samples have been collected.

### 3. Timer-Triggered ADC

- Use a hardware timer to control the ADC sampling frequency.
- Ensure consistent sampling intervals.
- Combine timer-triggered conversions with DMA or interrupts.

### 4. Bare-Metal Register Configuration

- Configure the STM32 peripherals directly through their registers.
- Avoid relying on the STM32CubeMX ecosystem.
- Manually configure the ADC, timers, DMA and interrupt handling.

---

## CubeMX or Bare-Metal?

Extra points if you implement this **without the CubeMX ecosystem!**

However, STM32CubeMX and the associated HAL libraries are widely used in industry, so using them is also a great opportunity to gain practical experience with industry-standard embedded development workflows.

If you're not used to embedded systems, manually configuring registers is an excellent way to understand what is happening beneath the abstraction layers.

**Feel free to experiment with either approach!**

The ultimate goal is to get real-time YIN pitch detection working on the STM32L432KC.

---

## Contributing

If you'd like to contribute:

1. Fork the repository.
2. Create a separate branch for your implementation.
3. Develop and test your approach.
4. Document your peripheral configuration and sampling strategy.
5. Submit a pull request.

Some interesting areas to investigate and compare:

- Sampling frequency.
- Buffer size.
- YIN processing time.
- CPU utilisation.
- Pitch detection accuracy.
- Overall processing latency.

Different implementations and ideas are welcome!

---

## References

[1] A. de Cheveigné and H. Kawahara, "YIN, a fundamental frequency estimator for speech and music," *The Journal of the Acoustical Society of America*, vol. 111, no. 4, pp. 1917–1930, Apr. 2002, doi: [10.1121/1.1458024](https://doi.org/10.1121/1.1458024).

---

**Happy hacking, and let's get some pitch detection running!**
