var sniffer = require("./build/Release/sniffer");

// console.log(sniffer);
// console.log();

var numPackets = 1;

console.log(sniffer.version() + "\n");

sniffer.onPacket(arg => {
  var buffer = Buffer.from(arg);
  console.log(
    "New Packet (" + numPackets + ")! Length: " + buffer.length + "\n"
  );
  numPackets += 1;

  if (numPackets > 10) {
    sniffer.closeDev();
  }
});

sniffer.setFilter("tcp");
sniffer.setDevice();
sniffer.openDev();
sniffer.sniff();

// sniffer.runCallbackBuffer();
