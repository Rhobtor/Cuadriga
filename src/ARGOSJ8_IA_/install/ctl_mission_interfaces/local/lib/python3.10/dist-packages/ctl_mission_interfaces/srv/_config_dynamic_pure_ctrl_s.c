// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from ctl_mission_interfaces:srv/ConfigDynamicPureCtrl.idl
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
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__struct.h"
#include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool ctl_mission_interfaces__srv__config_dynamic_pure_ctrl__request__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[83];
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
    assert(strncmp("ctl_mission_interfaces.srv._config_dynamic_pure_ctrl.ConfigDynamicPureCtrl_Request", full_classname_dest, 82) == 0);
  }
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request * ros_message = _ros_message;
  {  // look_ahead_dis
    PyObject * field = PyObject_GetAttrString(_pymsg, "look_ahead_dis");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->look_ahead_dis = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // max_v_forward
    PyObject * field = PyObject_GetAttrString(_pymsg, "max_v_forward");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->max_v_forward = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // max_ang_acc
    PyObject * field = PyObject_GetAttrString(_pymsg, "max_ang_acc");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->max_ang_acc = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // max_ang_dec
    PyObject * field = PyObject_GetAttrString(_pymsg, "max_ang_dec");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->max_ang_dec = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // max_lin_acc
    PyObject * field = PyObject_GetAttrString(_pymsg, "max_lin_acc");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->max_lin_acc = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // max_lin_dec
    PyObject * field = PyObject_GetAttrString(_pymsg, "max_lin_dec");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->max_lin_dec = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * ctl_mission_interfaces__srv__config_dynamic_pure_ctrl__request__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ConfigDynamicPureCtrl_Request */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("ctl_mission_interfaces.srv._config_dynamic_pure_ctrl");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ConfigDynamicPureCtrl_Request");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request * ros_message = (ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request *)raw_ros_message;
  {  // look_ahead_dis
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->look_ahead_dis);
    {
      int rc = PyObject_SetAttrString(_pymessage, "look_ahead_dis", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // max_v_forward
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->max_v_forward);
    {
      int rc = PyObject_SetAttrString(_pymessage, "max_v_forward", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // max_ang_acc
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->max_ang_acc);
    {
      int rc = PyObject_SetAttrString(_pymessage, "max_ang_acc", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // max_ang_dec
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->max_ang_dec);
    {
      int rc = PyObject_SetAttrString(_pymessage, "max_ang_dec", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // max_lin_acc
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->max_lin_acc);
    {
      int rc = PyObject_SetAttrString(_pymessage, "max_lin_acc", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // max_lin_dec
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->max_lin_dec);
    {
      int rc = PyObject_SetAttrString(_pymessage, "max_lin_dec", field);
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
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__struct.h"
// already included above
// #include "ctl_mission_interfaces/srv/detail/config_dynamic_pure_ctrl__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool ctl_mission_interfaces__srv__config_dynamic_pure_ctrl__response__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[84];
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
    assert(strncmp("ctl_mission_interfaces.srv._config_dynamic_pure_ctrl.ConfigDynamicPureCtrl_Response", full_classname_dest, 83) == 0);
  }
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response * ros_message = _ros_message;
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
PyObject * ctl_mission_interfaces__srv__config_dynamic_pure_ctrl__response__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of ConfigDynamicPureCtrl_Response */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("ctl_mission_interfaces.srv._config_dynamic_pure_ctrl");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "ConfigDynamicPureCtrl_Response");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response * ros_message = (ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response *)raw_ros_message;
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
