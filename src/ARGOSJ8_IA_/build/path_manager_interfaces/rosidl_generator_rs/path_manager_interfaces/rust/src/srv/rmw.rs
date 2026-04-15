#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__WritePathToFile_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__WritePathToFile_Request__init(msg: *mut WritePathToFile_Request) -> bool;
    fn path_manager_interfaces__srv__WritePathToFile_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<WritePathToFile_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__WritePathToFile_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<WritePathToFile_Request>);
    fn path_manager_interfaces__srv__WritePathToFile_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<WritePathToFile_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<WritePathToFile_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__WritePathToFile_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct WritePathToFile_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub filename: rosidl_runtime_rs::String,

}



impl Default for WritePathToFile_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__WritePathToFile_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__WritePathToFile_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for WritePathToFile_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__WritePathToFile_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__WritePathToFile_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__WritePathToFile_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for WritePathToFile_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for WritePathToFile_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/WritePathToFile_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__WritePathToFile_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__WritePathToFile_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__WritePathToFile_Response__init(msg: *mut WritePathToFile_Response) -> bool;
    fn path_manager_interfaces__srv__WritePathToFile_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<WritePathToFile_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__WritePathToFile_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<WritePathToFile_Response>);
    fn path_manager_interfaces__srv__WritePathToFile_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<WritePathToFile_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<WritePathToFile_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__WritePathToFile_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct WritePathToFile_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,

}



impl Default for WritePathToFile_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__WritePathToFile_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__WritePathToFile_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for WritePathToFile_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__WritePathToFile_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__WritePathToFile_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__WritePathToFile_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for WritePathToFile_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for WritePathToFile_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/WritePathToFile_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__WritePathToFile_Response() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReadPathFromFile_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__ReadPathFromFile_Request__init(msg: *mut ReadPathFromFile_Request) -> bool;
    fn path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ReadPathFromFile_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ReadPathFromFile_Request>);
    fn path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ReadPathFromFile_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ReadPathFromFile_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__ReadPathFromFile_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ReadPathFromFile_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub filename: rosidl_runtime_rs::String,

}



impl Default for ReadPathFromFile_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__ReadPathFromFile_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__ReadPathFromFile_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ReadPathFromFile_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReadPathFromFile_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ReadPathFromFile_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ReadPathFromFile_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/ReadPathFromFile_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReadPathFromFile_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReadPathFromFile_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__ReadPathFromFile_Response__init(msg: *mut ReadPathFromFile_Response) -> bool;
    fn path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ReadPathFromFile_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ReadPathFromFile_Response>);
    fn path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ReadPathFromFile_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ReadPathFromFile_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__ReadPathFromFile_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ReadPathFromFile_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,

}



impl Default for ReadPathFromFile_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__ReadPathFromFile_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__ReadPathFromFile_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ReadPathFromFile_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReadPathFromFile_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ReadPathFromFile_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ReadPathFromFile_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/ReadPathFromFile_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReadPathFromFile_Response() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReturnRobotPath_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__ReturnRobotPath_Request__init(msg: *mut ReturnRobotPath_Request) -> bool;
    fn path_manager_interfaces__srv__ReturnRobotPath_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ReturnRobotPath_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__ReturnRobotPath_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ReturnRobotPath_Request>);
    fn path_manager_interfaces__srv__ReturnRobotPath_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ReturnRobotPath_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ReturnRobotPath_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__ReturnRobotPath_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ReturnRobotPath_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for ReturnRobotPath_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__ReturnRobotPath_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__ReturnRobotPath_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ReturnRobotPath_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReturnRobotPath_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReturnRobotPath_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReturnRobotPath_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ReturnRobotPath_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ReturnRobotPath_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/ReturnRobotPath_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReturnRobotPath_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReturnRobotPath_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__ReturnRobotPath_Response__init(msg: *mut ReturnRobotPath_Response) -> bool;
    fn path_manager_interfaces__srv__ReturnRobotPath_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ReturnRobotPath_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__ReturnRobotPath_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ReturnRobotPath_Response>);
    fn path_manager_interfaces__srv__ReturnRobotPath_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ReturnRobotPath_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ReturnRobotPath_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__ReturnRobotPath_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ReturnRobotPath_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub path: nav_msgs::msg::rmw::Path,

}



impl Default for ReturnRobotPath_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__ReturnRobotPath_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__ReturnRobotPath_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ReturnRobotPath_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReturnRobotPath_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReturnRobotPath_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__ReturnRobotPath_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ReturnRobotPath_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ReturnRobotPath_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/ReturnRobotPath_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__ReturnRobotPath_Response() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__RobotPath_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__RobotPath_Request__init(msg: *mut RobotPath_Request) -> bool;
    fn path_manager_interfaces__srv__RobotPath_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<RobotPath_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__RobotPath_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<RobotPath_Request>);
    fn path_manager_interfaces__srv__RobotPath_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<RobotPath_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<RobotPath_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__RobotPath_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct RobotPath_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub path: nav_msgs::msg::rmw::Path,

}



