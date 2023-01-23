# ch579_hwaes
Hardware AES on WCH CH57x/CH58x

## Intro
[WCH](https://www.wch.cn/)'s BLE MCU, namely CH57x/CH58x series, includes an AES-CCM hardware engine exclusively used for BLE link layer encryption. However, when BLE is unused, it would be nice to repurpose the AES functionality for general-purpose crypto needs, e.g. accelerating SSL/TLS on CH579 Ethernet applications.

## Precaution
Since WCH did not document bit definitions nor usage of AES registers, information about register and usage in this repository is gathered either from bruteforcing or from reverse engineering, which could be inaccurate or wrong.

## Known Features
Supported == documented in this repo, unsupported == not yet
### Supported
 - AES-128-ECB
 - AES-256-ECB (not officially documented, might not work)
### Unsupported
 - AES-CCM

## Documentation
See [doc](https://github.com/fxsheep/ch579_hwaes/tree/master/doc)

## Demo
See [demo](https://github.com/fxsheep/ch579_hwaes/tree/master/demo)
