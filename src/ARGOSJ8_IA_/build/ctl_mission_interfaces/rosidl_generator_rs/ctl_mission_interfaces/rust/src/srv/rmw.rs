#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeMode_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ChangeMode_Request__init(msg: *mut ChangeMode_Request) -> bool;
    fn ctl_mission_interfaces__srv__ChangeMode_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ChangeMode_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ChangeMode_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ChangeMode_Request>);
    fn ctl_mission_interfaces__srv__ChangeMode_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ChangeMode_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ChangeMode_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeMode_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeMode_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub transition: i8,

}



impl Default for ChangeMode_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ChangeMode_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ChangeMode_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ChangeMode_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeMode_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeMode_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeMode_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ChangeMode_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ChangeMode_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ChangeMode_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeMode_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeMode_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ChangeMode_Response__init(msg: *mut ChangeMode_Response) -> bool;
    fn ctl_mission_interfaces__srv__ChangeMode_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ChangeMode_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ChangeMode_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ChangeMode_Response>);
    fn ctl_mission_interfaces__srv__ChangeMode_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ChangeMode_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ChangeMode_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeMode_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeMode_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,

}



impl Default for ChangeMode_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ChangeMode_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ChangeMode_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ChangeMode_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeMode_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeMode_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeMode_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ChangeMode_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ChangeMode_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ChangeMode_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeMode_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetMode_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__GetMode_Request__init(msg: *mut GetMode_Request) -> bool;
    fn ctl_mission_interfaces__srv__GetMode_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetMode_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__GetMode_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetMode_Request>);
    fn ctl_mission_interfaces__srv__GetMode_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetMode_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<GetMode_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__GetMode_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetMode_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetMode_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__GetMode_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__GetMode_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetMode_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetMode_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetMode_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetMode_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetMode_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetMode_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/GetMode_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetMode_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetMode_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__GetMode_Response__init(msg: *mut GetMode_Response) -> bool;
    fn ctl_mission_interfaces__srv__GetMode_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetMode_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__GetMode_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetMode_Response>);
    fn ctl_mission_interfaces__srv__GetMode_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetMode_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<GetMode_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__GetMode_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetMode_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub mode: i8,

}



impl Default for GetMode_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__GetMode_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__GetMode_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetMode_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetMode_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetMode_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetMode_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetMode_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetMode_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/GetMode_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetMode_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeSpeed_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ChangeSpeed_Request__init(msg: *mut ChangeSpeed_Request) -> bool;
    fn ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ChangeSpeed_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ChangeSpeed_Request>);
    fn ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ChangeSpeed_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ChangeSpeed_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeSpeed_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeSpeed_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub speed: f32,

}



impl Default for ChangeSpeed_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ChangeSpeed_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ChangeSpeed_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ChangeSpeed_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeSpeed_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ChangeSpeed_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ChangeSpeed_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ChangeSpeed_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeSpeed_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeSpeed_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ChangeSpeed_Response__init(msg: *mut ChangeSpeed_Response) -> bool;
    fn ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ChangeSpeed_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ChangeSpeed_Response>);
    fn ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ChangeSpeed_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ChangeSpeed_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeSpeed_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeSpeed_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub succes: bool,

}



impl Default for ChangeSpeed_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ChangeSpeed_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ChangeSpeed_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ChangeSpeed_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeSpeed_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ChangeSpeed_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ChangeSpeed_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ChangeSpeed_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeSpeed_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigSimpleCtl_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init(msg: *mut ConfigSimpleCtl_Request) -> bool;
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Request>);
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigSimpleCtl_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigSimpleCtl_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub k_angle: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub points_in_window: i16,


    // This member is not documented.
    #[allow(missing_docs)]
    pub k_position: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub v_forward: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub l_ahead_dist: f32,

}



impl Default for ConfigSimpleCtl_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigSimpleCtl_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigSimpleCtl_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigSimpleCtl_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigSimpleCtl_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigSimpleCtl_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigSimpleCtl_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigSimpleCtl_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init(msg: *mut ConfigSimpleCtl_Response) -> bool;
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Response>);
    fn ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigSimpleCtl_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigSimpleCtl_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigSimpleCtl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigSimpleCtl_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigSimpleCtl_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigSimpleCtl_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigSimpleCtl_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigSimpleCtl_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigSimpleCtl_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigSimpleCtl_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetPossibleTransitions_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Request__init(msg: *mut GetPossibleTransitions_Request) -> bool;
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetPossibleTransitions_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetPossibleTransitions_Request>);
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetPossibleTransitions_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<GetPossibleTransitions_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__GetPossibleTransitions_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetPossibleTransitions_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetPossibleTransitions_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__GetPossibleTransitions_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__GetPossibleTransitions_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetPossibleTransitions_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetPossibleTransitions_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetPossibleTransitions_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetPossibleTransitions_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/GetPossibleTransitions_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetPossibleTransitions_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetPossibleTransitions_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Response__init(msg: *mut GetPossibleTransitions_Response) -> bool;
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<GetPossibleTransitions_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<GetPossibleTransitions_Response>);
    fn ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<GetPossibleTransitions_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<GetPossibleTransitions_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__GetPossibleTransitions_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetPossibleTransitions_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub possible_transitions: rosidl_runtime_rs::Sequence<i32>,

}



