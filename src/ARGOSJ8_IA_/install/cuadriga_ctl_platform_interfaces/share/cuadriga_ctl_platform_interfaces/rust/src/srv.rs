#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};




// Corresponds to cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetVelocity_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub new_velocity: i8,

}



impl Default for SetVelocity_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::SetVelocity_Request::default())
  }
}

impl rosidl_runtime_rs::Message for SetVelocity_Request {
  type RmwMsg = super::srv::rmw::SetVelocity_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        new_velocity: msg.new_velocity,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      new_velocity: msg.new_velocity,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      new_velocity: msg.new_velocity,
    }
  }
}


// Corresponds to cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetVelocity_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub actual_velocity: i8,

}



impl Default for SetVelocity_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::SetVelocity_Response::default())
  }
}

impl rosidl_runtime_rs::Message for SetVelocity_Response {
  type RmwMsg = super::srv::rmw::SetVelocity_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        actual_velocity: msg.actual_velocity,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      actual_velocity: msg.actual_velocity,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      actual_velocity: msg.actual_velocity,
    }
  }
}


// Corresponds to cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetVelocity_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetVelocity_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::GetVelocity_Request::default())
  }
}

impl rosidl_runtime_rs::Message for GetVelocity_Request {
  type RmwMsg = super::srv::rmw::GetVelocity_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        structure_needs_at_least_one_member: msg.structure_needs_at_least_one_member,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      structure_needs_at_least_one_member: msg.structure_needs_at_least_one_member,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      structure_needs_at_least_one_member: msg.structure_needs_at_least_one_member,
    }
  }
}


// Corresponds to cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetVelocity_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub actual_velocity: i8,

}



impl Default for GetVelocity_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::GetVelocity_Response::default())
  }
}

impl rosidl_runtime_rs::Message for GetVelocity_Response {
  type RmwMsg = super::srv::rmw::GetVelocity_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        actual_velocity: msg.actual_velocity,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      actual_velocity: msg.actual_velocity,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      actual_velocity: msg.actual_velocity,
    }
  }
}


// Corresponds to cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct EmergencyStop_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub stop: bool,

}



impl Default for EmergencyStop_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::EmergencyStop_Request::default())
  }
}

impl rosidl_runtime_rs::Message for EmergencyStop_Request {
  type RmwMsg = super::srv::rmw::EmergencyStop_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        stop: msg.stop,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      stop: msg.stop,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      stop: msg.stop,
    }
  }
}


// Corresponds to cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct EmergencyStop_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for EmergencyStop_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::EmergencyStop_Response::default())
  }
}

impl rosidl_runtime_rs::Message for EmergencyStop_Response {
  type RmwMsg = super::srv::rmw::EmergencyStop_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        structure_needs_at_least_one_member: msg.structure_needs_at_least_one_member,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      structure_needs_at_least_one_member: msg.structure_needs_at_least_one_member,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      structure_needs_at_least_one_member: msg.structure_needs_at_least_one_member,
    }
  }
}






#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__cuadriga_ctl_platform_interfaces__srv__SetVelocity() -> *const std::ffi::c_void;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__SetVelocity
#[allow(missing_docs, non_camel_case_types)]
pub struct SetVelocity;

impl rosidl_runtime_rs::Service for SetVelocity {
    type Request = SetVelocity_Request;
    type Response = SetVelocity_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__cuadriga_ctl_platform_interfaces__srv__SetVelocity() }
    }
}




#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__cuadriga_ctl_platform_interfaces__srv__GetVelocity() -> *const std::ffi::c_void;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__GetVelocity
#[allow(missing_docs, non_camel_case_types)]
pub struct GetVelocity;

impl rosidl_runtime_rs::Service for GetVelocity {
    type Request = GetVelocity_Request;
    type Response = GetVelocity_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__cuadriga_ctl_platform_interfaces__srv__GetVelocity() }
    }
}




#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__cuadriga_ctl_platform_interfaces__srv__EmergencyStop() -> *const std::ffi::c_void;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__EmergencyStop
#[allow(missing_docs, non_camel_case_types)]
pub struct EmergencyStop;

impl rosidl_runtime_rs::Service for EmergencyStop {
    type Request = EmergencyStop_Request;
    type Response = EmergencyStop_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__cuadriga_ctl_platform_interfaces__srv__EmergencyStop() }
    }
}


