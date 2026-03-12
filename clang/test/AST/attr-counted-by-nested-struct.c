// RUN: %clang_cc1 -fexperimental-bounds-safety-expressions %s -ast-dump | FileCheck %s

#define __counted_by(f)  __attribute__((counted_by(f)))

struct inner {
  int len;
};

// CHECK-LABEL: RecordDecl {{.+}} struct basic_nested definition
// CHECK-NEXT: |-FieldDecl {{.+}} referenced s 'struct inner'
// CHECK-NEXT: `-FieldDecl {{.+}} data 'int[] __counted_by(s.len)':'int[]'
struct basic_nested {
  struct inner s;
  int data[] __counted_by(s.len);
};

struct level2 { int count; };
struct level1 { struct level2 l2; };

// CHECK-LABEL: RecordDecl {{.+}} struct deep_nested definition
// CHECK-NEXT: |-FieldDecl {{.+}} referenced l1 'struct level1'
// CHECK-NEXT: `-FieldDecl {{.+}} data 'int[] __counted_by(l1.l2.count)':'int[]'
struct deep_nested {
  struct level1 l1;
  int data[] __counted_by(l1.l2.count);
};

// CHECK-LABEL: RecordDecl {{.+}} struct simple definition
// CHECK-NEXT: |-FieldDecl {{.+}} referenced len 'int'
// CHECK-NEXT: `-FieldDecl {{.+}} data 'int[] __counted_by(len)':'int[]'
struct simple {
  int len;
  int data[] __counted_by(len);
};

// CHECK-LABEL: RecordDecl {{.+}} struct anon_struct definition
// CHECK-NEXT: |-RecordDecl {{.+}} struct definition
// CHECK-NEXT: | `-FieldDecl {{.+}} referenced len 'int'
// CHECK-NEXT: |-FieldDecl {{.+}} referenced s 'struct (unnamed at {{.+}})'
// CHECK-NEXT: `-FieldDecl {{.+}} data 'int[] __counted_by(s.len)':'int[]'
struct anon_struct {
  struct { int len; } s;
  int data[] __counted_by(s.len);
};
