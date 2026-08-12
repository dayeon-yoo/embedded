# RFID Traffic Card

## Overview

An RFID-based traffic card system using Raspberry Pi and
the MFRC522 RFID reader.

The system uses Block 1 of the RFID card to store and manage
the card balance.

## Functions

| Input | Function |
| ----- | -------- |
| 1 | Recharge |
| 2 | Payment |
| 3 | Check balance |

### Recharge

Adds the input amount to the RFID card balance.

### Payment

Subtracts the input amount from the RFID card balance.

If the payment amount is greater than the current balance,
the payment is cancelled and `Not Enough balance` is displayed.

### Check

Displays the current balance stored on the RFID card.

When the card is checked for the first time, the balance is
initialized to 10,000.

## RFID Block

Only Block 1 is used for the traffic card data.

| Data | Location |
| ---- | -------- |
| Balance | Block 1, Byte 0~3 |
| Unused | Block 1, Byte 4~15 |

## Hardware

- Raspberry Pi 3 Model B+
- MFRC522 RFID Reader
- MIFARE RFID Card

## Implementation

- Initialize Raspberry Pi GPIO and SPI communication
- Detect an RFID card
- Select and authenticate the card
- Use Block 1 to store the balance
- Read the balance from the RFID card
- Recharge the card with the input amount
- Deduct the payment amount from the balance
- Display the current balance
- Handle insufficient balance and invalid input
- Return to the function selection menu after each operation

## Build

```bash
gcc -Wall -o rfid_traffic_card main.c ../rfid/MFRC522.c -lwiringPi
```

## Run

```bash
sudo ./rfid_traffic_card
```

## Example

```text
... Touch RFID CARD

Select Functions! (1.recharge  2.payment  3.check)
3
Function 3 is selected
... Touch RFID CARD
balance: 10000

Select Functions! (1.recharge  2.payment  3.check)
2
Function 2 is selected
Input the payment amount
2000
... Touch RFID CARD
balance: 8000

Select Functions! (1.recharge  2.payment  3.check)
1
Function 1 is selected
Input the recharge amount
5000
... Touch RFID CARD
balance: 13000

Select Functions! (1.recharge  2.payment  3.check)
2
Function 2 is selected
Input the payment amount
200000
... Touch RFID CARD
Not Enough balance
balance: 13000
```

## Error Handling

- Invalid function numbers return to the function selection menu.
- Invalid amounts are rejected.
- RFID connection errors are reported as `Connection Error!`.
- Payment is cancelled when the requested amount exceeds the balance.

## Source

Implemented based on the RFID traffic card assignment
requirements from the Embedded Systems laboratory materials.