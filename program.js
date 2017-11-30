var sniffer = require("./build/Release/sniffer");
var packet = require("./src/packet");

console.log(sniffer.version());
console.log("Running program...");

sniffer.onPacket(arg => {
  var buf = Buffer.from(arg);
  console.log(buf);
  packet.process(buf);
  //   numPackets += 1;
});

sniffer.setFilter("tcp");
sniffer.setDevice();

// console.log(JSON.stringify(sniffer.getDevProperties()));
sniffer.openDev();
sniffer.sniff();
