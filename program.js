var addon = require("./build/Release/sniffer");

console.log(addon);
console.log();

console.log(addon.pcapVersion() + "\n");

addon.onPacket(arg => {
  var buffer = Buffer.from(arg);
  console.log(buffer);
  console.log();
});

addon.runCallbackBuffer();
