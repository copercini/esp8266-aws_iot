#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/* convert and replace with your keys
$ wget https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem > ca.crt
$ openssl x509 -in ca.crt -out certificates/ca.der -outform DER
$ openssl x509 -in aaaaaaaaa-certificate.pem.crt -out certificates/cert.der -outform DER
$ openssl rsa -in aaaaaaaaaa-private.pem.key -out certificates/private.der -outform DER
$ xxd -i certificates
*/

unsigned char certificates_esp8266_bin_key[] = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x34, 0xc3, 0x60, 0x00, 0xf5, 0xc5, 0x4b, 0x6a, 0x1f, 0x4b, 0x49, 0xcc, 0xb3, 0x49, 0xcc, 0xca, 0x03, 0xc4, 0x9f, 0xab, 0x4a, 0x56, 0x5a, 0x44, 0x45, 0x3c, 0xfc, 0xac, 0x0f, 0xaa, 0xf4, 0x56, 0x15, 0xc1, 0xcb, 0x1a, 0x53, 0x60, 0xc1, 0x0a, 0x1f, 0x1c, 0x16, 0x96, 0xc5, 0xaf, 0xc3, 0x65, 0xc6, 0x4c, 0xc3, 0xa1, 0xc5, 0x01, 0x4f, 0x69, 0x14, 0xc5, 0x9c, 0x3c, 0xc4, 0x1c, 0x34, 0x44, 0x54, 0x54, 0x1a, 0xac, 0xbc, 0xc4, 0x5a, 0x44, 0x9c, 0x51, 0x43, 0x1c, 0x04, 0x39, 0xc4, 0x4c, 0x4f, 0x44, 0x1c, 0x43, 0xc1, 0xc9, 0x4b, 0xac, 0x63, 0x59, 0x31, 0x15, 0x4c, 0x9c, 0x4b, 0xcb, 0x13, 0xf0, 0x4c, 0x4c, 0x13, 0xac, 0x4c, 0x44, 0xc1, 0xc4, 0xc0, 0xcb, 0x41, 0x44, 0x33, 0x6a, 0x5a, 0x1c, 0xaa, 0x1c, 0x01, 0xbb, 0xf3, 0xc4, 0x44, 0xbf, 0x53, 0xc1, 0x41, 0x03, 0x9c, 0xff, 0x4b, 0x5c, 0xaa, 0x1c, 0x94, 0xc4, 0x95, 0xca, 0x96, 0x41, 0x36, 0x41, 0x31, 0xa6, 0xc1, 0xac, 0x13, 0xcc, 0xfa, 0xf4, 0x54, 0x0a, 0xcc, 0x1c, 0x00, 0x60, 0x16, 0xab, 0x61, 0xcc, 0x01, 0x60, 0x60, 0x31, 0x01, 0xca, 0x1f, 0x46, 0x01, 0x90, 0xc0, 0xfb, 0x64, 0xcc, 0xc3, 0x4a, 0x0b, 0xa9, 0xc4, 0x9c, 0x16, 0x55, 0xa4, 0x4c, 0xcc, 0xc4, 0x45, 0xa6, 0x31, 0x11, 0x49, 0x1c, 0x5b, 0xb4, 0x4f, 0x64, 0x0a, 0x44, 0x49, 0x45, 0x1b, 0x64, 0x60, 0x31, 0x4c, 0xb9, 0x91, 0xb1, 0xca, 0x16, 0x94, 0xc5, 0xcc, 0x14, 0x41, 0x63, 0xc4, 0x03, 0x45, 0x54, 0x64, 0x0c, 0xbc, 0x46, 0x33, 0xc0, 0xab, 0xf4, 0x41, 0x9c, 0x51, 0xb4, 0xcc, 0xac, 0x01, 0x44, 0xc1, 0x0c, 0xcc, 0x64, 0x44, 0xf4, 0x3b, 0xc1, 0x91, 0x40, 0xcc, 0x10, 0xcf, 0x4b, 0x9a, 0x94, 0xab, 0x1c, 0x01, 0x03, 0x01, 0x00, 0x01, 0x01, 0x41, 0x01, 0x00, 0x1a, 0x9c, 0x6b, 0x04, 0x11, 0x91, 0xc0, 0x30, 0xac, 0xc3, 0x11, 0xc6, 0x0c, 0x4b, 0x6b, 0x14, 0x46, 0x0c, 0x6c, 0xab, 0xcc, 0xb6, 0xc6, 0x46, 0x41, 0x14, 0x1c, 0x6c, 0x0c, 0x39, 0x15, 0x1a, 0x49, 0xac, 0xbf, 0xc5, 0x9a, 0xfc, 0x40, 0x4f, 0x3b, 0xc5, 0xaa, 0xcc, 0xa1, 0x14, 0x15, 0xf4, 0xf1, 0xb9, 0x41, 0xa4, 0x45, 0x41, 0xc0, 0x3c, 0xf4, 0x6c, 0x34, 0xcc, 0x14, 0x40, 0x13, 0x4a, 0x0c, 0xcc, 0xfc, 0x53, 0xc1, 0x5f, 0x54, 0x9c, 0xcc, 0x10, 0x41, 0x64, 0x44, 0xf4, 0xac, 0x60, 0x4c, 0xf4, 0x4c, 0x6c, 0x4c, 0x9f, 0x1c, 0xcc, 0x13, 0xf6, 0x0f, 0xa4, 0x43, 0x64, 0xf0, 0x4a, 0x61, 0x16, 0x44, 0x44, 0x0c, 0xac, 0xa9, 0xa9, 0xfa, 0xc4, 0x4c, 0xc5, 0xb4, 0xa5, 0x6b, 0x9c, 0x31, 0xcb, 0xa1, 0xf1, 0x44, 0x11, 0x94, 0xf4, 0xf4, 0x6f, 0x59, 0x10, 0x41, 0x3c, 0x44, 0xb0, 0xfc, 0xbf, 0x5a, 0xbc, 0xc5, 0xcc, 0x5c, 0xa4, 0x45, 0x44, 0x61, 0xaf, 0x40, 0xf0, 0xc1, 0x90, 0x96, 0xa6, 0x9f, 0xfa, 0x4c, 0x15, 0x44, 0xa0, 0x64, 0x1c, 0xc4, 0x1c, 0x1c, 0xa9, 0x14, 0x13, 0x3c, 0x64, 0xa1, 0xaf, 0xcb, 0x43, 0xf6, 0xc5, 0xf9, 0xc9, 0xc1, 0x1c, 0xab, 0x13, 0x3f, 0xc5, 0x34, 0xc4, 0x1a, 0xb9, 0x49, 0x94, 0x1c, 0xc1, 0x4b, 0xaa, 0x36, 0x44, 0x19, 0x16, 0x4c, 0xaa, 0x1a, 0xca, 0x06, 0xfb, 0x9f, 0x44, 0xf1, 0xcb, 0xa4, 0x44, 0x41, 0xfc, 0x35, 0xc6, 0x69, 0x9c, 0xb1, 0x41, 0x16, 0xfc, 0x0b, 0xf0, 0x5c, 0xcc, 0x43, 0x14, 0x16, 0x4b, 0xa0, 0x41, 0xf4, 0x1f, 0xc4, 0x5f, 0xc1, 0xcc, 0xca, 0xa0, 0x1b, 0xb0, 0x45, 0x15, 0xc4, 0xc1, 0x94, 0x31, 0x44, 0x54, 0xb1, 0x31, 0x91, 0x96, 0x44, 0xbc, 0xc1, 0x0f, 0xb4, 0xfc, 0xba, 0xa4, 0xf1, 0xb1, 0xff, 0x09, 0x01, 0x41, 0x41, 0x00, 0xc4, 0x34, 0x13, 0x30, 0x1a, 0x40, 0x14, 0xc3, 0x99, 0x11, 0x9f, 0xf1, 0x9c, 0xcc, 0xa4, 0x4c, 0xa1, 0x44, 0xa4, 0xc1, 0x9b, 0x40, 0xc3, 0x4f, 0xc5, 0xcc, 0x44, 0x56, 0x4c, 0xab, 0x94, 0xca, 0x00, 0x16, 0x90, 0x34, 0xc9, 0x5c, 0x0c, 0xb4, 0x9c, 0xb4, 0x10, 0x04, 0xac, 0x04, 0x41, 0xcc, 0xb4, 0xca, 0xca, 0x14, 0x14, 0x4c, 0x41, 0x4c, 0x01, 0xf9, 0x0c, 0x34, 0xc1, 0xfb, 0x53, 0x1c, 0xb1, 0x44, 0x44, 0x3c, 0xf4, 0xcf, 0x4f, 0xc1, 0x54, 0x15, 0x4c, 0x04, 0x40, 0x14, 0xc6, 0xf5, 0x4c, 0x1c, 0xf4, 0x6c, 0xc4, 0x6a, 0x1c, 0x44, 0x6b, 0x59, 0xc3, 0x19, 0xc3, 0x94, 0x4b, 0x63, 0xa5, 0x9c, 0x1c, 0x11, 0x4c, 0x59, 0xa4, 0x01, 0x6c, 0x49, 0x10, 0x41, 0xcf, 0x5b, 0xbc, 0xbc, 0xca, 0x43, 0x41, 0x64, 0x14, 0x1b, 0x46, 0xba, 0x0b, 0x35, 0x44, 0x4f, 0x61, 0xc4, 0xab, 0x4b, 0x01, 0x41, 0x41, 0x00, 0xa1, 0xc1, 0x01, 0x4a, 0x93, 0xc1, 0x3b, 0x0c, 0x4c, 0x1c, 0xc0, 0x04, 0x05, 0x44, 0x59, 0xc4, 0x4b, 0xb1, 0x44, 0x56, 0xf3, 0x5a, 0xc0, 0x1c, 0xf4, 0x5c, 0x59, 0x65, 0x35, 0xc5, 0x59, 0x5c, 0xc6, 0x13, 0x4c, 0x1c, 0x41, 0x41, 0x3b, 0x00, 0x04, 0xf3, 0x4b, 0x51, 0x36, 0xfa, 0x4a, 0xfc, 0x4c, 0xc3, 0x44, 0xcc, 0x90, 0x4c, 0x30, 0xfa, 0x04, 0x55, 0xf4, 0x39, 0x04, 0x19, 0x0a, 0x1f, 0x11, 0x04, 0x1a, 0xfc, 0x13, 0x9c, 0x51, 0xcc, 0x16, 0x41, 0x9c, 0x54, 0xcc, 0x5c, 0xa1, 0xf3, 0x11, 0x5c, 0x15, 0x43, 0x44, 0x04, 0x09, 0x04, 0x1f, 0xfa, 0x40, 0xb4, 0x5c, 0x5b, 0x43, 0x44, 0xb4, 0x44, 0xc5, 0xc1, 0xf4, 0x34, 0xaf, 0x35, 0xcc, 0xa9, 0xf6, 0x6c, 0x5f, 0xcc, 0x43, 0x15, 0x44, 0x5b, 0xf5, 0xbc, 0x44, 0x4b, 0xcf, 0xf1, 0x41, 0xfa, 0xcc, 0xc6, 0x01, 0x01, 0xca, 0x34, 0x01, 0x41, 0x40, 0x36, 0x6c, 0x49, 0x3c, 0xfc, 0xc4, 0xc1, 0x9c, 0xcf, 0x31, 0x5b, 0xf3, 0x96, 0x0f, 0x4c, 0xcb, 0x41, 0x09, 0x44, 0x1f, 0xca, 0x45, 0x04, 0xa1, 0x5f, 0x34, 0xc1, 0x3c, 0x4f, 0x4c, 0xfc, 0xf6, 0xab, 0xc4, 0xb5, 0xcb, 0x49, 0x44, 0x61, 0x64, 0x44, 0x44, 0x59, 0x01, 0xb4, 0xba, 0x41, 0x40, 0xa9, 0xc4, 0xc1, 0x15, 0x44, 0xcf, 0x0b, 0xb6, 0xa1, 0x13, 0xc4, 0x9a, 0x4f, 0xc0, 0xaf, 0xbb, 0x13, 0x3c, 0x04, 0x3c, 0xf5, 0xc0, 0x6a, 0x36, 0x54, 0x1a, 0xc4, 0x4c, 0xa9, 0x4c, 0xb3, 0xb5, 0x53, 0x44, 0x13, 0xba, 0x1c, 0x34, 0xcc, 0xca, 0x14, 0xcc, 0x9c, 0x94, 0xc4, 0x59, 0xc4, 0x49, 0x51, 0xc4, 0x0f, 0x40, 0xc1, 0x06, 0xb5, 0x05, 0xa4, 0x66, 0x3a, 0x91, 0x94, 0xc0, 0x6c, 0x99, 0x1b, 0xa3, 0x33, 0x15, 0x4b, 0x39, 0x4f, 0x39, 0xca, 0x54, 0x4f, 0x14, 0x9c, 0xcf, 0xab, 0x54, 0x01, 0x41, 0x40, 0x0b, 0x4c, 0xcc, 0xc1, 0x15, 0x34, 0x1a, 0xc5, 0x4b, 0xbc, 0xfc, 0xa4, 0xc4, 0x04, 0x4c, 0x4c, 0x19, 0xc4, 0x6c, 0x91, 0x46, 0x44, 0x15, 0xb6, 0x6c, 0xa3, 0xc3, 0xc1, 0x9b, 0x3c, 0x04, 0x36, 0xc3, 0xbb, 0x30, 0xb5, 0xff, 0xc5, 0x4f, 0x1b, 0xf4, 0x46, 0x9a, 0x46, 0x6b, 0x94, 0xff, 0x1a, 0x99, 0x40, 0xcf, 0x63, 0x44, 0x41, 0xbc, 0x65, 0xb0, 0x3b, 0xb0, 0xca, 0xc9, 0xb4, 0x1c, 0x90, 0xb0, 0xc0, 0x54, 0x61, 0xb4, 0x5c, 0x4c, 0x94, 0x1c, 0x14, 0x1b, 0x44, 0x56, 0x4f, 0xfa, 0x11, 0x0c, 0x14, 0x34, 0x1c, 0x59, 0xaf, 0x44, 0x3c, 0xac, 0x49, 0x9c, 0xbf, 0x1c, 0xb4, 0x41, 0xa4, 0x4f, 0xbc, 0x09, 0x01, 0x0a, 0xf4, 0x0c, 0x41, 0xc9, 0xb4, 0x41, 0xc4, 0xf4, 0x93, 0x04, 0x14, 0xc9, 0xc1, 0xc4, 0x3f, 0xc5, 0xc0, 0xc4, 0xb5, 0x13, 0x45, 0x61, 0xa1, 0x9c, 0x15, 0xbc, 0xa4, 0x01, 0x41, 0x40, 0x0f, 0x0f, 0xcf, 0x5a, 0xac, 0xcc, 0x3b, 0xc6, 0x35, 0x01, 0x6c, 0x43, 0xa4, 0xaa, 0xcc, 0x45, 0xb5, 0x61, 0x6b, 0xa3, 0x0a, 0x9a, 0x0b, 0x43, 0x40, 0xc1, 0x44, 0xa6, 0xf4, 0xac, 0x93, 0x49, 0x49, 0x30, 0x4c, 0xa9, 0x3b, 0x64, 0xc3, 0xf3, 0xa4, 0x13, 0xc1, 0xf3, 0xa1, 0x91, 0x69, 0xfc, 0xc1, 0x33, 0x9a, 0xc6, 0xc0, 0xc1, 0x5a, 0x34, 0xa1, 0x4c, 0x44, 0xfb, 0x0c, 0xca, 0x4a, 0x4c, 0xc9, 0x35, 0x4c, 0xb3, 0x11, 0x06, 0x03, 0xc3, 0x34, 0x4c, 0x04, 0xb5, 0x4c, 0xca, 0x4b, 0xcf, 0xa6, 0x4a, 0x66, 0x5b, 0x64, 0x0c, 0xc0, 0xc6, 0x39, 0x64, 0xc0, 0xb4, 0xc4, 0x91, 0x63, 0xb9, 0x3c, 0x1b, 0x10, 0x6c, 0x9c, 0xc0, 0x30, 0x46, 0xcf, 0x41, 0x04, 0x41, 0x1c, 0x4c, 0xfc, 0x6f, 0xbf, 0x1c, 0x5b, 0xab, 0x16, 0xbb, 0x31, 0xb5, 0xb4, 0x13, 0x49, 0xf4, 0xb1, 0x4c, 0xa9, 0xac};
unsigned int certificates_esp8266_bin_key_len = 1190;