impl Default for RobotPath_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__RobotPath_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__RobotPath_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for RobotPath_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__RobotPath_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__RobotPath_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__RobotPath_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for RobotPath_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for RobotPath_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/RobotPath_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__RobotPath_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__RobotPath_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__RobotPath_Response__init(msg: *mut RobotPath_Response) -> bool;
    fn path_manager_interfaces__srv__RobotPath_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<RobotPath_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__RobotPath_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<RobotPath_Response>);
    fn path_manager_interfaces__srv__RobotPath_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<RobotPath_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<RobotPath_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__RobotPath_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct RobotPath_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for RobotPath_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__RobotPath_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__RobotPath_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for RobotPath_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__RobotPath_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__RobotPath_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__RobotPath_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for RobotPath_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for RobotPath_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/RobotPath_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__RobotPath_Response() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetFixFramePath_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__GetFixFramePath_Request__init(msg: *mut GetFixFramePath_Request) -> bool;
    fn path_manager_interfaces__srv__GetFixFramePath_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetFixFramePath_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__GetFixFramePath_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetFixFramePath_Request>);
    fn path_manager_interfaces__srv__GetFixFramePath_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetFixFramePath_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<GetFixFramePath_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__GetFixFramePath_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetFixFramePath_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ask: bool,

}



impl Default for GetFixFramePath_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__GetFixFramePath_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__GetFixFramePath_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetFixFramePath_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetFixFramePath_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetFixFramePath_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetFixFramePath_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetFixFramePath_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetFixFramePath_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/GetFixFramePath_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetFixFramePath_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetFixFramePath_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__GetFixFramePath_Response__init(msg: *mut GetFixFramePath_Response) -> bool;
    fn path_manager_interfaces__srv__GetFixFramePath_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetFixFramePath_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__GetFixFramePath_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetFixFramePath_Response>);
    fn path_manager_interfaces__srv__GetFixFramePath_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetFixFramePath_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<GetFixFramePath_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__GetFixFramePath_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetFixFramePath_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub path: nav_msgs::msg::rmw::Path,

}



impl Default for GetFixFramePath_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__GetFixFramePath_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__GetFixFramePath_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetFixFramePath_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetFixFramePath_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetFixFramePath_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetFixFramePath_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetFixFramePath_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetFixFramePath_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/GetFixFramePath_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetFixFramePath_Response() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetLLPath_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__GetLLPath_Request__init(msg: *mut GetLLPath_Request) -> bool;
    fn path_manager_interfaces__srv__GetLLPath_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetLLPath_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__GetLLPath_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetLLPath_Request>);
    fn path_manager_interfaces__srv__GetLLPath_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetLLPath_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<GetLLPath_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__GetLLPath_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetLLPath_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetLLPath_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__GetLLPath_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__GetLLPath_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetLLPath_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetLLPath_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetLLPath_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetLLPath_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetLLPath_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetLLPath_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/GetLLPath_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetLLPath_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetLLPath_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__GetLLPath_Response__init(msg: *mut GetLLPath_Response) -> bool;
    fn path_manager_interfaces__srv__GetLLPath_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetLLPath_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__GetLLPath_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetLLPath_Response>);
    fn path_manager_interfaces__srv__GetLLPath_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetLLPath_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<GetLLPath_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__GetLLPath_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetLLPath_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub path: nav_msgs::msg::rmw::Path,


    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,

}



impl Default for GetLLPath_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__GetLLPath_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__GetLLPath_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetLLPath_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetLLPath_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetLLPath_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__GetLLPath_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetLLPath_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetLLPath_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/GetLLPath_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__GetLLPath_Response() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__PlanPath_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__PlanPath_Request__init(msg: *mut PlanPath_Request) -> bool;
    fn path_manager_interfaces__srv__PlanPath_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<PlanPath_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__PlanPath_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<PlanPath_Request>);
    fn path_manager_interfaces__srv__PlanPath_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<PlanPath_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<PlanPath_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__PlanPath_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct PlanPath_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub start: geometry_msgs::msg::rmw::Point,


    // This member is not documented.
    #[allow(missing_docs)]
    pub goal: geometry_msgs::msg::rmw::Point,

}



impl Default for PlanPath_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__PlanPath_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__PlanPath_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for PlanPath_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__PlanPath_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__PlanPath_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__PlanPath_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for PlanPath_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for PlanPath_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/PlanPath_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__PlanPath_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__PlanPath_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__PlanPath_Response__init(msg: *mut PlanPath_Response) -> bool;
    fn path_manager_interfaces__srv__PlanPath_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<PlanPath_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__PlanPath_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<PlanPath_Response>);
    fn path_manager_interfaces__srv__PlanPath_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<PlanPath_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<PlanPath_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__PlanPath_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct PlanPath_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub path: nav_msgs::msg::rmw::Path,

}



