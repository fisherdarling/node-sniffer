#include <addon.h>
#include <nan.h>
#include <node.h>
#include <pcap.h>
#include <iostream>

using namespace v8;
using namespace Nan;

Callback *cb;

namespace addon {

// TODO: Implement runCallback to return packet data.
void runCallback(const struct pcap_pkthdr *header, const u_char *packet);

// Executes the user-defined callback. Currently implemented is a skeleton
// function that returns random data. Helps debug js calling.
void runCallback() {
  u_int size = 32;
  char *buff = new char[32];

  for (size_t i = 0; i < size; i++) buff[i] = rand() % 256;

  Nan::MaybeLocal<v8::Object> buffer = Nan::NewBuffer(buff, 32);

  Nan::MaybeLocal<v8::Object> *bufferptr = &buffer;

  std::cout << bufferptr << std::endl;

  const unsigned int argc = 1;                      // Arg count.
  Local<Value> argv[] = {buffer.ToLocalChecked()};  // Arg values.

  cb->Call(argc, argv);
}

// Sets the callback to be executed on a callback.
// TODO: Implement argument parsing and runtime errors.
NAN_METHOD(onPacket) {
  // Get the function.
  Local<Function> function = info[0].As<Function>();

  // Set the callback.
  cb = new Callback(function);
}

// The callback handler for libpcap.
void packetCallbackHandle(unsigned char *args, const struct pcap_pkthdr *header,
                          const unsigned char *packet) {
  // execute the defined callback with the new data.
  // runCallback(packet, header);
}

// Takes a string input and sets the filter string from that input.
NAN_METHOD(setFilter) {}

// Returns a list of strings the available system devices.
NAN_METHOD(getDevices) {}

// Sets the given string as the device name. Executes initDevice() after
// setting the device name.
NAN_METHOD(setDevice) {}

// Creates and initializes the set device.
void initDevice() {}

// Returns a dictionary of the 'name', 'ip', and 'netmask' of the given device.
NAN_METHOD(getDevProperties) {}

// Opens the device and creates its handle. The filter is compiled in this
// function.
NAN_METHOD(openDevice) {}

// Begins sniffing on the set device.
NAN_METHOD(beginSniffing) {}

// Closes the device and removes the handle. openDevice needs to be called again
// in order to sniff on the device the next time.
NAN_METHOD(closeDevice) {}

// Returns the pcap version being used.
NAN_METHOD(pcapVersion) {
  std::string version(pcap_lib_version());
  info.GetReturnValue().Set(New<String>(version).ToLocalChecked());
}

NAN_METHOD(runCallbackBuffer) {
  u_int size = 32;
  char *buff = new char[32];

  for (size_t i = 0; i < size; i++) buff[i] = rand() % 256;

  Nan::MaybeLocal<v8::Object> buffer = Nan::NewBuffer(buff, 32);

  Nan::MaybeLocal<v8::Object> *bufferptr = &buffer;

  std::cout << bufferptr << std::endl;

  const unsigned int argc = 1;                      // Arg count.
  Local<Value> argv[] = {buffer.ToLocalChecked()};  // Arg values.

  cb->Call(argc, argv);
}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, onPacket);
  NAN_EXPORT(target, setFilter);
  NAN_EXPORT(target, getDevices);
  NAN_EXPORT(target, setDevice);
  NAN_EXPORT(target, getDevProperties);
  NAN_EXPORT(target, openDevice);
  NAN_EXPORT(target, beginSniffing);
  NAN_EXPORT(target, closeDevice);
  NAN_EXPORT(target, pcapVersion);
  NAN_EXPORT(target, runCallbackBuffer);
}

NODE_MODULE(addon, Init)

}  // namespace addon
