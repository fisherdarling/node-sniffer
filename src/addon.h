#ifndef ADDON_H
#define ADDON_H

#include <nan.h>
#include <node.h>
#include <pcap.h>
#include <iostream>

namespace addon {

// The handle that controls the current listening session.
pcap_t *handle;

// The device that will be listened on.
char *device;

// Any error message from libpcap.
char err[PCAP_ERRBUF_SIZE];

// The packet filter to compile.
std::string filterString;

// The compiled filter.
struct bpf_program filter;

// The network mask of the device.
bpf_u_int32 netmask;

// The ipv4 address of the device.
bpf_u_int32 ip;

// Executes the user defined callback.
void runCallback();

// Sets the callback to be executed on a new packet.
NAN_METHOD(onPacket);

void packetCallbackHandle(unsigned char *, const struct pcap_pkthdr *,
                          const unsigned char *);

// Sets the filter that the packets will be filtered against.
NAN_METHOD(setFilter);

// Returns a list of devices that can be listened on, null if there are no
// avaliable devices.
NAN_METHOD(getDevices);

// Set the device to listen on.
NAN_METHOD(setDevice);

// Initializes and retrieves the properties of the given device.
void initDevice();

// Returns the name, ip, and netmask of the set device.
NAN_METHOD(getDevProperties);

// Compiles and opens the chosen device. If no device is given, opens the
// first available system device.
NAN_METHOD(openDevice);

// Begins listening on the opened device.
NAN_METHOD(beginSniffing);

// Closes the device being sniffed on.
NAN_METHOD(closeDevice);

NAN_MODULE_INIT(Init);

}  // namespace addon

#endif