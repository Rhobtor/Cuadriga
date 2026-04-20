// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from ctl_mission_interfaces:srv/ConfigSimpleCtl.idl
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
#include "ctl_mission_interfaces/srv/detail/config_simple_ctl__struct.h"
#include "ctl_mission_interfaces/srv/detail/config_simple_ctl__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool ctl_mission_interfaces__srv__config_simple_ctl__request__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[70];
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
    assert(strncmp("ctl_mission_interfaces.srv._config_simple_ctl.ConfigSimpleCtl_Request", full_classname_dest, 69) == 0);
  }
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * ros_message = _ros_message;
  {  // k_angle
    PyObject * field = PyObject_GetAttrString(_pymsg, "k_angle");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->k_angle = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // points_in_window
    PyObject * field = PyObject_GetAttrString(_pymsg, "points_in_window");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->points_in_window = (int16_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // k_position
    PyObject * field = PyObject_GetAttrString(_pymsg, "k_position");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->k_position = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // v_forward
    PyObject * field = PyObject_GetAttrString(_pymsg, "v_forward");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->v_forward = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // l_ahead_dist
    PyObject * field = PyObject_GetAttrString(_pymsg, "l_ahead_dist");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->l_ahead_dist = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * ctl_mission_interfaces__srv__config_simple_ctl__request__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ConfigSimpleCtl_Request */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("ctl_mission_interfaces.srv._config_simple_ctl");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ConfigSimpleCtl_Request");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Request * ros_message = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Request *)raw_ros_message;
  {  // k_angle
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->k_angle);
    {
      int rc = PyObject_SetAttrString(_pymessage, "k_angle", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // points_in_window
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->points_in_window);
    {
      int rc = PyObject_SetAttrString(_pymessage, "points_in_window", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // k_position
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->k_position);
    {
      int rc = PyObject_SetAttrString(_pymessage, "k_position", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // v_forward
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->v_forward);
    {
      int rc = PyObject_SetAttrString(_pymessage, "v_forward", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // l_ahead_dist
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->l_ahead_dist);
    {
      int rc = PyObject_SetAttrString(_pymessage, "l_ahead_dist", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
// already included above
// #include <Python.h>
// already included above
// #include <stdbool.h>
// already included above
// #include "numpy/ndarrayobject.h"
// already included above
// #include "rosidl_runtime_c/visibility_control.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_simple_ctl__struct.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_simple_ctl__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool ctl_mission_interfaces__srv__config_simple_ctl__response__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[71];
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
    assert(strncmp("ctl_mission_interfaces.srv._config_simple_ctl.ConfigSimpleCtl_Response", full_classname_dest, 70) == 0);
  }
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * ros_message = _ros_message;
  {  // ack
    PyObject * field = PyObject_GetAttrString(_pymsg, "ack");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->ack = (Py_True == field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * ctl_mission_interfaces__srv__config_simple_ctl__response__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ConfigSimpleCtl_Response */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("ctl_mission_interfaces.srv._config_simple_ctl");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ConfigSimpleCtl_Response");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  ctl_mission_interfaces__srv__ConfigSimpleCtl_Response * ros_message = (ctl_mission_interfaces__srv__ConfigSimpleCtl_Response *)raw_ros_message;
  {  // ack
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->ack ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ack", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
