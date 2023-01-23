# AES Registers
|Offset|        Name         | Reset value |Access| Description |
|------|---------------------|-------------|------|-------------|
| 0x00 | R32_AES_CTRL_CCMMOD | 0x00000060  |  RW  | AES Control |
| 0x04 | R32_AES_CCMINT_EN   | 0x00000000  |  RW  | AES Interrupt control/status |
| 0x08 | R32_AES_CCMVT_INIT0 | 0x00000000  |  RW  | AES-CCM Initial Vector 0 |
| 0x0C | R32_AES_CCMVT_INIT1 | 0x00000000  |  RW  | AES-CCM Initial Vector 1 |
| 0x10 | R32_AES_PKT_CNT0    | 0x00000000  |  RW  | AES-CCM packet count 0 |
| 0x14 | R32_AES_PKT_CNT1    | 0x00000000  |  RW  | AES-CCM packet count 1 |
| 0x18 | R32_AES_DATA0       | 0x00000000  |  RW  | AES Data 0 |
| 0x1C | R32_AES_DATA1       | 0x00000000  |  RW  | AES Data 1 |
| 0x20 | R32_AES_DATA2       | 0x00000000  |  RW  | AES Data 2 |
| 0x24 | R32_AES_DATA3       | 0x00000000  |  RW  | AES Data 3 |
| 0x28 | R32_AES_KEY0        | 0x00000000  |  RW  | AES Key 0 |
| 0x2C | R32_AES_KEY1        | 0x00000000  |  RW  | AES Key 1 |
| 0x30 | R32_AES_KEY2        | 0x00000000  |  RW  | AES Key 2 |
| 0x34 | R32_AES_KEY3        | 0x00000000  |  RW  | AES Key 3 |
| 0x38 | R32_AES_KEY4        | 0x00000000  |  RW  | AES Key 4 |
| 0x3C | R32_AES_KEY5        | 0x00000000  |  RW  | AES Key 5 |
| 0x40 | R32_AES_KEY6        | 0x00000000  |  RW  | AES Key 6 |
| 0x44 | R32_AES_KEY7        | 0x00000000  |  RW  | AES Key 7 |
| 0x48 | R32_AES_RAND0       | 0x96906220  |  RO  | AES-CCM Random 0 |
| 0x4C | R32_AES_RAND1       | 0x8A3BBF80  |  RO  | AES-CCM Random 1 |
| 0x50 | R32_AES_RAND2       | 0xB5625304  |  RO  | AES-CCM Random 2 |
| 0x54 | R32_AES_RAND3       | 0x2A43B7E8  |  RO  | AES-CCM Random 3 |

# Bit definitions
## R32_AES_CTRL_CCMMOD
|Bit|      Name      |Access|Description|
|---|----------------|------|-----------|
|0  |   CTRL_START   |  RW  | Write 1 to start AES opration, automatically clears by HW |
|1  |  CTRL_DECRYPT  |  RW  | 0 - encryption, 1 - decryption |
|3-2|       ?        |  ?   | ? |
|4  |  CTRL_KEYLEN   |  RW  | 0 - AES-128, 1 - AES-256 |

## R32_AES_CCMINT_EN
|Bit|      Name      |Access|Description|
|---|----------------|------|-----------|
|0  |     INT_EN     |  RW  | 1 - Enable AES operation complete interrupt |
|1  |   INT_STATUS   |RW,W1C| Set to 1 when an AES operation is completed, requires INT_EN=1 |

