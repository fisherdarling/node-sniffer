var sniffer = require("./build/Release/sniffer");

// console.log(sniffer);
// console.log();

var arr = sniffer.getDevices();

console.log(arr);

// var numPackets = 1;

// console.log(sniffer.version() + "\n");

// sniffer.onPacket(arg => {
//   var buffer = Buffer.from(arg);
//   console.log(
//     "New Packet (" + numPackets + ")! Length: " + buffer.length + "\n"
//   );
//   numPackets += 1;
// });

// sniffer.setFilter("tcp");
// sniffer.setDevice();
// sniffer.openDev();
// sniffer.sniff();

// sniffer.runCallbackBuffer();