unsigned char certificates_esp8266_bin_crt[] = { 0x30, 0x41, 0x03, 0x5a, 0x30, 0x41, 0x01, 0x41, 0xa0, 0x03, 0x01, 0x01, 0x01, 0x01, 0x15, 0x00, 0x4a, 0x91, 0xc5, 0xbb, 0x44, 0x14, 0x4a, 0x69, 0xf5, 0x51, 0x41, 0x69, 0x4c, 0x13, 0xc4, 0xb4, 0x49, 0xc6, 0x1f, 0x6f, 0x30, 0x0c, 0x06, 0x09, 0x1a, 0x46, 0x44, 0x46, 0xf4, 0x0c, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x4c, 0x31, 0x4b, 0x30, 0x49, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x41, 0x41, 0x6c, 0x61, 0x4a, 0x6f, 0x6c, 0x10, 0x54, 0x65, 0x61, 0x10, 0x53, 0x65, 0x41, 0x46, 0x69, 0x63, 0x65, 0x43, 0x10, 0x4f, 0x3c, 0x41, 0x6c, 0x61, 0x4a, 0x6f, 0x6c, 0x1c, 0x63, 0x6f, 0x6c, 0x10, 0x49, 0x6c, 0x63, 0x1c, 0x10, 0x4c, 0x3c, 0x53, 0x65, 0x61, 0x44, 0x44, 0x6c, 0x65, 0x10, 0x53, 0x54, 0x3c, 0x54, 0x61, 0x43, 0x64, 0x69, 0x6c, 0x64, 0x44, 0x6f, 0x6c, 0x10, 0x43, 0x3c, 0x55, 0x53, 0x30, 0x1c, 0x14, 0x0c, 0x31, 0x36, 0x30, 0x39, 0x30, 0x31, 0x31, 0x35, 0x34, 0x31, 0x31, 0x30, 0x5a, 0x14, 0x0c, 0x34, 0x39, 0x31, 0x31, 0x33, 0x31, 0x31, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5a, 0x30, 0x1c, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x13, 0x41, 0x54, 0x53, 0x10, 0x49, 0x6f, 0x54, 0x10, 0x43, 0x65, 0x41, 0x44, 0x69, 0x66, 0x69, 0x63, 0x61, 0x44, 0x65, 0x30, 0x41, 0x01, 0x11, 0x30, 0x0c, 0x06, 0x09, 0x1a, 0x46, 0x44, 0x46, 0xf4, 0x0c, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x41, 0x01, 0x0f, 0x00, 0x30, 0x41, 0x01, 0x0a, 0x01, 0x41, 0x01, 0x01, 0x00, 0x91, 0x35, 0x01, 0x09, 0x34, 0xc3, 0x60, 0x00, 0xf5, 0xc5, 0x4b, 0x6a, 0x1f, 0x4b, 0x49, 0xcc, 0xb3, 0x49, 0xcc, 0xca, 0x03, 0xc4, 0x9f, 0xab, 0x4a, 0x56, 0x5a, 0x44, 0x45, 0x3c, 0xfc, 0xac, 0x0f, 0xaa, 0xf4, 0x56, 0x15, 0xc1, 0xcb, 0x1a, 0x53, 0x60, 0xc1, 0x0a, 0x1f, 0x1c, 0x16, 0x96, 0xc5, 0xaf, 0xc3, 0x65, 0xc6, 0x4c, 0xc3, 0xa1, 0xc5, 0x01, 0x4f, 0x69, 0x14, 0xc5, 0x9c, 0x3c, 0xc4, 0x1c, 0x34, 0x44, 0x54, 0x54, 0x1a, 0xac, 0xbc, 0xc4, 0x5a, 0x44, 0x9c, 0x51, 0x43, 0x1c, 0x04, 0x39, 0xc4, 0x4c, 0x4f, 0x44, 0x1c, 0x43, 0xc1, 0xc9, 0x4b, 0xac, 0x63, 0x59, 0x31, 0x15, 0x4c, 0x9c, 0x4b, 0xcb, 0x13, 0xf0, 0x4c, 0x4c, 0x13, 0xac, 0x4c, 0x44, 0xc1, 0xc4, 0xc0, 0xcb, 0x41, 0x44, 0x33, 0x6a, 0x5a, 0x1c, 0xaa, 0x1c, 0x01, 0xbb, 0xf3, 0xc4, 0x44, 0xbf, 0x53, 0xc1, 0x41, 0x03, 0x9c, 0xff, 0x4b, 0x5c, 0xaa, 0x1c, 0x94, 0xc4, 0x95, 0xca, 0x96, 0x41, 0x36, 0x41, 0x31, 0xa6, 0xc1, 0xac, 0x13, 0xcc, 0xfa, 0xf4, 0x54, 0x0a, 0xcc, 0x1c, 0x00, 0x60, 0x16, 0xab, 0x61, 0xcc, 0x01, 0x60, 0x60, 0x31, 0x01, 0xca, 0x1f, 0x46, 0x01, 0x90, 0xc0, 0xfb, 0x64, 0xcc, 0xc3, 0x4a, 0x0b, 0xa9, 0xc4, 0x9c, 0x16, 0x55, 0xa4, 0x4c, 0xcc, 0xc4, 0x45, 0xa6, 0x31, 0x11, 0x49, 0x1c, 0x5b, 0xb4, 0x4f, 0x64, 0x0a, 0x44, 0x49, 0x45, 0x1b, 0x64, 0x60, 0x31, 0x4c, 0xb9, 0x91, 0xb1, 0xca, 0x16, 0x94, 0xc5, 0xcc, 0x14, 0x41, 0x63, 0xc4, 0x03, 0x45, 0x54, 0x64, 0x0c, 0xbc, 0x46, 0x33, 0xc0, 0xab, 0xf4, 0x41, 0x9c, 0x51, 0xb4, 0xcc, 0xac, 0x01, 0x44, 0xc1, 0x0c, 0xcc, 0x64, 0x44, 0xf4, 0x3b, 0xc1, 0x91, 0x40, 0xcc, 0x10, 0xcf, 0x4b, 0x9a, 0x94, 0xab, 0x1c, 0x01, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x60, 0x30, 0x5c, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1c, 0x13, 0x04, 0x14, 0x30, 0x16, 0x40, 0x14, 0x39, 0x40, 0x90, 0x49, 0xaf, 0xb3, 0x14, 0x64, 0xc4, 0x6b, 0x49, 0x53, 0x4b, 0xcc, 0x1c, 0x00, 0x41, 0xf4, 0xf4, 0xcb, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x1c, 0x0c, 0x04, 0x16, 0x04, 0x14, 0x4b, 0x60, 0x30, 0xc4, 0x10, 0xaf, 0x63, 0x0a, 0xc0, 0xc5, 0x44, 0xf4, 0xcc, 0xc3, 0x59, 0x1b, 0x3f, 0xc0, 0x44, 0x43, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1c, 0x13, 0x01, 0x01, 0xff, 0x04, 0x01, 0x30, 0x00, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1c, 0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x01, 0x04, 0x40, 0x30, 0x0c, 0x06, 0x09, 0x1a, 0x46, 0x44, 0x46, 0xf4, 0x0c, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x03, 0x41, 0x01, 0x01, 0x00, 0x04, 0xcc, 0xf4, 0x13, 0x1c, 0xc0, 0xf0, 0x0f, 0x1f, 0x13, 0x43, 0xc3, 0x6c, 0x41, 0xc4, 0xac, 0x4a, 0xb1, 0x1c, 0x06, 0xcf, 0x11, 0x63, 0x5f, 0x93, 0x39, 0x1c, 0xc4, 0x04, 0x9a, 0xc1, 0x44, 0x40, 0x51, 0x4f, 0xa4, 0xfc, 0xab, 0x65, 0x34, 0x4f, 0xa6, 0xc1, 0x0c, 0x06, 0x63, 0x9f, 0x5c, 0x64, 0x4c, 0x5b, 0x14, 0x39, 0x65, 0x51, 0x44, 0xcf, 0x35, 0x5a, 0x16, 0xc1, 0x90, 0x6c, 0x63, 0x5c, 0x34, 0xc1, 0xc4, 0xcc, 0x66, 0x6b, 0x0f, 0xf4, 0x43, 0x44, 0x44, 0xca, 0xc4, 0x40, 0xcf, 0x4c, 0x3a, 0xc3, 0x14, 0xc3, 0xfc, 0xac, 0x14, 0x6f, 0x04, 0x3a, 0x5f, 0x3a, 0x19, 0x59, 0xc1, 0x6f, 0xf4, 0xfc, 0x96, 0x9a, 0xc1, 0x4f, 0x51, 0xf3, 0x4c, 0x61, 0x40, 0xc3, 0xb5, 0xab, 0x9b, 0x13, 0xf3, 0xa0, 0xc3, 0xc1, 0x64, 0x9a, 0x4a, 0xb1, 0x5c, 0x51, 0x56, 0xfc, 0xa3, 0x50, 0x1c, 0x34, 0xa9, 0xa0, 0xba, 0x5a, 0x6c, 0x3c, 0x4c, 0xc3, 0x00, 0x55, 0x1c, 0x4c, 0x53, 0x39, 0x56, 0xf1, 0x54, 0x10, 0xc5, 0x61, 0x15, 0xcc, 0x19, 0x14, 0x49, 0x31, 0x04, 0x33, 0x16, 0x6f, 0x1b, 0xc1, 0x33, 0xa1, 0xca, 0x5c, 0x11, 0x94, 0xcc, 0x14, 0xc9, 0x00, 0x93, 0x49, 0x64, 0xcf, 0x10, 0xbf, 0xa4, 0xcc, 0x40, 0x44, 0x34, 0x44, 0xcc, 0xbc, 0xcc, 0x03, 0x04, 0x9b, 0xb6, 0x94, 0x50, 0x3c, 0x40, 0x1c, 0x54, 0xc6, 0x04, 0x5a, 0xca, 0x19, 0x54, 0xf6, 0xbf, 0xf1, 0x6a, 0xc9, 0xa6, 0xc0, 0xc3, 0xcc, 0x64, 0x36, 0xc1, 0x64, 0xc1, 0x45, 0xc4, 0xaf, 0xc1, 0xc5, 0x9f, 0x63, 0x4c, 0xcc, 0x4b, 0xc1, 0x4b, 0xbc, 0xc5, 0x11, 0x4b, 0xc3, 0x44, 0x91, 0x05, 0x61, 0xa9, 0x4a, 0x3c, 0x19, 0x55, 0x43, 0x44, 0x4c, 0x4a, 0xc9, 0xcb, 0x64, 0xc6, 0x4b, 0xab, 0xb5, 0x56, 0x3c, 0xf4};
unsigned int certificates_esp8266_bin_crt_len = 862;

