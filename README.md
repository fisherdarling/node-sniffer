# node-sniffer

_node-sniffer_ will provide up-to-date bindings for the libpcap and WinPcap c
libraries. Support for this project will be long-term, with goals to support the
major networking protocols, including ipv6.

### Prerequisites

---

1. Linux system.
2. `nodejs`.
3. `libpcap` c library.
4. `build-tools` package must be installed (make, gcc, etc.).
5. `node-gyp` npm package.
   ```
   npm install node-gyp -g
   ```

## Installation

---

1. Clone the repository:
   ```
   $ git clone https://github.com/harpo109/node-sniffer.git
   ```
2. Install the node headers:

   ```
   $ node-gyp install
   ```

3. Build the addon from the root directory:
   ```
   $ node-gyp rebuild
   ```
4. `require` the addon in your javascript code:

   ```javascript
   var sniffer = require("./build/Release/sniffer");
   ```

## Usage

---

_node-sniffer_ requires a callback to be defined before sniffing can begin. This
callback will recieve the raw Packet information in the form of a Node Buffer.

### Example code:

```javascript
var sniffer = require("./build/Release/sniffer");
console.log(sniffer.version() + "\n");

// Set the packet callback.
sniffer.onPacket(arg => {
  var packet = Buffer.from(arg);
  // Do something with the packet...
});

// Set the filter to filter the incoming packets with.
// See the libpcap filter page for help.
sniffer.setFilter("tcp");

// Set the device. No argument will set the first available device.
sniffer.setDevice();

// Open and prepare the device to be sniffed.
// Compiles the filter in this call.
sniffer.openDev();

// Start sniffing the device until closeDev() is called. An integer argument will
// sniff that number of packets.
sniffer.sniff();
```

## Contributors

---

[Fisher Darling](https://github.com/harpo109) - _Main Author_

## License

---

```
MIT License

Copyright (c) 2017 Fisher Darling

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
