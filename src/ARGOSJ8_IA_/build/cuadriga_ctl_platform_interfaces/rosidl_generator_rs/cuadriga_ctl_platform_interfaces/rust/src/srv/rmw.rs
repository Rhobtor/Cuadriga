#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__init(msg: *mut SetVelocity_Request) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<SetVelocity_Request>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<SetVelocity_Request>);
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<SetVelocity_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<SetVelocity_Request>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetVelocity_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub new_velocity: i8,

}



impl Default for SetVelocity_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for SetVelocity_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for SetVelocity_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for SetVelocity_Request where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/srv/SetVelocity_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__SetVelocity_Request() }
  }
}


#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__init(msg: *mut SetVelocity_Response) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<SetVelocity_Response>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<SetVelocity_Response>);
    fn cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<SetVelocity_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<SetVelocity_Response>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetVelocity_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub actual_velocity: i8,

}



impl Default for SetVelocity_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for SetVelocity_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for SetVelocity_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for SetVelocity_Response where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/srv/SetVelocity_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__SetVelocity_Response() }
  }
}


#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__init(msg: *mut GetVelocity_Request) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetVelocity_Request>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetVelocity_Request>);
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetVelocity_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<GetVelocity_Request>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetVelocity_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetVelocity_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetVelocity_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetVelocity_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetVelocity_Request where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/srv/GetVelocity_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Request() }
  }
}


#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__init(msg: *mut GetVelocity_Response) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetVelocity_Response>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetVelocity_Response>);
    fn cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetVelocity_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<GetVelocity_Response>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetVelocity_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub actual_velocity: i8,

}



impl Default for GetVelocity_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetVelocity_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetVelocity_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetVelocity_Response where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/srv/GetVelocity_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__GetVelocity_Response() }
  }
}


#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__init(msg: *mut EmergencyStop_Request) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<EmergencyStop_Request>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<EmergencyStop_Request>);
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<EmergencyStop_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<EmergencyStop_Request>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct EmergencyStop_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub stop: bool,

}



impl Default for EmergencyStop_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for EmergencyStop_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for EmergencyStop_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for EmergencyStop_Request where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/srv/EmergencyStop_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Request() }
  }
}


#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__init(msg: *mut EmergencyStop_Response) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<EmergencyStop_Response>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<EmergencyStop_Response>);
    fn cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<EmergencyStop_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<EmergencyStop_Response>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct EmergencyStop_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for EmergencyStop_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for EmergencyStop_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for EmergencyStop_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for EmergencyStop_Response where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/srv/EmergencyStop_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__srv__EmergencyStop_Response() }
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


