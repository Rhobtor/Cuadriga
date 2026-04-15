// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from cuadriga_ctl_platform_interfaces:msg/KinematicDebugMsg.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__struct.h"
#include "cuadriga_ctl_platform_interfaces/msg/detail/kinematic_debug_msg__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool cuadriga_ctl_platform_interfaces__msg__kinematic_debug_msg__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[76];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("cuadriga_ctl_platform_interfaces.msg._kinematic_debug_msg.KinematicDebugMsg", full_classname_dest, 75) == 0);
  }
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * ros_message = _ros_message;
  {  // throttle
    PyObject * field = PyObject_GetAttrString(_pymsg, "throttle");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->throttle = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // steering
    PyObject * field = PyObject_GetAttrString(_pymsg, "steering");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->steering = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // requested_throttle_acc
    PyObject * field = PyObject_GetAttrString(_pymsg, "requested_throttle_acc");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->requested_throttle_acc = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // requested_steering_acc
    PyObject * field = PyObject_GetAttrString(_pymsg, "requested_steering_acc");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->requested_steering_acc = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // throttle_limited
    PyObject * field = PyObject_GetAttrString(_pymsg, "throttle_limited");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->throttle_limited = (Py_True == field);
    Py_DECREF(field);
  }
  {  // steering_limited
    PyObject * field = PyObject_GetAttrString(_pymsg, "steering_limited");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->steering_limited = (Py_True == field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * cuadriga_ctl_platform_interfaces__msg__kinematic_debug_msg__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of KinematicDebugMsg */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("cuadriga_ctl_platform_interfaces.msg._kinematic_debug_msg");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "KinematicDebugMsg");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg * ros_message = (cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg *)raw_ros_message;
  {  // throttle
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->throttle);
    {
      int rc = PyObject_SetAttrString(_pymessage, "throttle", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // steering
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->steering);
    {
      int rc = PyObject_SetAttrString(_pymessage, "steering", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // requested_throttle_acc
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->requested_throttle_acc);
    {
      int rc = PyObject_SetAttrString(_pymessage, "requested_throttle_acc", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // requested_steering_acc
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->requested_steering_acc);
    {
      int rc = PyObject_SetAttrString(_pymessage, "requested_steering_acc", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // throttle_limited
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->throttle_limited ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "throttle_limited", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // steering_limited
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->steering_limited ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "steering_limited", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
