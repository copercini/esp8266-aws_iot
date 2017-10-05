### Some examples using x.509 certificates and TLSv1.2 under Arduino IDE to communicate with AWS IoT

After AxTLS update to v2.0.0, the ESP8266 can work with TLS v1.2, the major restriction for this small device communicates with AWS IoT natively.


#### How can I get AWS IoT working in my ESP8266 with arduino IDE?

##### First: Update ESP8266 Arduino core to last git version, [Here are the instructions](https://github.com/esp8266/Arduino#using-git-version)

##### Second: Creating a thing, download and convert AWS IoT certificates to DER format
http://docs.aws.amazon.com/iot/latest/developerguide/create-device-certificate.html

Converting PEM to DER format: (On Windowns you should download openssl first)<br />
`$ openssl x509 -in aaaaaaaaa-certificate.pem.crt.txt -out cert.der -outform DER` <br />
`$ openssl rsa -in aaaaaaaaaa-private.pem.key -out private.der -outform DER` <br />

copy cert.der and private.der to sketch data folder and upload it to SPIFFS using [arduino-esp8266fs-plugin
](https://github.com/esp8266/arduino-esp8266fs-plugin)

##### Third: Uploading a arduino sketch
Some sketch examples are available in examples folder of this repository.
