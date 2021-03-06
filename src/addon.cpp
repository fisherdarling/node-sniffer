#include <addon.h>
#include <nan.h>
#include <node.h>
#include <pcap.h>
#include <iostream>

using namespace v8;
using namespace Nan;

Callback* cb;

namespace addon {

// void runCallback(const struct pcap_pkthdr *header, const u_char *packet);

// Executes the user-defined callback. Currently implemented is a skeleton
// function that returns random data. Helps debug js calling.
// void runCallback() {
//     u_int size = 32;
//     char* buff = new char[32];

//     for (size_t i = 0; i < size; i++) buff[i] = rand() % 256;

//     Nan::MaybeLocal<v8::Object> buffer = Nan::NewBuffer(buff, 32);

//     Nan::MaybeLocal<v8::Object>* bufferptr = &buffer;

//     std::cout << bufferptr << std::endl;

//     const unsigned int argc = 1;                        // Arg count.
//     Local<Value> argv[] = { buffer.ToLocalChecked() };  // Arg values.

//     cb->Call(argc, argv);
// }

// Sets the callback to be executed on a callback.
NAN_METHOD(onPacket) {
  // Check for the correct number of arguments.
  if (info.Length() != 1) {
    ThrowSyntaxError("onPacket() requires 1 argument.");
    return;
  }
  // Check that the argument is a function.
  if (!info[0]->IsFunction()) {
    ThrowSyntaxError("onPacket() requires a callback function.");
    return;
  }

  // Get the function.
  Local<Function> function = info[0].As<Function>();

  // Set the callback.
  cb = new Callback(function);
}

// The callback handler for libpcap.
void packetCallbackHandle(unsigned char* args, const struct pcap_pkthdr* header,
                          const unsigned char* packet) {
  // execute the defined callback with the new data.

  // Get the length of the capture.
  u_int size = header->caplen;
  // std::cout << "Size: " << size << std::endl;

  // Create the buffer to pass.
  Nan::MaybeLocal<v8::Object> packetData = Nan::CopyBuffer(
      reinterpret_cast<char*>(const_cast<unsigned char*>(packet)), size);
  // std::cout << "Deleting *packet." << std::endl;

  // std::cout << "Buffer *: " << &packetData << std::endl;

  // TODO: Add support for passing some sort of Packet object.

  const unsigned int argc = 1;                          // Arg count.
  Local<Value> argv[] = {packetData.ToLocalChecked()};  // Arg values.

  // std::cout << "Executing callback..." << std::endl;
  cb->Call(argc, argv);
  // delete *packetData.ToLocalChecked();
}

// Takes a string input and sets the filter string from that input.
NAN_METHOD(setFilter) {
  // Check if an argument was passed.
  if (info.Length() == 1) {
    // Check if the passed argmuent is a string.
    if (info[0]->IsString()) {
      Nan::Utf8String param1(info[0]->ToString());
      filterString = std::string(*param1);
      info.GetReturnValue().SetEmptyString();
      return;
      // Else set it to the default filter, ""
    } else {
      filterString = "";
      info.GetReturnValue().SetEmptyString();
      return;
    }
  }
}

// Returns a list of strings the available system devices.
NAN_METHOD(getDevices) {
  // Pointer to the first potential device.
  pcap_if_t* deviceList;

  // Get the devices.
  pcap_findalldevs(&deviceList, err);

  // TODO: Refactor to object? Find the length?
  Local<Object> retArr = Array::New(info.GetIsolate());

  int i = 0;  // Array index.
  for (pcap_if_t* curr = deviceList; curr; curr = curr->next) {
    if (curr != NULL) {
      // Get the device's data object.
      Local<Object> devObj = proccessDevice(curr, info.GetIsolate());

      // Set it at its index.
      retArr->Set(i, devObj);
      i++;
    }
  }

  info.GetReturnValue().Set(retArr);
}

// Returns a JS Object with properties of the sent device's value.
Local<Object> proccessDevice(pcap_if_t* dev, Isolate* iso) {
  Local<Object> retObj = Object::New(iso);

  // Get the device description.
  const char* devDesc = dev->description != NULL ? dev->description : "";

  // // Get and set the device flags.
  bpf_u_int32 flags = dev->flags;
  Local<Array> flagData = Array::New(iso);

  int index = 0;

  // "loopback" flag.
  if ((flags & 1) == 1) {
    flagData->Set(index, Nan::New("loopback").ToLocalChecked());
    index++;
  }

  // "up" flag.
  if ((flags & 2) == 2) {
    flagData->Set(index, Nan::New("up").ToLocalChecked());
    index++;
  }

  // "running" flag.
  if ((flags & 4) == 4) {
    flagData->Set(index, Nan::New("running").ToLocalChecked());
    index++;
  }

  // Set the device name.
  retObj->Set(Nan::New("name").ToLocalChecked(),
              Nan::New(dev->name).ToLocalChecked());

  // Set the description.
  retObj->Set(Nan::New("desc").ToLocalChecked(),
              Nan::New(devDesc).ToLocalChecked());

  // Set the flags.
  retObj->Set(Nan::New("flags").ToLocalChecked(), flagData);

  return retObj;
}

// TODO: Allow listening to any given device.
// Sets the given string as the device name. Executes initDevice() after
// setting the device name.
NAN_METHOD(setDevice) {
  // Check argument length.
  if (info.Length() > 1) {
    ThrowSyntaxError("setDevice() requires 0 or 1 argument(s).");
    return;
  }

  // If an argument was supplied.
  if (info.Length() == 1) {
    // If said argument is a string.
    if (info[0]->IsString()) {
      Nan::Utf8String param1(info[0]->ToString());
      device = const_cast<char*>(std::string(*param1).c_str());
      // std::cout << "Setting defined device: " << device << std::endl;
    } else {  // Argument wasn't a string.
      ThrowTypeError("setDevice() requires string arguments.");
      return;
    }
  } else {  // 0 arguments supplied, set device to first available.
    device = pcap_lookupdev(err);
    // std::cout << "Setting default device: " << device << std::endl;
  }

  // Device failed to be created.
  if (device == NULL) {
    ThrowError("Device could not be found.");
    return;
  }

  initDevice();
}

// Creates and initializes the set device.
void initDevice() { pcap_lookupnet(device, &ip, &netmask, err); }

// Returns a dictionary object of the 'name', 'ip',
// and 'netmask' of the given device.
NAN_METHOD(getDevProperties) {
  // Check if device is null or empty.
  if (device == NULL || std::string(device).length() == 0) {
    ThrowError("Device does not exist or is empty.");
    return;
  }

  // Create the return array.
  Local<Array> devStats = Array::New(info.GetIsolate(), 3);

  // Set the device name.
  devStats->Set(Nan::New("name").ToLocalChecked(),
                Nan::New(device).ToLocalChecked());

  // Convert and set the device ip.
  struct in_addr addr;
  addr.s_addr = ip;
  char* ipString = inet_ntoa(addr);
  devStats->Set(Nan::New("ip").ToLocalChecked(),
                Nan::New(ipString).ToLocalChecked());

  // Convert and set the device netmask.
  struct in_addr nmaddr;
  nmaddr.s_addr = netmask;
  char* nmString = inet_ntoa(nmaddr);
  devStats->Set(Nan::New("netmask").ToLocalChecked(),
                Nan::New(nmString).ToLocalChecked());

  info.GetReturnValue().Set(devStats);
}

// TODO: Check for failures.
// Opens the device and creates its handle. The filter is compiled in this
// function.

// Returns the status (int) of the pcap_activate() function.
NAN_METHOD(openDev) {
  // std::cout << "Creating handle..." << std::endl << std::endl;

  // Create the handle.
  if (device == NULL) {
    Nan::ThrowError("Device is not defined.");
    return;
  }

  handle = pcap_create(device, err);
  int status = pcap_activate(handle);

  // Compile the filter.
  // std::cout << "Compiling filter..." << std::endl;
  if (pcap_compile(handle, &filter, filterString.c_str(), 0, ip) == -1) {
    std::string errormsg = "Filter could not be compiled: ";
    errormsg += pcap_geterr(handle);
    Nan::ThrowError(errormsg.c_str());
  }
  // std::cout << filterString.c_str() << std::endl;

  // Set the filter.
  // std::cout << "Setting filter..." << std::endl;
  pcap_setfilter(handle, &filter);
  // std::cout << &filter << std::endl;

  // Set the snaplength.
  // std::cout << "Setting snapshot length..." << std::endl;
  pcap_set_snaplen(handle, 65535);
  // std::cout << &handle << std::endl;

  // Return the function.
  // std::cout << "Setting return value..." << std::endl;
  info.GetReturnValue().Set(New<Integer>(status));
}

// Begins sniffing on the set device.
NAN_METHOD(sniff) {
  int numPackets = -1;

  // Check that only one argument was given.
  if (info.Length() == 1) {
    if (info[0]->IsNumber())
      numPackets = info[0]->IntegerValue();
    else
      ThrowSyntaxError(
          "sniff() requires either no arguments or a single argument; the "
          "number of packets to capture.");
  }

  // Start sniffing.
  pcap_loop(handle, numPackets, packetCallbackHandle, NULL);
}

// Closes the device and removes the handle. openDevice needs to be called
// again in order to sniff on the device the next time.
NAN_METHOD(closeDev) { pcap_breakloop(handle); }

// Returns the pcap version being used.
NAN_METHOD(version) {
  info.GetReturnValue().Set(New<String>(pcap_lib_version()).ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, onPacket);
  NAN_EXPORT(target, setFilter);
  NAN_EXPORT(target, getDevices);
  NAN_EXPORT(target, setDevice);
  NAN_EXPORT(target, getDevProperties);
  NAN_EXPORT(target, openDev);
  NAN_EXPORT(target, sniff);
  NAN_EXPORT(target, closeDev);
  NAN_EXPORT(target, version);
}

NODE_MODULE(addon, Init)

}  // namespace addon