impl Default for GetPossibleTransitions_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__GetPossibleTransitions_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__GetPossibleTransitions_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for GetPossibleTransitions_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__GetPossibleTransitions_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for GetPossibleTransitions_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for GetPossibleTransitions_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/GetPossibleTransitions_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__GetPossibleTransitions_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeController_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ChangeController_Request__init(msg: *mut ChangeController_Request) -> bool;
    fn ctl_mission_interfaces__srv__ChangeController_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ChangeController_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ChangeController_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ChangeController_Request>);
    fn ctl_mission_interfaces__srv__ChangeController_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ChangeController_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ChangeController_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeController_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeController_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub controller_type: rosidl_runtime_rs::String,

}



impl Default for ChangeController_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ChangeController_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ChangeController_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ChangeController_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeController_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeController_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeController_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ChangeController_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ChangeController_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ChangeController_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeController_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeController_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ChangeController_Response__init(msg: *mut ChangeController_Response) -> bool;
    fn ctl_mission_interfaces__srv__ChangeController_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ChangeController_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ChangeController_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ChangeController_Response>);
    fn ctl_mission_interfaces__srv__ChangeController_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ChangeController_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ChangeController_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeController_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeController_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,

}



impl Default for ChangeController_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ChangeController_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ChangeController_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ChangeController_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeController_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeController_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ChangeController_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ChangeController_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ChangeController_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ChangeController_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ChangeController_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__init(msg: *mut ConfigPurePursuitCtrl_Request) -> bool;
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Request>);
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigPurePursuitCtrl_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub v_forward: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub l_ahead_dist: f32,

}



impl Default for ConfigPurePursuitCtrl_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigPurePursuitCtrl_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigPurePursuitCtrl_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigPurePursuitCtrl_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigPurePursuitCtrl_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__init(msg: *mut ConfigPurePursuitCtrl_Response) -> bool;
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Response>);
    fn ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigPurePursuitCtrl_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigPurePursuitCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigPurePursuitCtrl_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigPurePursuitCtrl_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigPurePursuitCtrl_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigPurePursuitCtrl_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigPurePursuitCtrl_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__init(msg: *mut ConfigStanleyCtrl_Request) -> bool;
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Request>);
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigStanleyCtrl_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub v_forward: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub l_ahead_dist: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub k_error_lineal: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub k_error_angular: f32,

}



impl Default for ConfigStanleyCtrl_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigStanleyCtrl_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigStanleyCtrl_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigStanleyCtrl_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigStanleyCtrl_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__init(msg: *mut ConfigStanleyCtrl_Response) -> bool;
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Response>);
    fn ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigStanleyCtrl_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigStanleyCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigStanleyCtrl_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigStanleyCtrl_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigStanleyCtrl_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigStanleyCtrl_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigStanleyCtrl_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__init(msg: *mut ConfigDynamicPureCtrl_Request) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Request>);
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigDynamicPureCtrl_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub look_ahead_dis: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_v_forward: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_ang_acc: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_ang_dec: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_lin_acc: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_lin_dec: f32,

}



impl Default for ConfigDynamicPureCtrl_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigDynamicPureCtrl_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicPureCtrl_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigDynamicPureCtrl_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigDynamicPureCtrl_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__init(msg: *mut ConfigDynamicPureCtrl_Response) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Response>);
    fn ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicPureCtrl_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigDynamicPureCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigDynamicPureCtrl_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigDynamicPureCtrl_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicPureCtrl_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigDynamicPureCtrl_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigDynamicPureCtrl_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__init(msg: *mut ConfigDynamicLAPureCtrl_Request) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Request>);
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigDynamicLAPureCtrl_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub look_ahead_v_gain: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_v_forward: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_ang_acc: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_ang_dec: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_lin_acc: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub max_lin_dec: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub speed_pow: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub min_look_ahead_d: f32,

}



impl Default for ConfigDynamicLAPureCtrl_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigDynamicLAPureCtrl_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicLAPureCtrl_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigDynamicLAPureCtrl_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__init(msg: *mut ConfigDynamicLAPureCtrl_Response) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Response>);
    fn ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigDynamicLAPureCtrl_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigDynamicLAPureCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigDynamicLAPureCtrl_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigDynamicLAPureCtrl_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicLAPureCtrl_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigDynamicLAPureCtrl_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigDynamicLAPureCtrl_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__init(msg: *mut ConfigRegulatedPureCtrl_Request) -> bool;
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Request>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Request>);
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Request>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigRegulatedPureCtrl_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub look_ahead_dis: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub v_forward: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub r_min: f32,

}



