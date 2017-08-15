#include <string.h>
#include <node.h>
#include "judyarray.h"
#include "judy.h"
using namespace v8;


const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

Persistent<Function> JudyArray::constructor;

JudyArray::JudyArray(int size, int depth) : size_(size), depth_(depth) {
  //this->container = jg_init(size, depth);
  this->container = NULL;
}

JudyArray::~JudyArray() {
  if (this->container) {
    jg_close(this->container);
    this->container = NULL;
  }
}

void JudyArray::Init(Handle<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "JudyArray"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "get", GetValue);
  NODE_SET_PROTOTYPE_METHOD(tpl, "set", SetValue);
  NODE_SET_PROTOTYPE_METHOD(tpl, "delete", DeleteValue);
  NODE_SET_PROTOTYPE_METHOD(tpl, "keys", Keys);
  NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "JudyArray"),
               tpl->GetFunction());
}

void JudyArray::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Judy(...)`
    int size = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    int depth = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();

    JudyArray* obj = new JudyArray(size, depth);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 2;
    Local<Value> argv[argc] = { args[0], args[1] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void JudyArray::GetValue(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  JudyArray* obj = ObjectWrap::Unwrap<JudyArray>(args.Holder());
  struct store *st;

  // Check the number of arguments passed.
  if (args.Length() < 1) {
    args.GetReturnValue().SetUndefined();
    return;
  }

  String::Utf8Value arg0(args[0]);
  const char* key = ToCString(arg0);

  if (!obj->container) {
    obj->container = jg_init(obj->size_, obj->depth_);
  }

  if (obj->container) {
    st = jg_get((Judy *) obj->container, (uint8_t *) key);

    if (st) {
      args.GetReturnValue().Set(String::NewFromOneByte(isolate, (const uint8_t *) st->ptr));
    } else {
      args.GetReturnValue().SetUndefined();
    }
  } else {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Internal Error: Missing Container")));
  }
}

void JudyArray::SetValue(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  JudyArray* obj = ObjectWrap::Unwrap<JudyArray>(args.Holder());
  bool ret = false;

  // Check the number of arguments passed.
  if (args.Length() < 2) {
    args.GetReturnValue().SetUndefined();
    return;
  }

  String::Utf8Value arg0(args[0]);
  const char* key = ToCString(arg0);
  String::Utf8Value arg1(args[1]);
  const char* value = ToCString(arg1);

  if (!obj->container) {
    obj->container = jg_init(obj->size_, obj->depth_);
  }

  if (obj->container) {
    ret = jg_set(obj->container, (uint8_t *) key, (uint8_t *) value, (uint16_t) strlen((const char *) value) + 1, (uint8_t) TYPE_STRING);

  } else {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Internal Error: Missing Container")));
  }

  args.GetReturnValue().Set(Boolean::New(isolate, ret));
}

void JudyArray::DeleteValue(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  JudyArray* obj = ObjectWrap::Unwrap<JudyArray>(args.Holder());

  // Check the number of arguments passed.
  if (args.Length() < 1) {
    args.GetReturnValue().SetUndefined();
    return;
  }

  String::Utf8Value arg0(args[0]);
  const char* key = ToCString(arg0);

  if (!obj->container) {
    obj->container = jg_init(obj->size_, obj->depth_);
  }

  if (obj->container) {
    jg_delete((Judy *) obj->container, (uint8_t *) key);
  } else {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Internal Error: Missing Container")));
  }

  args.GetReturnValue().SetUndefined();
}

void InitAll(Handle<Object> exports) {
  JudyArray::Init(exports);
}

void JudyArray::Keys(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  JudyArray* obj = ObjectWrap::Unwrap<JudyArray>(args.Holder());
  uchar buf[1024];
  uint32_t counter = 0;
  JudySlot *cell = (JudySlot *)judy_strt((Judy *)obj->container, NULL, 0);
  Local<Array> arr = Array::New(isolate);

  while (cell) {
    judy_key((Judy *)obj->container, buf, 1024);
    Local<String> val = String::NewFromUtf8(isolate, (char *) buf);
    arr->Set(counter, val);
    cell = judy_nxt((Judy *)obj->container);
    counter++;
  }

  args.GetReturnValue().Set(arr);
}

void JudyArray::Size(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  JudyArray* obj = ObjectWrap::Unwrap<JudyArray>(args.Holder());
  uint32_t counter = 0, memory = 0;
  JudySlot *cell = (JudySlot *)judy_strt((Judy *)obj->container, NULL, 0);
  Local<Object> object = Object::New(isolate);
  struct store *data;
  uchar buf[1024];

  while (cell) {
    data = (struct store *) *cell;
    counter++;
    memory += data->length;

    judy_key((Judy *)obj->container, buf, 1024);
    memory += (strlen((char *)buf) + 1);

    cell = judy_nxt((Judy *)obj->container);
  }

  object->Set(String::NewFromUtf8(isolate, "keyCount"), Number::New(isolate, counter));
  object->Set(String::NewFromUtf8(isolate, "memoryAllocated"), Number::New(isolate, memory));

  args.GetReturnValue().Set(object);
}

NODE_MODULE(judyarray, InitAll)
