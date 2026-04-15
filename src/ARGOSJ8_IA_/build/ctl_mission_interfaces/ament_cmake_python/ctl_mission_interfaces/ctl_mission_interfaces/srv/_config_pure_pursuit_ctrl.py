# generated from rosidl_generator_py/resource/_idl.py.em
# with input from ctl_mission_interfaces:srv/ConfigPurePursuitCtrl.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ConfigPurePursuitCtrl_Request(type):
    """Metaclass of message 'ConfigPurePursuitCtrl_Request'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('ctl_mission_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'ctl_mission_interfaces.srv.ConfigPurePursuitCtrl_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__config_pure_pursuit_ctrl__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__config_pure_pursuit_ctrl__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__config_pure_pursuit_ctrl__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__config_pure_pursuit_ctrl__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__config_pure_pursuit_ctrl__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ConfigPurePursuitCtrl_Request(metaclass=Metaclass_ConfigPurePursuitCtrl_Request):
    """Message class 'ConfigPurePursuitCtrl_Request'."""

    __slots__ = [
        '_v_forward',
        '_l_ahead_dist',
    ]

    _fields_and_field_types = {
        'v_forward': 'float',
        'l_ahead_dist': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.v_forward = kwargs.get('v_forward', float())
        self.l_ahead_dist = kwargs.get('l_ahead_dist', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.v_forward != other.v_forward:
            return False
        if self.l_ahead_dist != other.l_ahead_dist:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def v_forward(self):
        """Message field 'v_forward'."""
        return self._v_forward

    @v_forward.setter
    def v_forward(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'v_forward' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'v_forward' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._v_forward = value

    @builtins.property
    def l_ahead_dist(self):
        """Message field 'l_ahead_dist'."""
        return self._l_ahead_dist

    @l_ahead_dist.setter
    def l_ahead_dist(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'l_ahead_dist' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'l_ahead_dist' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._l_ahead_dist = value


# Import statements for member types

# already imported above
# import builtins

# already imported above
# import rosidl_parser.definition


class Metaclass_ConfigPurePursuitCtrl_Response(type):
    """Metaclass of message 'ConfigPurePursuitCtrl_Response'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('ctl_mission_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'ctl_mission_interfaces.srv.ConfigPurePursuitCtrl_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__config_pure_pursuit_ctrl__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__config_pure_pursuit_ctrl__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__config_pure_pursuit_ctrl__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__config_pure_pursuit_ctrl__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__config_pure_pursuit_ctrl__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ConfigPurePursuitCtrl_Response(metaclass=Metaclass_ConfigPurePursuitCtrl_Response):
    """Message class 'ConfigPurePursuitCtrl_Response'."""

    __slots__ = [
        '_ack',
    ]

    _fields_and_field_types = {
        'ack': 'boolean',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.ack = kwargs.get('ack', bool())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.ack != other.ack:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def ack(self):
        """Message field 'ack'."""
        return self._ack

    @ack.setter
    def ack(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'ack' field must be of type 'bool'"
        self._ack = value


class Metaclass_ConfigPurePursuitCtrl(type):
    """Metaclass of service 'ConfigPurePursuitCtrl'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('ctl_mission_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'ctl_mission_interfaces.srv.ConfigPurePursuitCtrl')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__config_pure_pursuit_ctrl

            from ctl_mission_interfaces.srv import _config_pure_pursuit_ctrl
            if _config_pure_pursuit_ctrl.Metaclass_ConfigPurePursuitCtrl_Request._TYPE_SUPPORT is None:
                _config_pure_pursuit_ctrl.Metaclass_ConfigPurePursuitCtrl_Request.__import_type_support__()
            if _config_pure_pursuit_ctrl.Metaclass_ConfigPurePursuitCtrl_Response._TYPE_SUPPORT is None:
                _config_pure_pursuit_ctrl.Metaclass_ConfigPurePursuitCtrl_Response.__import_type_support__()


class ConfigPurePursuitCtrl(metaclass=Metaclass_ConfigPurePursuitCtrl):
    from ctl_mission_interfaces.srv._config_pure_pursuit_ctrl import ConfigPurePursuitCtrl_Request as Request
    from ctl_mission_interfaces.srv._config_pure_pursuit_ctrl import ConfigPurePursuitCtrl_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
