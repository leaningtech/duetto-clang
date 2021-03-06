// REQUIRES: powerpc-registered-target
// RUN: %clang_cc1 -triple powerpc64-unknown-linux-gnu -emit-llvm -o - %s | FileCheck %s

float crealf(_Complex float);
double creal(_Complex double);
long double creall(_Complex long double);

_Complex float foo_float(_Complex float x) {
  return x;
}

// CHECK: define %complex._ZTSf @foo_float(float {{[%A-Za-z0-9.]+}}, float {{[%A-Za-z0-9.]+}}) [[NUW:#[0-9]+]] {

_Complex double foo_double(_Complex double x) {
  return x;
}

// CHECK: define %complex._ZTSd @foo_double(double {{[%A-Za-z0-9.]+}}, double {{[%A-Za-z0-9.]+}}) [[NUW]] {

_Complex long double foo_long_double(_Complex long double x) {
  return x;
}

// CHECK: define %complex._ZTSe @foo_long_double(ppc_fp128 {{[%A-Za-z0-9.]+}}, ppc_fp128 {{[%A-Za-z0-9.]+}}) [[NUW]] {

_Complex int foo_int(_Complex int x) {
  return x;
}

// CHECK: define %complex._ZTSi @foo_int(i32 {{[%A-Za-z0-9.]+}}, i32 {{[%A-Za-z0-9.]+}}) [[NUW]] {

_Complex short foo_short(_Complex short x) {
  return x;
}

// CHECK: define %complex._ZTSs @foo_short(i16 {{[%A-Za-z0-9.]+}}, i16 {{[%A-Za-z0-9.]+}}) [[NUW]] {

_Complex signed char foo_char(_Complex signed char x) {
  return x;
}

// CHECK: define %complex._ZTSa @foo_char(i8 {{[%A-Za-z0-9.]+}}, i8 {{[%A-Za-z0-9.]+}}) [[NUW]] {

_Complex long foo_long(_Complex long x) {
  return x;
}

// CHECK: define %complex._ZTSl @foo_long(i64 {{[%A-Za-z0-9.]+}}, i64 {{[%A-Za-z0-9.]+}}) [[NUW]] {

_Complex long long foo_long_long(_Complex long long x) {
  return x;
}

// CHECK: define %complex._ZTSx @foo_long_long(i64 {{[%A-Za-z0-9.]+}}, i64 {{[%A-Za-z0-9.]+}}) [[NUW]] {

float bar_float(void) {
  return crealf(foo_float(2.0f - 2.5fi));
}

// CHECK: define float @bar_float() [[NUW]] {
// CHECK: [[VAR1:[%A-Za-z0-9.]+]] = call %complex._ZTSf @foo_float
// CHECK: extractvalue %complex._ZTSf [[VAR1]], 0
// CHECK: extractvalue %complex._ZTSf [[VAR1]], 1

double bar_double(void) {
  return creal(foo_double(2.0 - 2.5i));
}

// CHECK: define double @bar_double() [[NUW]] {
// CHECK: [[VAR2:[%A-Za-z0-9.]+]] = call %complex._ZTSd @foo_double
// CHECK: extractvalue %complex._ZTSd [[VAR2]], 0
// CHECK: extractvalue %complex._ZTSd [[VAR2]], 1

long double bar_long_double(void) {
  return creall(foo_long_double(2.0L - 2.5Li));
}

// CHECK: define ppc_fp128 @bar_long_double() [[NUW]] {
// CHECK: [[VAR3:[%A-Za-z0-9.]+]] = call %complex._ZTSe @foo_long_double
// CHECK: extractvalue %complex._ZTSe [[VAR3]], 0
// CHECK: extractvalue %complex._ZTSe [[VAR3]], 1

int bar_int(void) {
  return __real__(foo_int(2 - 3i));
}

// CHECK: define signext i32 @bar_int() [[NUW]] {
// CHECK: [[VAR4:[%A-Za-z0-9.]+]] = call %complex._ZTSi @foo_int
// CHECK: extractvalue %complex._ZTSi [[VAR4]], 0
// CHECK: extractvalue %complex._ZTSi [[VAR4]], 1

short bar_short(void) {
  return __real__(foo_short(2 - 3i));
}

// CHECK: define signext i16 @bar_short() [[NUW]] {
// CHECK: [[VAR5:[%A-Za-z0-9.]+]] = call %complex._ZTSs @foo_short
// CHECK: extractvalue %complex._ZTSs [[VAR5]], 0
// CHECK: extractvalue %complex._ZTSs [[VAR5]], 1

signed char bar_char(void) {
  return __real__(foo_char(2 - 3i));
}

// CHECK: define signext i8 @bar_char() [[NUW]] {
// CHECK: [[VAR6:[%A-Za-z0-9.]+]] = call %complex._ZTSa @foo_char
// CHECK: extractvalue %complex._ZTSa [[VAR6]], 0
// CHECK: extractvalue %complex._ZTSa [[VAR6]], 1

long bar_long(void) {
  return __real__(foo_long(2L - 3Li));
}

// CHECK: define i64 @bar_long() [[NUW]] {
// CHECK: [[VAR7:[%A-Za-z0-9.]+]] = call %complex._ZTSl @foo_long
// CHECK: extractvalue %complex._ZTSl [[VAR7]], 0
// CHECK: extractvalue %complex._ZTSl [[VAR7]], 1

long long bar_long_long(void) {
  return __real__(foo_long_long(2LL - 3LLi));
}

// CHECK: define i64 @bar_long_long() [[NUW]] {
// CHECK: [[VAR8:[%A-Za-z0-9.]+]] = call %complex._ZTSx @foo_long_long
// CHECK: extractvalue %complex._ZTSx [[VAR8]], 0
// CHECK: extractvalue %complex._ZTSx [[VAR8]], 1


// CHECK: attributes [[NUW]] = { noinline nounwind{{.*}} }
