#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__init(msg: *mut CmdThrottleMsg) -> bool;
    fn cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<CmdThrottleMsg>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<CmdThrottleMsg>);
    fn cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<CmdThrottleMsg>, out_seq: *mut rosidl_runtime_rs::Sequence<CmdThrottleMsg>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// CmdThrottleMsg.msg
///
/// Mensaje de mando a nivel bajo para la plataforma (modo Gazebo / LCM).
///
/// Semántica:
/// - throttle: mando de acelerador/velocidad longitudinal (habitualmente en "porcentaje"
///   o unidades normalizadas tras aplicar el modelo cinemático del vehículo).
/// - steering: mando de giro/dirección (también suele ser un porcentaje o valor
///   normalizado).
///
/// Nota: las unidades exactas dependen de los parámetros `throttle_to_percent` y
/// `steer_to_percent` y de la implementación del kinematic model.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct CmdThrottleMsg {

    // This member is not documented.
    #[allow(missing_docs)]
    pub throttle: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub steering: f32,

}



impl Default for CmdThrottleMsg {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for CmdThrottleMsg {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for CmdThrottleMsg {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for CmdThrottleMsg where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/msg/CmdThrottleMsg";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg() }
  }
}


#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg() -> *const std::ffi::c_void;
}

#[link(name = "cuadriga_ctl_platform_interfaces__rosidl_generator_c")]
extern "C" {
    fn cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__init(msg: *mut KinematicDebugMsg) -> bool;
    fn cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<KinematicDebugMsg>, size: usize) -> bool;
    fn cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<KinematicDebugMsg>);
    fn cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<KinematicDebugMsg>, out_seq: *mut rosidl_runtime_rs::Sequence<KinematicDebugMsg>) -> bool;
}

// Corresponds to cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// KinematicDebugMsg.msg
///
/// Estado de depuracion del modelo cinemático usado para generar
/// el mando low-level a partir de cmd_vel.
///
/// Todos los campos escalarizados de throttle/steering se publican en las
/// mismas unidades que CmdThrottleMsg. Las aceleraciones solicitadas se
/// expresan antes de aplicar la saturacion configurada.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct KinematicDebugMsg {

    // This member is not documented.
    #[allow(missing_docs)]
    pub throttle: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub steering: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub requested_throttle_acc: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub requested_steering_acc: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub throttle_limited: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub steering_limited: bool,

}



impl Default for KinematicDebugMsg {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__init(&mut msg as *mut _) {
        panic!("Call to cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for KinematicDebugMsg {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for KinematicDebugMsg {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for KinematicDebugMsg where Self: Sized {
  const TYPE_NAME: &'static str = "cuadriga_ctl_platform_interfaces/msg/KinematicDebugMsg";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg() }
  }
}


