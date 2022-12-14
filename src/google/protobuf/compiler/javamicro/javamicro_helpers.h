// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
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

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#ifndef GOOGLE_PROTOBUF_COMPILER_JAVA_HELPERS_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVA_HELPERS_H__

#include <string>
#include <google/protobuf/compiler/javamicro/javamicro_params.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javamicro {

// Commonly-used separator comments.  Thick is a line of '=', thin is a line
// of '-'.
extern const char kThickSeparator[];
extern const char kThinSeparator[];

// Converts the field's name to camel-case, e.g. "foo_bar_baz" becomes
// "fooBarBaz" or "FooBarBaz", respectively.
std::string UnderscoresToCamelCase(const FieldDescriptor* field);
std::string UnderscoresToCapitalizedCamelCase(const FieldDescriptor* field);

// Similar, but for method names.  (Typically, this merely has the effect
// of lower-casing the first letter of the name.)
std::string UnderscoresToCamelCase(const MethodDescriptor* method);

// Strips ".proto" or ".protodevel" from the end of a filename.
std::string StripProto(const std::string& filename);

// Gets the unqualified class name for the file.  Each .proto file becomes a
// single Java class, with all its contents nested in that class.
std::string FileClassName(const Params& params, const FileDescriptor* file);

// Returns the file's Java package name.
std::string FileJavaPackage(const Params& params, const FileDescriptor* file);

// Returns whether the Java outer class is needed, i.e. whether the option
// java_multiple_files is false, or the proto file contains any file-scope
// enums/extensions.
bool IsOuterClassNeeded(const Params& params, const FileDescriptor* file);

// Converts the given simple name of a proto entity to its fully-qualified name
// in the Java namespace, given that it is in the given file enclosed in the
// given parent message (or NULL for file-scope entities). Whether the file's
// outer class name should be included in the return value depends on factors
// inferrable from the given arguments, including is_class which indicates
// whether the entity translates to a Java class.
std::string ToJavaName(const Params& params, const std::string& name, bool is_class,
    const Descriptor* parent, const FileDescriptor* file);

// These return the fully-qualified class name corresponding to the given
// descriptor.
inline std::string ClassName(const Params& params, const Descriptor* descriptor) {
  return ToJavaName(params, descriptor->name(), true,
                    descriptor->containing_type(), descriptor->file());
}
std::string ClassName(const Params& params, const EnumDescriptor* descriptor);
inline std::string ClassName(const Params& params,
    const ServiceDescriptor* descriptor) {
  return ToJavaName(params, descriptor->name(), true, NULL, descriptor->file());
}
inline std::string ExtensionIdentifierName(const Params& params,
    const FieldDescriptor* descriptor) {
  return ToJavaName(params, descriptor->name(), false,
                    descriptor->extension_scope(), descriptor->file());
}
std::string ClassName(const Params& params, const FileDescriptor* descriptor);

// Get the unqualified name that should be used for a field's field
// number constant.
std::string FieldConstantName(const FieldDescriptor *field);

enum JavaType {
  JAVATYPE_INT,
  JAVATYPE_LONG,
  JAVATYPE_FLOAT,
  JAVATYPE_DOUBLE,
  JAVATYPE_BOOLEAN,
  JAVATYPE_STRING,
  JAVATYPE_BYTES,
  JAVATYPE_ENUM,
  JAVATYPE_MESSAGE
};

JavaType GetJavaType(FieldDescriptor::Type field_type);

inline JavaType GetJavaType(const FieldDescriptor* field) {
  return GetJavaType(field->type());
}

// Get the fully-qualified class name for a boxed primitive type, e.g.
// "java.lang.Integer" for JAVATYPE_INT.  Returns NULL for enum and message
// types.
const char* BoxedPrimitiveTypeName(JavaType type);

std::string DefaultValue(const Params& params, const FieldDescriptor* field);

}  // namespace javamicro
}  // namespace compiler
}  // namespace protobuf

}  // namespace google
#endif  // GOOGLE_PROTOBUF_COMPILER_JAVA_HELPERS_H__
