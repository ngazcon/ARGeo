/*
 * Copyright (c) 2014 Spotify AB
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef __JniWeakGlobalRef_h__
#define __JniWeakGlobalRef_h__

#include "JniHelpersCommon.h"
#include "JavaThreadUtils.h"
#include "JniLocalRef.h"

namespace argeo {
namespace jni {

// RAII helper to maintain global references automatically.
template<typename JniType>
class EXPORT JniWeakGlobalRef {
 public:
  JniWeakGlobalRef() : _obj(NULL) {}
  JniWeakGlobalRef(const JniWeakGlobalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }
  JniWeakGlobalRef(const JniLocalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }

  ~JniWeakGlobalRef() { set(NULL); }

  JniType get() const { return _obj; }

  JniType leak() {
    JniType obj = _obj;
    _obj = NULL;
    return obj;
  }

  void set(JniType obj) {
    JNIEnv *env = NULL;
    if (_obj || obj) {
      env = JavaThreadUtils::getEnvForCurrentThread();
    }
    if (_obj) {
      if (env) {
        env->DeleteGlobalRef(_obj);
      }
      _obj = NULL;
    }
    if (obj && env) {
      _obj = (JniType)env->NewWeakGlobalRef(obj);
    }
  }

  operator JniType() const { return _obj; }

  void operator=(const JniLocalRef<JniType> &ref) { set(ref.get()); }

 private:
  JniType _obj;
};

} // namespace jni
} // namespace argeo

#endif // __JniWeakGlobalRef_h__
