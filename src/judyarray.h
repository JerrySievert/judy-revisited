#pragma once

using namespace v8;

#include <node.h>
#include <node_object_wrap.h>
#include "glue.h"

class JudyArray : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit JudyArray(int size = MAX_JUDY_SIZE, int depth = 0);
  ~JudyArray();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void GetValue(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetValue(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void DeleteValue(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Keys(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Size(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Setup();
  static v8::Persistent<v8::Function> constructor;
  void *container;
  int size_;
  int depth_;
};
