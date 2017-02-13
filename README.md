## Working example for ESP32: https://github.com/copercini/esp32-iot-examples/

# Some examples using x.509 certificates and TLSv1.2 under Arduino IDE to communicate with AWS iot

After AxTLS update to v2.0.0, the ESP8266 can work with TLS v1.2, the major restriction for this small device communicates with AWS iot natively. You can follow the port here: https://github.com/igrr/axtls-8266

Due heap limitation, the ESP8266 is rebooting before complete handshake, I am hopeful it can be fixed with some optimization.

## How I can get TLS v1.2 working in my ESP8266 with arduino IDE?

#### First: Fix WiFiSecureClient.cc
There is a bug in WiFiSecureClient which doesn't load the certificates before call WiFiClientSecure::connect, it caused because a new SSLContext is created just in this function and not in loadCertificate, loadPrivateKey or loadCACert functions.

My poor fix to this is in WiFiSecureClient folder of this git.
It need to be replaced in Arduino\hardware\esp8266com\esp8266\libraries\ESP8266WiFi\src <br />
of your Arduino git installation (https://github.com/esp8266/Arduino#using-git-version)

#### Second: Update libaxtls.a to last version (b20140f)
You can compile it as you own: add xtensa toolchain to your path, and run make. <br />
Or download a compiled one in the folder axtls of this git and replace it on Arduino\hardware\esp8266com\esp8266\tools\sdk\lib of your Arduino git installation (https://github.com/esp8266/Arduino#using-git-version)


#### Third: Creating a thing, Downloading and converting AWS iot certificates to DER format
http://docs.aws.amazon.com/iot/latest/developerguide/create-device-certificate.html

Converting PEM to DER format: <br />
$ wget https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem > ca.crt <br />
$ openssl x509 -in ca.crt -out certificates/ca.der -outform DER <br />
$ openssl x509 -in aaaaaaaaa-certificate.pem.crt -out certificates/cert.der -outform DER <br />
$ openssl rsa -in aaaaaaaaaa-private.pem.key -out certificates/private.der -outform DER <br />


#### Fourth: Uploading a arduino sketch
Some sketch examples are available in examples folder of this git.


## Why it doesn't work yet?
The heap consumption during TLS handshake is too high, forcing watchdog timer to reboot the device. 

- After boot, login in wifi network and load MQTT functions, ESP8266 has about ~30k fo free heap.
- After load the certificate, private key and CA certificate via SPIFFS filesystem , ESP8266 has about ~19k fo free heap.
- And during TLS handshake, the heap is gone:

State: receiving Server Hello (2) <br />
State: receiving Certificate (11) <br />
State: receiving Certificate Request (13) <br />
State: receiving Server Hello Done (14) <br />
State: sending Certificate (11) <br />
WDT reset <br />
<br />
<br />
Complete handshake reference:
![TLS handshake](https://zoompf.com/wp-content/uploads/2014/10/handshake.png "TLS handshake")

