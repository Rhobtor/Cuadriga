# generated from rosidl_generator_py/resource/_idl.py.em
# with input from ctl_mission_interfaces:srv/GetPossibleTransitions.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_GetPossibleTransitions_Request(type):
    """Metaclass of message 'GetPossibleTransitions_Request'."""

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
                'ctl_mission_interfaces.srv.GetPossibleTransitions_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__get_possible_transitions__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__get_possible_transitions__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__get_possible_transitions__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__get_possible_transitions__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__get_possible_transitions__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class GetPossibleTransitions_Request(metaclass=Metaclass_GetPossibleTransitions_Request):
    """Message class 'GetPossibleTransitions_Request'."""

    __slots__ = [
    ]

    _fields_and_field_types = {
    }

    SLOT_TYPES = (
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))

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
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)


# Import statements for member types

# Member 'possible_transitions'
import array  # noqa: E402, I100

import builtins  # noqa: E402, I100

# already imported above
# import rosidl_parser.definition


class Metaclass_GetPossibleTransitions_Response(type):
    """Metaclass of message 'GetPossibleTransitions_Response'."""

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
                'ctl_mission_interfaces.srv.GetPossibleTransitions_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__get_possible_transitions__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__get_possible_transitions__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__get_possible_transitions__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__get_possible_transitions__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__get_possible_transitions__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class GetPossibleTransitions_Response(metaclass=Metaclass_GetPossibleTransitions_Response):
    """Message class 'GetPossibleTransitions_Response'."""

    __slots__ = [
        '_possible_transitions',
    ]

    _fields_and_field_types = {
        'possible_transitions': 'sequence<int32>',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('int32')),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.possible_transitions = array.array('i', kwargs.get('possible_transitions', []))

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
        if self.possible_transitions != other.possible_transitions:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def possible_transitions(self):
        """Message field 'possible_transitions'."""
        return self._possible_transitions

    @possible_transitions.setter
    def possible_transitions(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'i', \
                "The 'possible_transitions' array.array() must have the type code of 'i'"
            self._possible_transitions = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, int) for v in value) and
                 all(val >= -2147483648 and val < 2147483648 for val in value)), \
                "The 'possible_transitions' field must be a set or sequence and each value of type 'int' and each integer in [-2147483648, 2147483647]"
        self._possible_transitions = array.array('i', value)


class Metaclass_GetPossibleTransitions(type):
    """Metaclass of service 'GetPossibleTransitions'."""

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
                'ctl_mission_interfaces.srv.GetPossibleTransitions')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__get_possible_transitions

            from ctl_mission_interfaces.srv import _get_possible_transitions
            if _get_possible_transitions.Metaclass_GetPossibleTransitions_Request._TYPE_SUPPORT is None:
                _get_possible_transitions.Metaclass_GetPossibleTransitions_Request.__import_type_support__()
            if _get_possible_transitions.Metaclass_GetPossibleTransitions_Response._TYPE_SUPPORT is None:
                _get_possible_transitions.Metaclass_GetPossibleTransitions_Response.__import_type_support__()


class GetPossibleTransitions(metaclass=Metaclass_GetPossibleTransitions):
    from ctl_mission_interfaces.srv._get_possible_transitions import GetPossibleTransitions_Request as Request
    from ctl_mission_interfaces.srv._get_possible_transitions import GetPossibleTransitions_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
