# Some examples using x.509 certificates and TLSv1.2 under Arduino IDE to communicate with AWS iot

After AxTLS update to v2.0.0, the ESP8266 can work with TLS v1.2, the major restriction for this small device communicates with AWS iot natively. You can follow the port here: https://github.com/igrr/axtls-8266


## How I can get AWS IoT working in my ESP8266 with arduino IDE?

#### First: Update ESP8266 Arduino core to last git version, [Here are the instructions](https://github.com/esp8266/Arduino#using-git-version)

#### Second: Creating a thing, Downloading and converting AWS iot certificates to DER format
http://docs.aws.amazon.com/iot/latest/developerguide/create-device-certificate.html

Converting PEM to DER format: <br />
$ openssl x509 -in aaaaaaaaa-certificate.pem.crt.txt -out cert.der -outform DER <br />
$ openssl rsa -in aaaaaaaaaa-private.pem.key -out private.der -outform DER <br />

copy cert.der and private.der to data folder

#### Third: Uploading a arduino sketch
Some sketch examples are available in examples folder of this git.

