# generated from rosidl_generator_py/resource/_idl.py.em
# with input from ctl_mission_interfaces:srv/ConfigDynamicLAPureCtrl.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ConfigDynamicLAPureCtrl_Request(type):
    """Metaclass of message 'ConfigDynamicLAPureCtrl_Request'."""

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
                'ctl_mission_interfaces.srv.ConfigDynamicLAPureCtrl_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__config_dynamic_la_pure_ctrl__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__config_dynamic_la_pure_ctrl__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__config_dynamic_la_pure_ctrl__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__config_dynamic_la_pure_ctrl__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__config_dynamic_la_pure_ctrl__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ConfigDynamicLAPureCtrl_Request(metaclass=Metaclass_ConfigDynamicLAPureCtrl_Request):
    """Message class 'ConfigDynamicLAPureCtrl_Request'."""

    __slots__ = [
        '_look_ahead_v_gain',
        '_max_v_forward',
        '_max_ang_acc',
        '_max_ang_dec',
        '_max_lin_acc',
        '_max_lin_dec',
        '_speed_pow',
        '_min_look_ahead_d',
    ]

    _fields_and_field_types = {
        'look_ahead_v_gain': 'float',
        'max_v_forward': 'float',
        'max_ang_acc': 'float',
        'max_ang_dec': 'float',
        'max_lin_acc': 'float',
        'max_lin_dec': 'float',
        'speed_pow': 'float',
        'min_look_ahead_d': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.look_ahead_v_gain = kwargs.get('look_ahead_v_gain', float())
        self.max_v_forward = kwargs.get('max_v_forward', float())
        self.max_ang_acc = kwargs.get('max_ang_acc', float())
        self.max_ang_dec = kwargs.get('max_ang_dec', float())
        self.max_lin_acc = kwargs.get('max_lin_acc', float())
        self.max_lin_dec = kwargs.get('max_lin_dec', float())
        self.speed_pow = kwargs.get('speed_pow', float())
        self.min_look_ahead_d = kwargs.get('min_look_ahead_d', float())

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
        if self.look_ahead_v_gain != other.look_ahead_v_gain:
            return False
        if self.max_v_forward != other.max_v_forward:
            return False
        if self.max_ang_acc != other.max_ang_acc:
            return False
        if self.max_ang_dec != other.max_ang_dec:
            return False
        if self.max_lin_acc != other.max_lin_acc:
            return False
        if self.max_lin_dec != other.max_lin_dec:
            return False
        if self.speed_pow != other.speed_pow:
            return False
        if self.min_look_ahead_d != other.min_look_ahead_d:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def look_ahead_v_gain(self):
        """Message field 'look_ahead_v_gain'."""
        return self._look_ahead_v_gain

    @look_ahead_v_gain.setter
    def look_ahead_v_gain(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'look_ahead_v_gain' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'look_ahead_v_gain' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._look_ahead_v_gain = value

    @builtins.property
    def max_v_forward(self):
        """Message field 'max_v_forward'."""
        return self._max_v_forward

    @max_v_forward.setter
    def max_v_forward(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'max_v_forward' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'max_v_forward' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._max_v_forward = value

    @builtins.property
    def max_ang_acc(self):
        """Message field 'max_ang_acc'."""
        return self._max_ang_acc

    @max_ang_acc.setter
    def max_ang_acc(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'max_ang_acc' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'max_ang_acc' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._max_ang_acc = value

    @builtins.property
    def max_ang_dec(self):
        """Message field 'max_ang_dec'."""
        return self._max_ang_dec

    @max_ang_dec.setter
    def max_ang_dec(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'max_ang_dec' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'max_ang_dec' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._max_ang_dec = value

    @builtins.property
    def max_lin_acc(self):
        """Message field 'max_lin_acc'."""
        return self._max_lin_acc

    @max_lin_acc.setter
    def max_lin_acc(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'max_lin_acc' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'max_lin_acc' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._max_lin_acc = value

    @builtins.property
    def max_lin_dec(self):
        """Message field 'max_lin_dec'."""
        return self._max_lin_dec

    @max_lin_dec.setter
    def max_lin_dec(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'max_lin_dec' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'max_lin_dec' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._max_lin_dec = value

    @builtins.property
    def speed_pow(self):
        """Message field 'speed_pow'."""
        return self._speed_pow

    @speed_pow.setter
    def speed_pow(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'speed_pow' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'speed_pow' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._speed_pow = value

    @builtins.property
    def min_look_ahead_d(self):
        """Message field 'min_look_ahead_d'."""
        return self._min_look_ahead_d

    @min_look_ahead_d.setter
    def min_look_ahead_d(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'min_look_ahead_d' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'min_look_ahead_d' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._min_look_ahead_d = value


# Import statements for member types

# already imported above
# import builtins

# already imported above
# import rosidl_parser.definition


class Metaclass_ConfigDynamicLAPureCtrl_Response(type):
    """Metaclass of message 'ConfigDynamicLAPureCtrl_Response'."""

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
                'ctl_mission_interfaces.srv.ConfigDynamicLAPureCtrl_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__config_dynamic_la_pure_ctrl__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__config_dynamic_la_pure_ctrl__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__config_dynamic_la_pure_ctrl__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__config_dynamic_la_pure_ctrl__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__config_dynamic_la_pure_ctrl__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ConfigDynamicLAPureCtrl_Response(metaclass=Metaclass_ConfigDynamicLAPureCtrl_Response):
    """Message class 'ConfigDynamicLAPureCtrl_Response'."""

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


class Metaclass_ConfigDynamicLAPureCtrl(type):
    """Metaclass of service 'ConfigDynamicLAPureCtrl'."""

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
                'ctl_mission_interfaces.srv.ConfigDynamicLAPureCtrl')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__config_dynamic_la_pure_ctrl

            from ctl_mission_interfaces.srv import _config_dynamic_la_pure_ctrl
            if _config_dynamic_la_pure_ctrl.Metaclass_ConfigDynamicLAPureCtrl_Request._TYPE_SUPPORT is None:
                _config_dynamic_la_pure_ctrl.Metaclass_ConfigDynamicLAPureCtrl_Request.__import_type_support__()
            if _config_dynamic_la_pure_ctrl.Metaclass_ConfigDynamicLAPureCtrl_Response._TYPE_SUPPORT is None:
                _config_dynamic_la_pure_ctrl.Metaclass_ConfigDynamicLAPureCtrl_Response.__import_type_support__()


class ConfigDynamicLAPureCtrl(metaclass=Metaclass_ConfigDynamicLAPureCtrl):
    from ctl_mission_interfaces.srv._config_dynamic_la_pure_ctrl import ConfigDynamicLAPureCtrl_Request as Request
    from ctl_mission_interfaces.srv._config_dynamic_la_pure_ctrl import ConfigDynamicLAPureCtrl_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
