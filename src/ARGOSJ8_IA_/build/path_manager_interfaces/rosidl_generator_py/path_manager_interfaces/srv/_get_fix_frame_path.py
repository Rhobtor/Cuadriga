# generated from rosidl_generator_py/resource/_idl.py.em
# with input from path_manager_interfaces:srv/GetFixFramePath.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_GetFixFramePath_Request(type):
    """Metaclass of message 'GetFixFramePath_Request'."""

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
            module = import_type_support('path_manager_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'path_manager_interfaces.srv.GetFixFramePath_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__get_fix_frame_path__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__get_fix_frame_path__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__get_fix_frame_path__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__get_fix_frame_path__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__get_fix_frame_path__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class GetFixFramePath_Request(metaclass=Metaclass_GetFixFramePath_Request):
    """Message class 'GetFixFramePath_Request'."""

    __slots__ = [
        '_ask',
    ]

    _fields_and_field_types = {
        'ask': 'boolean',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.ask = kwargs.get('ask', bool())

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
        if self.ask != other.ask:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def ask(self):
        """Message field 'ask'."""
        return self._ask

    @ask.setter
    def ask(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'ask' field must be of type 'bool'"
        self._ask = value


# Import statements for member types

# already imported above
# import builtins

# already imported above
# import rosidl_parser.definition


class Metaclass_GetFixFramePath_Response(type):
    """Metaclass of message 'GetFixFramePath_Response'."""

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
            module = import_type_support('path_manager_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'path_manager_interfaces.srv.GetFixFramePath_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__get_fix_frame_path__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__get_fix_frame_path__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__get_fix_frame_path__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__get_fix_frame_path__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__get_fix_frame_path__response

            from nav_msgs.msg import Path
            if Path.__class__._TYPE_SUPPORT is None:
                Path.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class GetFixFramePath_Response(metaclass=Metaclass_GetFixFramePath_Response):
    """Message class 'GetFixFramePath_Response'."""

    __slots__ = [
        '_path',
    ]

    _fields_and_field_types = {
        'path': 'nav_msgs/Path',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['nav_msgs', 'msg'], 'Path'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from nav_msgs.msg import Path
        self.path = kwargs.get('path', Path())

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
        if self.path != other.path:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def path(self):
        """Message field 'path'."""
        return self._path

    @path.setter
    def path(self, value):
        if __debug__:
            from nav_msgs.msg import Path
            assert \
                isinstance(value, Path), \
                "The 'path' field must be a sub message of type 'Path'"
        self._path = value


class Metaclass_GetFixFramePath(type):
    """Metaclass of service 'GetFixFramePath'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('path_manager_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'path_manager_interfaces.srv.GetFixFramePath')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__get_fix_frame_path

            from path_manager_interfaces.srv import _get_fix_frame_path
            if _get_fix_frame_path.Metaclass_GetFixFramePath_Request._TYPE_SUPPORT is None:
                _get_fix_frame_path.Metaclass_GetFixFramePath_Request.__import_type_support__()
            if _get_fix_frame_path.Metaclass_GetFixFramePath_Response._TYPE_SUPPORT is None:
                _get_fix_frame_path.Metaclass_GetFixFramePath_Response.__import_type_support__()


class GetFixFramePath(metaclass=Metaclass_GetFixFramePath):
    from path_manager_interfaces.srv._get_fix_frame_path import GetFixFramePath_Request as Request
    from path_manager_interfaces.srv._get_fix_frame_path import GetFixFramePath_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
