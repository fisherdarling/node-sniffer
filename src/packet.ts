// var Parser = require("binary-parser").Parser;

// /**
//  *
//  * @param {Buffer} bin
//  */

// function ipv6(bin: Buffer) {}

class ip {
  version: String;
  h_length: Number;
}

// var ipv4 = new Parser()
//   .endianess("big")
//   .bit4("version")
//   .bit4("ihl")
//   .bit8("qos")
//   .bit16("length");
// //   .bit16("id")
// //   .bit3("frag")
// //   .bit16("offset")
// //   .bit8("ttl")
// //   .bit8("protocol")
// //   .bit16("checksum")
// //   .bit32("src")
// //   .bit32("dest");

// module.exports = {
//   process: function(bin) {
//     console.log("Parsing packet...");
//     var ipPacket = ipv4.parse(bin);
//     console.log(ipPacket);
//     console.log("Actual Length: " + Buffer.byteLength(bin));
//   }
// };

// console.log(Parser);
