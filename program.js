var addon = require("./build/Release/addon");

addon.setCallback(arg => {
  var buffer = Buffer.from(arg);
  console.log(buffer);
});

addon.runCallbackBuffer();
