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
console.log("Sniffing!");
