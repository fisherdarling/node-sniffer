# node-sniffer

_node-sniffer_ provides primitive bindings to libpcap. The future goal for this project is to create a fast sniffing library for the nodejs ecosystem.

## Prerequisites

---

1.  Linux system
2.  `nodejs`
3.  `libpcap` library.
4.  `build-tools` (make, gcc, etc.)
5.  `node-gyp` npm package

    ```
    npm install node-gyp -g
    ```

## Installation

---

1.  Clone the repository:

    ```
    $ git clone https://github.com/fisherdarling/node-sniffer.git
    ```

2.  Install the build-dependencies (nan)

    ```
    $ npm install
    ```

3.  Install the node headers:

    ```
    $ node-gyp install
    ```

4.  Build the addon from the root directory:

    ```
    $ node-gyp rebuild
    ```

5.  `require` the addon in your javascript code:

    ```javascript
    var sniffer = require("./build/Release/sniffer");
    ```

## Usage

---

_node-sniffer_ requires a callback to be defined before sniffing can begin. This
callback will recieve the raw Packet information in the form of a Node Buffer. Any program must be run with sufficient privaledges to sniff on the given device. i.e. `sudo node program.js`

### Example code (from program.js):

```javascript
var sniffer = require("./build/Release/sniffer");
console.log(sniffer.version() + "\n");

// Set the packet callback.
sniffer.onPacket(arg => {
  var packet = Buffer.from(arg);
  console.log(packet);
  // Do something with the packet...
});

// Set the filter to filter the incoming packets with.
// See the libpcap filter page for help.
sniffer.setFilter("tcp");
console.log("Set Filter");

// Set the device. No argument will set the first available device.
sniffer.setDevice();
console.log("Set Device");

// Open and prepare the device to be sniffed.
// Compiles the filter in this call.
sniffer.openDev();
console.log("Opened Device.");

// Start sniffing the device until closeDev() is called. An integer argument will
// sniff that number of packets.
sniffer.sniff(10);
console.log("Finished Sniffing!");
```

## Contributors

---

- [Fisher Darling](https://github.com/fisherdarling)

## License

---

```
MIT License

Copyright (c) 2018 Fisher Darling <fdarlingco@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
