#include "snekobject.h"

void snek_object_free(snek_object_t *obj) {
  switch (obj->kind) {
  // Integers and floats are allocated within the object, so nothing to free
  case INTEGER:
  case FLOAT:
    break;
  case STRING:
    // Free the allocated data for the string
    free(obj->data.v_string);
    break;
  case VECTOR3: {
    // Since x, y, and z are all allocated within the `snek_object_t`,
    // there is nothing additional for us to free here!
    break;
  }
  case ARRAY: {
    // Free the element list, but not the elements themselves.
    //  The rest of the mark-and-sweep algorithm will take care of that.
    snek_array_t *array = &obj->data.v_array;
    free(array->elements);

    break;
  }
  }

  free(obj);
}
