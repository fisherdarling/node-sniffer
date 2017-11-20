var addon = require("./build/Release/sniffer");

addon.onPacket(arg => {
  var buffer = Buffer.from(arg);
  console.log(buffer);
});

addon.runCallbackBuffer();
