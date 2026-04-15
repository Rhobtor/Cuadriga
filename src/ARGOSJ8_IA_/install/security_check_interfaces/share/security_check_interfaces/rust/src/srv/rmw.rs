#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



#[link(name = "security_check_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__PasswordCheck_Request() -> *const std::ffi::c_void;
}

#[link(name = "security_check_interfaces__rosidl_generator_c")]
extern "C" {
    fn security_check_interfaces__srv__PasswordCheck_Request__init(msg: *mut PasswordCheck_Request) -> bool;
    fn security_check_interfaces__srv__PasswordCheck_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<PasswordCheck_Request>, size: usize) -> bool;
    fn security_check_interfaces__srv__PasswordCheck_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<PasswordCheck_Request>);
    fn security_check_interfaces__srv__PasswordCheck_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<PasswordCheck_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<PasswordCheck_Request>) -> bool;
}

// Corresponds to security_check_interfaces__srv__PasswordCheck_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct PasswordCheck_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub password: rosidl_runtime_rs::String,

}



impl Default for PasswordCheck_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !security_check_interfaces__srv__PasswordCheck_Request__init(&mut msg as *mut _) {
        panic!("Call to security_check_interfaces__srv__PasswordCheck_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for PasswordCheck_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__PasswordCheck_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__PasswordCheck_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__PasswordCheck_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for PasswordCheck_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for PasswordCheck_Request where Self: Sized {
  const TYPE_NAME: &'static str = "security_check_interfaces/srv/PasswordCheck_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__PasswordCheck_Request() }
  }
}


#[link(name = "security_check_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__PasswordCheck_Response() -> *const std::ffi::c_void;
}

#[link(name = "security_check_interfaces__rosidl_generator_c")]
extern "C" {
    fn security_check_interfaces__srv__PasswordCheck_Response__init(msg: *mut PasswordCheck_Response) -> bool;
    fn security_check_interfaces__srv__PasswordCheck_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<PasswordCheck_Response>, size: usize) -> bool;
    fn security_check_interfaces__srv__PasswordCheck_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<PasswordCheck_Response>);
    fn security_check_interfaces__srv__PasswordCheck_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<PasswordCheck_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<PasswordCheck_Response>) -> bool;
}

// Corresponds to security_check_interfaces__srv__PasswordCheck_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct PasswordCheck_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub message: rosidl_runtime_rs::String,

}



impl Default for PasswordCheck_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !security_check_interfaces__srv__PasswordCheck_Response__init(&mut msg as *mut _) {
        panic!("Call to security_check_interfaces__srv__PasswordCheck_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for PasswordCheck_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__PasswordCheck_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__PasswordCheck_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__PasswordCheck_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for PasswordCheck_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for PasswordCheck_Response where Self: Sized {
  const TYPE_NAME: &'static str = "security_check_interfaces/srv/PasswordCheck_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__PasswordCheck_Response() }
  }
}


#[link(name = "security_check_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__GetSecurityCheckStatus_Request() -> *const std::ffi::c_void;
}

#[link(name = "security_check_interfaces__rosidl_generator_c")]
extern "C" {
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Request__init(msg: *mut GetSecurityCheckStatus_Request) -> bool;
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Request>, size: usize) -> bool;
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Request>);
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Request>) -> bool;
}

// Corresponds to security_check_interfaces__srv__GetSecurityCheckStatus_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetSecurityCheckStatus_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetSecurityCheckStatus_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !security_check_interfaces__srv__GetSecurityCheckStatus_Request__init(&mut msg as *mut _) {
        panic!("Call to security_check_interfaces__srv__GetSecurityCheckStatus_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetSecurityCheckStatus_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__GetSecurityCheckStatus_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetSecurityCheckStatus_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetSecurityCheckStatus_Request where Self: Sized {
  const TYPE_NAME: &'static str = "security_check_interfaces/srv/GetSecurityCheckStatus_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__GetSecurityCheckStatus_Request() }
  }
}


#[link(name = "security_check_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__GetSecurityCheckStatus_Response() -> *const std::ffi::c_void;
}

#[link(name = "security_check_interfaces__rosidl_generator_c")]
extern "C" {
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Response__init(msg: *mut GetSecurityCheckStatus_Response) -> bool;
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Response>, size: usize) -> bool;
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Response>);
    fn security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<GetSecurityCheckStatus_Response>) -> bool;
}

// Corresponds to security_check_interfaces__srv__GetSecurityCheckStatus_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetSecurityCheckStatus_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub status: bool,

}



impl Default for GetSecurityCheckStatus_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !security_check_interfaces__srv__GetSecurityCheckStatus_Response__init(&mut msg as *mut _) {
        panic!("Call to security_check_interfaces__srv__GetSecurityCheckStatus_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetSecurityCheckStatus_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { security_check_interfaces__srv__GetSecurityCheckStatus_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetSecurityCheckStatus_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetSecurityCheckStatus_Response where Self: Sized {
  const TYPE_NAME: &'static str = "security_check_interfaces/srv/GetSecurityCheckStatus_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__security_check_interfaces__srv__GetSecurityCheckStatus_Response() }
  }
}






#[link(name = "security_check_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__security_check_interfaces__srv__PasswordCheck() -> *const std::ffi::c_void;
}

// Corresponds to security_check_interfaces__srv__PasswordCheck
#[allow(missing_docs, non_camel_case_types)]
pub struct PasswordCheck;

impl rosidl_runtime_rs::Service for PasswordCheck {
    type Request = PasswordCheck_Request;
    type Response = PasswordCheck_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__security_check_interfaces__srv__PasswordCheck() }
    }
}




#[link(name = "security_check_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__security_check_interfaces__srv__GetSecurityCheckStatus() -> *const std::ffi::c_void;
}

// Corresponds to security_check_interfaces__srv__GetSecurityCheckStatus
#[allow(missing_docs, non_camel_case_types)]
pub struct GetSecurityCheckStatus;

impl rosidl_runtime_rs::Service for GetSecurityCheckStatus {
    type Request = GetSecurityCheckStatus_Request;
    type Response = GetSecurityCheckStatus_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__security_check_interfaces__srv__GetSecurityCheckStatus() }
    }
}


