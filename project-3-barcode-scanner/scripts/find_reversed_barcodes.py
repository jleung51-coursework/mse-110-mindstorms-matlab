#!/usr/bin/env python3

barcode_values = [
    'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X',
    'Y', 'Z' ]
barcode_keys = [
    311113113, 113113113, 313113111, 111133113,
    311133111, 113133111, 111113313, 311113311,
    113113311, 111133311, 311111133, 113111133,
    313111131, 111131133, 311131131, 113131131,
    111111333, 311111331, 113111331, 111131331,
    331111113, 133111113, 333111111, 131131113,
    331131111, 133131111 ]

key_index = 0
for key in barcode_keys:
    reversed_key = int(''.join(reversed(str(key))))
    if reversed_key in barcode_keys:
		reversed_key_index = barcode_keys.index(reversed_key)
		if key_index < reversed_key_index:
		    print(str(barcode_values[key_index]) + " backwards equals " +
                    str(barcode_values[reversed_key_index]) + " forwards")
    key_index += 1