impl Default for ConfigRegulatedPureCtrl_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigRegulatedPureCtrl_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigRegulatedPureCtrl_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigRegulatedPureCtrl_Request where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request() }
  }
}


#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response() -> *const std::ffi::c_void;
}

#[link(name = "ctl_mission_interfaces__rosidl_generator_c")]
extern "C" {
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__init(msg: *mut ConfigRegulatedPureCtrl_Response) -> bool;
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Response>, size: usize) -> bool;
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Response>);
    fn ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<ConfigRegulatedPureCtrl_Response>) -> bool;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigRegulatedPureCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigRegulatedPureCtrl_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__init(&mut msg as *mut _) {
        panic!("Call to ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ConfigRegulatedPureCtrl_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ConfigRegulatedPureCtrl_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ConfigRegulatedPureCtrl_Response where Self: Sized {
  const TYPE_NAME: &'static str = "ctl_mission_interfaces/srv/ConfigRegulatedPureCtrl_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response() }
  }
}






#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ChangeMode() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeMode
#[allow(missing_docs, non_camel_case_types)]
pub struct ChangeMode;

impl rosidl_runtime_rs::Service for ChangeMode {
    type Request = ChangeMode_Request;
    type Response = ChangeMode_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ChangeMode() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__GetMode() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__GetMode
#[allow(missing_docs, non_camel_case_types)]
pub struct GetMode;

impl rosidl_runtime_rs::Service for GetMode {
    type Request = GetMode_Request;
    type Response = GetMode_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__GetMode() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ChangeSpeed() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeSpeed
#[allow(missing_docs, non_camel_case_types)]
pub struct ChangeSpeed;

impl rosidl_runtime_rs::Service for ChangeSpeed {
    type Request = ChangeSpeed_Request;
    type Response = ChangeSpeed_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ChangeSpeed() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigSimpleCtl() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigSimpleCtl
#[allow(missing_docs, non_camel_case_types)]
pub struct ConfigSimpleCtl;

impl rosidl_runtime_rs::Service for ConfigSimpleCtl {
    type Request = ConfigSimpleCtl_Request;
    type Response = ConfigSimpleCtl_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigSimpleCtl() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__GetPossibleTransitions() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__GetPossibleTransitions
#[allow(missing_docs, non_camel_case_types)]
pub struct GetPossibleTransitions;

impl rosidl_runtime_rs::Service for GetPossibleTransitions {
    type Request = GetPossibleTransitions_Request;
    type Response = GetPossibleTransitions_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__GetPossibleTransitions() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ChangeController() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ChangeController
#[allow(missing_docs, non_camel_case_types)]
pub struct ChangeController;

impl rosidl_runtime_rs::Service for ChangeController {
    type Request = ChangeController_Request;
    type Response = ChangeController_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ChangeController() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigPurePursuitCtrl() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigPurePursuitCtrl
#[allow(missing_docs, non_camel_case_types)]
pub struct ConfigPurePursuitCtrl;

impl rosidl_runtime_rs::Service for ConfigPurePursuitCtrl {
    type Request = ConfigPurePursuitCtrl_Request;
    type Response = ConfigPurePursuitCtrl_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigPurePursuitCtrl() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigStanleyCtrl() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigStanleyCtrl
#[allow(missing_docs, non_camel_case_types)]
pub struct ConfigStanleyCtrl;

impl rosidl_runtime_rs::Service for ConfigStanleyCtrl {
    type Request = ConfigStanleyCtrl_Request;
    type Response = ConfigStanleyCtrl_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigStanleyCtrl() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicPureCtrl
#[allow(missing_docs, non_camel_case_types)]
pub struct ConfigDynamicPureCtrl;

impl rosidl_runtime_rs::Service for ConfigDynamicPureCtrl {
    type Request = ConfigDynamicPureCtrl_Request;
    type Response = ConfigDynamicPureCtrl_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicPureCtrl() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl
#[allow(missing_docs, non_camel_case_types)]
pub struct ConfigDynamicLAPureCtrl;

impl rosidl_runtime_rs::Service for ConfigDynamicLAPureCtrl {
    type Request = ConfigDynamicLAPureCtrl_Request;
    type Response = ConfigDynamicLAPureCtrl_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl() }
    }
}




#[link(name = "ctl_mission_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl() -> *const std::ffi::c_void;
}

// Corresponds to ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl
#[allow(missing_docs, non_camel_case_types)]
pub struct ConfigRegulatedPureCtrl;

impl rosidl_runtime_rs::Service for ConfigRegulatedPureCtrl {
    type Request = ConfigRegulatedPureCtrl_Request;
    type Response = ConfigRegulatedPureCtrl_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl() }
    }
}


