var sniffer = require("./build/Release/sniffer");

var proc = require("./src/packet");

// console.log(sniffer);
// console.log();

// var arr = Array.from(sniffer.getDevices());
var arr = Array.from(sniffer.getDevices());

// console.log(typeof arr);
// console.log(arr);

// var numPackets = 1;

// console.log(sniffer.version() + "\n");

sniffer.onPacket(arg => {
  var buffer = Buffer.from(arg);
  console.log(
    "New Packet (" + numPackets + ")! Length: " + buffer.length + "\n"
  );
  numPackets += 1;
});

sniffer.setFilter("tcp");
sniffer.setDevice();

console.log(JSON.stringify(sniffer.getDevProperties()));
sniffer.openDev();
sniffer.sniff();

sniffer.runCallbackBuffer();