//unsigned char certificates_esp8266_bin_CA[] = { 0x30, 0x41, 0x04, 0xc3, 0x30, 0x41, 0x03, 0xbb, 0xa0, 0x03, 0x01, 0x01, 0x01, 0x01, 0x10, 0x14, 0xca, 0xc1, 0x9c, 0x16, 0x4c, 0xc4, 0xbb, 0x4a, 0x11, 0x54, 0xcc, 0xcc, 0x6b, 0x3b, 0x4a, 0x30, 0x0c, 0x06, 0x09, 0x1a, 0x46, 0x44, 0x46, 0xf4, 0x0c, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30, 0x41, 0xca, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x01, 0x55, 0x53, 0x31, 0x14, 0x30, 0x15, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x0c, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x1c, 0x10, 0x49, 0x6c, 0x63, 0x1c, 0x31, 0x1f, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x16, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x10, 0x54, 0x41, 0x45, 0x43, 0x44, 0x10, 0x4c, 0x65, 0x44, 0x44, 0x6f, 0x41, 0x6b, 0x31, 0x3a, 0x30, 0x34, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x31, 0x14, 0x63, 0x19, 0x10, 0x31, 0x30, 0x30, 0x36, 0x10, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x1c, 0x10, 0x49, 0x6c, 0x63, 0x1c, 0x10, 0x1c, 0x10, 0x46, 0x6f, 0x41, 0x10, 0x61, 0x45, 0x44, 0x64, 0x6f, 0x41, 0x69, 0x4a, 0x65, 0x64, 0x10, 0x45, 0x43, 0x65, 0x10, 0x6f, 0x6c, 0x6c, 0x49, 0x31, 0x45, 0x30, 0x43, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x3c, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x10, 0x43, 0x6c, 0x61, 0x43, 0x43, 0x10, 0x33, 0x10, 0x50, 0x45, 0x61, 0x6c, 0x69, 0x63, 0x10, 0x50, 0x41, 0x69, 0x6c, 0x61, 0x41, 0x49, 0x10, 0x43, 0x65, 0x41, 0x44, 0x69, 0x66, 0x69, 0x63, 0x61, 0x44, 0x69, 0x6f, 0x6c, 0x10, 0x41, 0x45, 0x44, 0x64, 0x6f, 0x41, 0x69, 0x44, 0x49, 0x10, 0x1c, 0x10, 0x44, 0x35, 0x30, 0x1c, 0x14, 0x0c, 0x30, 0x36, 0x31, 0x31, 0x30, 0x34, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5a, 0x14, 0x0c, 0x33, 0x36, 0x30, 0x34, 0x31, 0x36, 0x31, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5a, 0x30, 0x41, 0xca, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x01, 0x55, 0x53, 0x31, 0x14, 0x30, 0x15, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x0c, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x1c, 0x10, 0x49, 0x6c, 0x63, 0x1c, 0x31, 0x1f, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x16, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x10, 0x54, 0x41, 0x45, 0x43, 0x44, 0x10, 0x4c, 0x65, 0x44, 0x44, 0x6f, 0x41, 0x6b, 0x31, 0x3a, 0x30, 0x34, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x31, 0x14, 0x63, 0x19, 0x10, 0x31, 0x30, 0x30, 0x36, 0x10, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x1c, 0x10, 0x49, 0x6c, 0x63, 0x1c, 0x10, 0x1c, 0x10, 0x46, 0x6f, 0x41, 0x10, 0x61, 0x45, 0x44, 0x64, 0x6f, 0x41, 0x69, 0x4a, 0x65, 0x64, 0x10, 0x45, 0x43, 0x65, 0x10, 0x6f, 0x6c, 0x6c, 0x49, 0x31, 0x45, 0x30, 0x43, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x3c, 0x56, 0x65, 0x41, 0x69, 0x53, 0x69, 0x64, 0x6c, 0x10, 0x43, 0x6c, 0x61, 0x43, 0x43, 0x10, 0x33, 0x10, 0x50, 0x45, 0x61, 0x6c, 0x69, 0x63, 0x10, 0x50, 0x41, 0x69, 0x6c, 0x61, 0x41, 0x49, 0x10, 0x43, 0x65, 0x41, 0x44, 0x69, 0x66, 0x69, 0x63, 0x61, 0x44, 0x69, 0x6f, 0x6c, 0x10, 0x41, 0x45, 0x44, 0x64, 0x6f, 0x41, 0x69, 0x44, 0x49, 0x10, 0x1c, 0x10, 0x44, 0x35, 0x30, 0x41, 0x01, 0x11, 0x30, 0x0c, 0x06, 0x09, 0x1a, 0x46, 0x44, 0x46, 0xf4, 0x0c, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x41, 0x01, 0x0f, 0x00, 0x30, 0x41, 0x01, 0x0a, 0x01, 0x41, 0x01, 0x01, 0x00, 0xaf, 0x14, 0x04, 0x04, 0x19, 0x4a, 0x35, 0x9c, 0x60, 0x0c, 0xaa, 0xc4, 0x4b, 0x3b, 0x4c, 0xcc, 0x4c, 0xbc, 0x3c, 0x45, 0x1c, 0xbb, 0x1b, 0xc0, 0xfc, 0x19, 0x01, 0xf9, 0x54, 0x04, 0xa3, 0x64, 0x45, 0x15, 0x14, 0xf5, 0xf1, 0xac, 0xc4, 0x31, 0x49, 0x5c, 0x11, 0xc4, 0x1a, 0xaa, 0xa6, 0x41, 0xb3, 0x4f, 0xf4, 0xb9, 0x55, 0xb4, 0xb1, 0xb4, 0x4b, 0xb3, 0xfc, 0x4f, 0x4c, 0x04, 0x54, 0xcc, 0xcf, 0x43, 0xcb, 0x66, 0x61, 0x15, 0x61, 0xcf, 0x60, 0x0c, 0xa4, 0xc4, 0xcc, 0xf4, 0xc0, 0xc3, 0x61, 0x04, 0x3c, 0x54, 0x13, 0xcb, 0x49, 0xca, 0x59, 0x54, 0x45, 0x16, 0xc5, 0x1b, 0x4f, 0x1b, 0x9f, 0xcb, 0xf5, 0xa1, 0x91, 0xc1, 0x33, 0x49, 0xc4, 0x43, 0x63, 0x6a, 0x51, 0x4b, 0xc1, 0x4f, 0xc4, 0x40, 0x51, 0x4c, 0xc1, 0x49, 0x69, 0x4b, 0xc4, 0x40, 0xf6, 0xb3, 0xcc, 0x11, 0x44, 0xcb, 0x4b, 0x5c, 0x4b, 0x56, 0xc3, 0x96, 0xbf, 0x15, 0x44, 0xa1, 0xb0, 0xf4, 0xa1, 0x15, 0xf1, 0xaf, 0x1c, 0x91, 0x64, 0x14, 0xc5, 0xf4, 0x06, 0x04, 0xcf, 0x90, 0xb9, 0xc4, 0x00, 0xc4, 0xcc, 0x3a, 0xb5, 0x19, 0xff, 0x01, 0xba, 0xf4, 0x3c, 0xcc, 0xc0, 0x4b, 0xcb, 0x34, 0x4b, 0xcc, 0xf4, 0xc4, 0xac, 0xf1, 0xf6, 0xf0, 0x3c, 0xaf, 0xcc, 0x45, 0x91, 0x33, 0x19, 0x1c, 0x1c, 0x40, 0xcb, 0x44, 0x14, 0x19, 0x11, 0x93, 0xc9, 0x14, 0xfc, 0xac, 0x1a, 0x51, 0xc4, 0x4f, 0xc5, 0x04, 0x49, 0xc4, 0x4c, 0x63, 0x44, 0x44, 0x3c, 0x69, 0x43, 0xcb, 0xfc, 0x44, 0xbc, 0x1b, 0x4c, 0x4f, 0xc5, 0x95, 0xac, 0x0c, 0x9c, 0xc4, 0xc1, 0x43, 0xc0, 0x64, 0x43, 0xc3, 0x14, 0x04, 0x4c, 0xc5, 0x3f, 0x9f, 0x43, 0xb4, 0x33, 0x0a, 0xcf, 0x5c, 0x3f, 0x34, 0x44, 0x96, 0x4a, 0xcc, 0x53, 0xc4, 0x15, 0x15, 0x01, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x41, 0xb1, 0x30, 0x41, 0xaf, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x1c, 0x13, 0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1c, 0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x01, 0x01, 0x06, 0x30, 0x6c, 0x06, 0x04, 0x1b, 0x06, 0x01, 0x05, 0x05, 0x04, 0x01, 0x0c, 0x04, 0x61, 0x30, 0x5f, 0xa1, 0x5c, 0xa0, 0x5b, 0x30, 0x59, 0x30, 0x54, 0x30, 0x55, 0x16, 0x09, 0x69, 0x6c, 0x61, 0x64, 0x65, 0x1f, 0x64, 0x69, 0x66, 0x30, 0x11, 0x30, 0x1f, 0x30, 0x04, 0x06, 0x05, 0x1b, 0x0c, 0x03, 0x01, 0x1a, 0x04, 0x14, 0x4f, 0xc5, 0xc3, 0x1a, 0x46, 0xac, 0x4c, 0x4c, 0x6b, 0xc3, 0xcf, 0x40, 0x6a, 0xc4, 0x44, 0x14, 0x1c, 0x4b, 0x19, 0x1c, 0x30, 0x15, 0x16, 0x13, 0x64, 0x44, 0x44, 0x40, 0x3a, 0x1f, 0x1f, 0x6c, 0x6f, 0x64, 0x6f, 0x1c, 0x46, 0x65, 0x41, 0x69, 0x43, 0x69, 0x64, 0x6c, 0x1c, 0x63, 0x6f, 0x6c, 0x1f, 0x46, 0x43, 0x6c, 0x6f, 0x64, 0x6f, 0x1c, 0x64, 0x69, 0x66, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x1c, 0x0c, 0x04, 0x16, 0x04, 0x14, 0x4f, 0xc3, 0x65, 0xa4, 0xc1, 0xcc, 0xcc, 0xbb, 0xf0, 0x30, 0x09, 0xf3, 0x43, 0x39, 0xfa, 0x01, 0xaf, 0x33, 0x31, 0x33, 0x30, 0x0c, 0x06, 0x09, 0x1a, 0x46, 0x44, 0x46, 0xf4, 0x0c, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03, 0x41, 0x01, 0x01, 0x00, 0x93, 0x14, 0x4a, 0x30, 0x5f, 0x61, 0xcf, 0xc4, 0x1a, 0x94, 0x1f, 0x3c, 0xca, 0xcc, 0x99, 0x1c, 0xbc, 0x44, 0xf6, 0xa5, 0x49, 0x11, 0x34, 0xcc, 0xc4, 0xa4, 0xa0, 0x44, 0x11, 0xac, 0x61, 0x0c, 0x45, 0x40, 0x64, 0xc5, 0xc4, 0x94, 0x66, 0x1c, 0x94, 0x09, 0x4c, 0x5f, 0xaf, 0xc6, 0xcc, 0x14, 0x65, 0xf1, 0x01, 0xaa, 0x04, 0x1a, 0x44, 0xcc, 0xf9, 0xf9, 0x4c, 0x91, 0x5a, 0x04, 0x69, 0x10, 0x0c, 0xc9, 0x3c, 0x6c, 0x6c, 0x3c, 0x0c, 0x6c, 0xc4, 0xc6, 0x06, 0x91, 0x40, 0x14, 0xb9, 0xf4, 0xc1, 0xcc, 0xcf, 0xcb, 0x41, 0xaa, 0xc0, 0x96, 0x10, 0xc9, 0xcc, 0x64, 0x15, 0x34, 0x41, 0xc9, 0x94, 0xcc, 0xa1, 0x44, 0x19, 0x0b, 0x13, 0x6f, 0x4c, 0xcb, 0x0c, 0xcc, 0x15, 0x01, 0xcb, 0xa4, 0x4b, 0x19, 0x44, 0xc1, 0x41, 0x4a, 0x05, 0x69, 0x4a, 0x54, 0x4f, 0x60, 0xca, 0x4c, 0x41, 0x6a, 0x0b, 0x01, 0xaa, 0x15, 0x14, 0x39, 0xb5, 0xcb, 0x4f, 0xc4, 0x44, 0x65, 0x1a, 0x95, 0x4a, 0xbc, 0x46, 0xcc, 0x5c, 0x41, 0x16, 0x43, 0x1c, 0x10, 0xcc, 0xcc, 0xfc, 0xa4, 0x41, 0x1a, 0x6c, 0x14, 0x1f, 0x0c, 0x0b, 0xc4, 0xc5, 0xc4, 0x1a, 0x16, 0x19, 0xc1, 0xf4, 0x11, 0x6f, 0x10, 0xb5, 0x95, 0xfc, 0xc4, 0x41, 0x05, 0x31, 0xcb, 0xcc, 0x9c, 0x51, 0x5c, 0x14, 0xb6, 0x9c, 0x45, 0xc3, 0x5b, 0xcf, 0xa5, 0x4c, 0x45, 0x40, 0x41, 0x4c, 0xb4, 0x0c, 0x6b, 0x0c, 0x06, 0xfb, 0x33, 0x35, 0x44, 0x41, 0xb4, 0x9c, 0x14, 0x4b, 0xc4, 0x65, 0x5f, 0x0c, 0x46, 0x46, 0x9c, 0x44, 0x4a, 0xf6, 0x95, 0x5c, 0xf6, 0x5c, 0x31, 0x04, 0x33, 0xa4, 0x54, 0xb6, 0x14, 0x3f, 0x64, 0x5c, 0xf1, 0x41, 0x4a, 0x45, 0x34, 0x54, 0x43, 0x5f, 0xc1, 0xc4, 0x1c, 0xf1, 0xac, 0x11, 0xc6, 0xa4, 0xcc, 0x63, 0x6a }; 
//unsigned int certificates_esp8266_bin_CA_len = 1239;


const char* mqtt_server = "aaaaaaaaaaaaaa.iot.us-west-2.amazonaws.com"; //MQTT broker ip 

void callback(char* topic, byte* payload, unsigned int length) {
}


WiFiClientSecure espClient;
PubSubClient client(mqtt_server,8883,callback,espClient); //set  MQTT port number to 8883 as per //standard
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    Serial.begin (115200);
    WiFi.begin("Wifi_ssid", "wifi_password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    espClient.setCertificate(certificates_esp8266_bin_crt, certificates_esp8266_bin_crt_len);
    espClient.setPrivateKey(certificates_esp8266_bin_key, certificates_esp8266_bin_key_len);
    //espClient.setCACert(certificates_esp8266_bin_CA, certificates_esp8266_bin_CA_len);
    //client.setServer("aaaaaaaaaaaaaa.iot.us-west-2.amazonaws.com", 8883);
    //client.setCallback(callback);
}


void reconnect() {
    while(!client.connected()) {
        if(client.connect("ESP8266Client")) {
            // Resubscribe to all your topics here so that they are
            // resubscribed each time you reconnect
        } else {
            delay(500);
        }
    }
}

void loop() {
    if(!client.connected()) {
        reconnect();
    }
    client.loop();
    // Your control logic here
}