impl Default for PlanPath_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__PlanPath_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__PlanPath_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for PlanPath_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__PlanPath_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__PlanPath_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__PlanPath_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for PlanPath_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for PlanPath_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/PlanPath_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__PlanPath_Response() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__AssistEmergency_Request() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__AssistEmergency_Request__init(msg: *mut AssistEmergency_Request) -> bool;
    fn path_manager_interfaces__srv__AssistEmergency_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<AssistEmergency_Request>, size: usize) -> bool;
    fn path_manager_interfaces__srv__AssistEmergency_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<AssistEmergency_Request>);
    fn path_manager_interfaces__srv__AssistEmergency_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<AssistEmergency_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<AssistEmergency_Request>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__AssistEmergency_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct AssistEmergency_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub start: geometry_msgs::msg::rmw::Point,


    // This member is not documented.
    #[allow(missing_docs)]
    pub goal: geometry_msgs::msg::rmw::Point,

}



impl Default for AssistEmergency_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__AssistEmergency_Request__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__AssistEmergency_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for AssistEmergency_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__AssistEmergency_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__AssistEmergency_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__AssistEmergency_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for AssistEmergency_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for AssistEmergency_Request where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/AssistEmergency_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__AssistEmergency_Request() }
  }
}


#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__AssistEmergency_Response() -> *const std::ffi::c_void;
}

#[link(name = "path_manager_interfaces__rosidl_generator_c")]
extern "C" {
    fn path_manager_interfaces__srv__AssistEmergency_Response__init(msg: *mut AssistEmergency_Response) -> bool;
    fn path_manager_interfaces__srv__AssistEmergency_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<AssistEmergency_Response>, size: usize) -> bool;
    fn path_manager_interfaces__srv__AssistEmergency_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<AssistEmergency_Response>);
    fn path_manager_interfaces__srv__AssistEmergency_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<AssistEmergency_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<AssistEmergency_Response>) -> bool;
}

// Corresponds to path_manager_interfaces__srv__AssistEmergency_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct AssistEmergency_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub path: nav_msgs::msg::rmw::Path,

}



impl Default for AssistEmergency_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !path_manager_interfaces__srv__AssistEmergency_Response__init(&mut msg as *mut _) {
        panic!("Call to path_manager_interfaces__srv__AssistEmergency_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for AssistEmergency_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__AssistEmergency_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__AssistEmergency_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { path_manager_interfaces__srv__AssistEmergency_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for AssistEmergency_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for AssistEmergency_Response where Self: Sized {
  const TYPE_NAME: &'static str = "path_manager_interfaces/srv/AssistEmergency_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__path_manager_interfaces__srv__AssistEmergency_Response() }
  }
}






#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__WritePathToFile() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__WritePathToFile
#[allow(missing_docs, non_camel_case_types)]
pub struct WritePathToFile;

impl rosidl_runtime_rs::Service for WritePathToFile {
    type Request = WritePathToFile_Request;
    type Response = WritePathToFile_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__WritePathToFile() }
    }
}




#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__ReadPathFromFile() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__ReadPathFromFile
#[allow(missing_docs, non_camel_case_types)]
pub struct ReadPathFromFile;

impl rosidl_runtime_rs::Service for ReadPathFromFile {
    type Request = ReadPathFromFile_Request;
    type Response = ReadPathFromFile_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__ReadPathFromFile() }
    }
}




#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__ReturnRobotPath() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__ReturnRobotPath
#[allow(missing_docs, non_camel_case_types)]
pub struct ReturnRobotPath;

impl rosidl_runtime_rs::Service for ReturnRobotPath {
    type Request = ReturnRobotPath_Request;
    type Response = ReturnRobotPath_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__ReturnRobotPath() }
    }
}




#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__RobotPath() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__RobotPath
#[allow(missing_docs, non_camel_case_types)]
pub struct RobotPath;

impl rosidl_runtime_rs::Service for RobotPath {
    type Request = RobotPath_Request;
    type Response = RobotPath_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__RobotPath() }
    }
}




#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__GetFixFramePath() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__GetFixFramePath
#[allow(missing_docs, non_camel_case_types)]
pub struct GetFixFramePath;

impl rosidl_runtime_rs::Service for GetFixFramePath {
    type Request = GetFixFramePath_Request;
    type Response = GetFixFramePath_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__GetFixFramePath() }
    }
}




#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__GetLLPath() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__GetLLPath
#[allow(missing_docs, non_camel_case_types)]
pub struct GetLLPath;

impl rosidl_runtime_rs::Service for GetLLPath {
    type Request = GetLLPath_Request;
    type Response = GetLLPath_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__GetLLPath() }
    }
}




#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__PlanPath() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__PlanPath
#[allow(missing_docs, non_camel_case_types)]
pub struct PlanPath;

impl rosidl_runtime_rs::Service for PlanPath {
    type Request = PlanPath_Request;
    type Response = PlanPath_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__PlanPath() }
    }
}




#[link(name = "path_manager_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__AssistEmergency() -> *const std::ffi::c_void;
}

// Corresponds to path_manager_interfaces__srv__AssistEmergency
#[allow(missing_docs, non_camel_case_types)]
pub struct AssistEmergency;

impl rosidl_runtime_rs::Service for AssistEmergency {
    type Request = AssistEmergency_Request;
    type Response = AssistEmergency_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__path_manager_interfaces__srv__AssistEmergency() }
    }
}


