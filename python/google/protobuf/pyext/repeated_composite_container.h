// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: anuraag@google.com (Anuraag Agrawal)
// Author: tibell@google.com (Johan Tibell)

#ifndef GOOGLE_PROTOBUF_PYTHON_CPP_REPEATED_COMPOSITE_CONTAINER_H__
#define GOOGLE_PROTOBUF_PYTHON_CPP_REPEATED_COMPOSITE_CONTAINER_H__

#include <Python.h>

#include <memory>
#include <string>
#include <vector>

#include <google/protobuf/pyext/message.h>

namespace yi {
namespace google {
namespace protobuf {

class FieldDescriptor;
class Message;

namespace python {

struct CMessageClass;

// A RepeatedCompositeContainer always has a parent message.
// The parent message also caches reference to items of the container.
typedef struct RepeatedCompositeContainer : public ContainerBase {
  // The type used to create new child messages.
  CMessageClass* child_message_class;
} RepeatedCompositeContainer;

extern PyTypeObject RepeatedCompositeContainer_Type;

namespace repeated_composite_container {

// Builds a RepeatedCompositeContainer object, from a parent message and a
// field descriptor.
RepeatedCompositeContainer* NewContainer(
    CMessage* parent,
    const FieldDescriptor* parent_field_descriptor,
    CMessageClass *child_message_class);

// Appends a new CMessage to the container and returns it.  The
// CMessage is initialized using the content of kwargs.
//
// Returns a new reference if successful; returns NULL and sets an
// exception if unsuccessful.
PyObject* Add(RepeatedCompositeContainer* self,
              PyObject* args,
              PyObject* kwargs);

// Appends all the CMessages in the input iterator to the container.
//
// Returns None if successful; returns NULL and sets an exception if
// unsuccessful.
PyObject* Extend(RepeatedCompositeContainer* self, PyObject* value);

// Appends a new message to the container for each message in the
// input iterator, merging each data element in. Equivalent to extend.
//
// Returns None if successful; returns NULL and sets an exception if
// unsuccessful.
PyObject* MergeFrom(RepeatedCompositeContainer* self, PyObject* other);

// Accesses messages in the container.
//
// Returns a new reference to the message for an integer parameter.
// Returns a new reference to a list of messages for a slice.
PyObject* Subscript(RepeatedCompositeContainer* self, PyObject* slice);

// Deletes items from the container (cannot be used for assignment).
//
// Returns 0 on success, -1 on failure.
int AssignSubscript(RepeatedCompositeContainer* self,
                    PyObject* slice,
                    PyObject* value);
}  // namespace repeated_composite_container
}  // namespace python
}  // namespace protobuf
} // namespace google
} // namespace yi

#endif  // GOOGLE_PROTOBUF_PYTHON_CPP_REPEATED_COMPOSITE_CONTAINER_H__
