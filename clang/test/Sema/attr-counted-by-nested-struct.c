// RUN: %clang_cc1 -fexperimental-bounds-safety-expressions -fsyntax-only -verify=expr %s
// RUN: %clang_cc1 -fsyntax-only -verify=std %s

#define __counted_by(f)  __attribute__((counted_by(f)))

struct inner { int len; };

// Arrow notation not supported
struct arrow_test {
  struct inner *s;
  int data[] __counted_by(s->len);  // expr-error{{arrow notation not allowed for struct member in count parameter}}
                                    // std-error@-1{{'counted_by' argument must be a simple declaration reference}}
};

// Union members not supported
union u { int a; float b; };
struct union_test {
  union u u;
  int data[] __counted_by(u.a); // expr-error{{count parameter refers to union 'u' of type 'union u'}}
                                // std-error@-1{{'counted_by' argument must be a simple declaration reference}}
};

// Pointer fields not supported
struct ptr_member_test {
  struct inner s;
  int *__counted_by(s.len) p; // expr-error{{invalid argument expression to bounds attribute}}
                              // expr-note@-1{{nested struct member in count parameter only supported for flexible array members}}
                              // std-error@-2{{'counted_by' argument must be a simple declaration reference}}
};

// Non-integer counts not supported
struct non_int_leaf {
  struct inner s;
  int data[] __counted_by(s); // expr-error{{'counted_by' requires a non-boolean integer type argument}}
                              // std-error@-1{{'counted_by' requires a non-boolean integer type argument}}
};
struct bool_struct {
  _Bool b;
};
struct bool_leaf {
  struct bool_struct s;
  int data[] __counted_by(s); // expr-error{{'counted_by' requires a non-boolean integer type argument}}
                              // std-error@-1{{'counted_by' requires a non-boolean integer type argument}}
};

// Bad anon struct
struct anon_struct_nonexistent_leaf {
  struct { int len; } s;
  int data[] __counted_by(s.ben); // expr-error{{no member named 'ben'}}
                                  // expr-error@-1{{'counted_by' requires a non-boolean integer type argument}}
                                  // std-error@-2{{no member named 'ben'}}
                                  // std-error@-3{{'counted_by' requires a non-boolean integer type argument}}
};

struct expr_leaf {
  struct inner s;
  int data[] __counted_by(s.len + 2); // expr-error{{'counted_by' argument must be a simple declaration reference}}
                                      // std-error@-1{{'counted_by' argument must be a simple declaration reference}}
};
