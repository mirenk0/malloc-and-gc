#include "vm.h"

void vm_frame_push(vm_t *vm, frame_t *frame) { stack_push(vm->frames, frame); }

frame_t *vm_new_frame(vm_t *vm) {
  frame_t *new_frame = malloc(sizeof(frame_t));
  if (!new_frame) {
    exit(1); // Handle allocation failure
  }
  new_frame->references = stack_new(8);

  vm_frame_push(vm, new_frame);

  return new_frame;
}

void frame_free(frame_t *frame) {
  stack_free(frame->references);
  free(frame);
}

vm_t *vm_new() {
  vm_t *vm = malloc(sizeof(vm_t));
  if (vm == NULL) {
    return NULL;
  }

  vm->frames = stack_new(8);
  vm->objects = stack_new(8);
  return vm;
}

void vm_free(vm_t *vm) {
  for (int i = 0; i < vm->frames->count; i++) {
    frame_free(vm->frames->data[i]);
  }
  stack_free(vm->frames);
  stack_free(vm->objects);
  free(vm);
}
