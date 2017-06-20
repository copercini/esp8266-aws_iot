## Working example for ESP32: https://github.com/copercini/esp32-iot-examples/
--------------------------------------------------------------------------------------------


# Some examples using x.509 certificates and TLSv1.2 under Arduino IDE to communicate with AWS iot

After AxTLS update to v2.0.0, the ESP8266 can work with TLS v1.2, the major restriction for this small device communicates with AWS iot natively. You can follow the port here: https://github.com/igrr/axtls-8266

Due heap limitation, the ESP8266 is rebooting before complete handshake, I am hopeful it can be fixed with some optimization.

## How I can get TLS v1.2 working in my ESP8266 with arduino IDE?

#### First: Update libaxtls.a to last version (b20140f)
You can compile it as you own: add xtensa toolchain to your path, and run make. <br />
Or download a compiled one in the folder axtls of this git and replace it on Arduino\hardware\esp8266com\esp8266\tools\sdk\lib of your Arduino git installation (https://github.com/esp8266/Arduino#using-git-version)


#### Second: Creating a thing, Downloading and converting AWS iot certificates to DER format
http://docs.aws.amazon.com/iot/latest/developerguide/create-device-certificate.html

Converting PEM to DER format: <br />
$ wget https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem > ca.crt <br />
$ openssl x509 -in ca.crt -out certificates/ca.der -outform DER <br />
$ openssl x509 -in aaaaaaaaa-certificate.pem.crt -out certificates/cert.der -outform DER <br />
$ openssl rsa -in aaaaaaaaaa-private.pem.key -out certificates/private.der -outform DER <br />


#### Third: Uploading a arduino sketch
Some sketch examples are available in examples folder of this git.


## Why it doesn't work yet?
Because AxTLS doens't support the AWS ioT required cipher suites.

State: receiving Server Hello (2) <br />
State: receiving Certificate (11) <br />
State: receiving Certificate Request (13) <br />
State: receiving Server Hello Done (14) <br />
State: sending Certificate (11) <br />
State:	sending Client Key Exchange (16)<br />
State:	sending Finished (16)<br />
Alert: handshake failure<br />
Error: SSL error 40<br />
<br />
Based on Issue: https://github.com/esp8266/Arduino/issues/2771 <br />
Means: AxTLS library doesn't support needed cipher suites <br />
<br />
