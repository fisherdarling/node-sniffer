#include <nan.h>
#include <node.h>
#include <iostream>

using namespace v8;
using namespace Nan;

Callback *cb;

namespace addon {

NAN_METHOD(setCallback) {
  // Get the function.
  Local<Function> function = info[0].As<Function>();

  // Set the callback.
  cb = new Callback(function);
}

NAN_METHOD(runCallback) {
  const unsigned int argc = 0;  // Arg count.
  Local<Value> argv[] = {};     // Arg values.

  cb->Call(argc, argv);
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
  NAN_EXPORT(target, setCallback);
  NAN_EXPORT(target, runCallback);
  NAN_EXPORT(target, runCallbackBuffer);
}

NODE_MODULE(addon, Init)

}  // namespace addon
