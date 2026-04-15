#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "wiimote_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wiimote_msgs__msg__IrSourceInfo() -> *const std::ffi::c_void;
}

#[link(name = "wiimote_msgs__rosidl_generator_c")]
extern "C" {
    fn wiimote_msgs__msg__IrSourceInfo__init(msg: *mut IrSourceInfo) -> bool;
    fn wiimote_msgs__msg__IrSourceInfo__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<IrSourceInfo>, size: usize) -> bool;
    fn wiimote_msgs__msg__IrSourceInfo__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<IrSourceInfo>);
    fn wiimote_msgs__msg__IrSourceInfo__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<IrSourceInfo>, out_seq: *mut rosidl_runtime_rs::Sequence<IrSourceInfo>) -> bool;
}

// Corresponds to wiimote_msgs__msg__IrSourceInfo
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Sensor data pertaining to the Wiimote infrared camera.
/// This message contains data for one of the four infrared
/// light sources that the camera detects.
///
/// Each light is specified with a 2D position and
/// a 'source magnitude' (ir_size). If the x dimension
/// is set to INVALID_FLOAT, then no light was detected for
/// the respective light. The Wiimote handles up to
/// four light sources, and the wiimote_controller software
/// is written to that limit as well.
///
/// I am unsure what the 'ir_size' values represent.
/// They are described as 'source magnitude' in some places. I
/// *assume* this is signal amplitude, but it's unclear.
/// Note that current lowest level cwiid driver does not
/// seem to pass the ir_size value to the cwiid Wiimote.c.
/// For now this size will therefore be set INVALID

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct IrSourceInfo {

    // This member is not documented.
    #[allow(missing_docs)]
    pub x: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub ir_size: i64,

}



impl Default for IrSourceInfo {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wiimote_msgs__msg__IrSourceInfo__init(&mut msg as *mut _) {
        panic!("Call to wiimote_msgs__msg__IrSourceInfo__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for IrSourceInfo {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__IrSourceInfo__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__IrSourceInfo__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__IrSourceInfo__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for IrSourceInfo {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for IrSourceInfo where Self: Sized {
  const TYPE_NAME: &'static str = "wiimote_msgs/msg/IrSourceInfo";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wiimote_msgs__msg__IrSourceInfo() }
  }
}


#[link(name = "wiimote_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wiimote_msgs__msg__State() -> *const std::ffi::c_void;
}

#[link(name = "wiimote_msgs__rosidl_generator_c")]
extern "C" {
    fn wiimote_msgs__msg__State__init(msg: *mut State) -> bool;
    fn wiimote_msgs__msg__State__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<State>, size: usize) -> bool;
    fn wiimote_msgs__msg__State__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<State>);
    fn wiimote_msgs__msg__State__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<State>, out_seq: *mut rosidl_runtime_rs::Sequence<State>) -> bool;
}

// Corresponds to wiimote_msgs__msg__State
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// Wiimote State message containing one complete Wiimote state

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct State {
    /// -----------------------------
    ///  Header
    /// ----------------------
    pub header: std_msgs::msg::rmw::Header,

    /// ----------------------
    ///  Gyro reading
    /// -----------------------------
    ///  In radians/sec. If reading is invalid,
    ///  for instance b/c no gyro is attached to the Wii, then
    ///  set first element of covariance to -1 (same as imu_data.msg):
    ///  Covariance matrix (roll, pitch, yaw) in radians^2/sec^2.
    pub angular_velocity_zeroed: geometry_msgs::msg::rmw::Vector3,


    // This member is not documented.
    #[allow(missing_docs)]
    pub angular_velocity_raw: geometry_msgs::msg::rmw::Vector3,


    // This member is not documented.
    #[allow(missing_docs)]
    pub angular_velocity_covariance: [f64; 9],

    /// ----------------------
    ///  Accelerometer reading
    /// -----------------------------
    ///  Acceleration in m/sec^2. Covariance matrix
    ///  (x,y,z) in m^2/sec^4.  (all same as imu_data.msg)
    pub linear_acceleration_zeroed: geometry_msgs::msg::rmw::Vector3,


    // This member is not documented.
    #[allow(missing_docs)]
    pub linear_acceleration_raw: geometry_msgs::msg::rmw::Vector3,


    // This member is not documented.
    #[allow(missing_docs)]
    pub linear_acceleration_covariance: [f64; 9],

    /// ------------------------------
    ///  Nunchuk Accelerometer reading
    /// ------------------------------
    pub nunchuk_acceleration_zeroed: geometry_msgs::msg::rmw::Vector3,


    // This member is not documented.
    #[allow(missing_docs)]
    pub nunchuk_acceleration_raw: geometry_msgs::msg::rmw::Vector3,

    /// -----------------
    ///  Nunchuk Joystick
    /// -----------------
    pub nunchuk_joystick_zeroed: [f32; 2],


    // This member is not documented.
    #[allow(missing_docs)]
    pub nunchuk_joystick_raw: [f32; 2],

    /// ----------------------
    ///  Wiimote buttons
    /// -----------------------------
    ///  Mappings from Wiimote button name
    ///  to array position are defined above.
    pub buttons: [bool; 11],


    // This member is not documented.
    #[allow(missing_docs)]
    pub nunchuk_buttons: [bool; 2],

    /// ----------------------
    ///  Wiimote LED states:
    /// -----------------------------
    pub leds: [bool; 4],

    /// ----------------------
    ///  Wiimote Rumble
    /// -----------------------------
    ///  State (True or False)
    pub rumble: bool,

    /// ----------------------
    ///  IR Light sensor (Camera)
    /// -----------------------------
    ///  The Wiimote handles up to four light sources,
    ///  and the wiimote_controller software is written to
    ///  that limit as well. For future expansion
    ///  we make the following array extensible, rather
    ///  than locking its length down to four:
    pub ir_tracking: rosidl_runtime_rs::Sequence<super::super::msg::rmw::IrSourceInfo>,

    /// ----------------------
    ///  Wiimote battery
    /// -----------------------------
    ///  A battery reading consists of two numbers:
    ///  the battery percentage, and the raw reading.
    ///  Maximum battery is 208 units (unknown how this
    ///  relates to electrical properties):
    pub raw_battery: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub percent_battery: f32,

    /// ----------------------
    ///  Time of most recent zeroing:
    /// -----------------------------
    pub zeroing_time: builtin_interfaces::msg::rmw::Time,

    /// ----------------------
    ///  Error vector
    /// -----------------------------
    ///  For error condition definitions see wiimoteConstants.py
    ///  Value of zero means all is well. (Currently NOT used).
    pub errors: u64,

}

impl State {
    /// Note: For ease of manipulation this message often uses
    ///       int8[] when a bit vector would work. One might
    ///       wish to use uint8[], but then Python takes
    ///       the respective structure as a string and disallows
    ///       item assignment.
    pub const INVALID: i8 = -1;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const INVALID_FLOAT: f32 = -1.0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_1: i8 = 0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_2: i8 = 1;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_A: i8 = 2;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_B: i8 = 3;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_PLUS: i8 = 4;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_MINUS: i8 = 5;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_LEFT: i8 = 6;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_RIGHT: i8 = 7;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_UP: i8 = 8;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_DOWN: i8 = 9;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_HOME: i8 = 10;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_Z: i8 = 0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_BTN_C: i8 = 1;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_X: i8 = 0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_Y: i8 = 1;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_A: i8 = 2;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_B: i8 = 3;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_PLUS: i8 = 4;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_MINUS: i8 = 5;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_LEFT: i8 = 6;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_RIGHT: i8 = 7;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_UP: i8 = 8;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_DOWN: i8 = 9;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_HOME: i8 = 10;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_L: i8 = 11;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_R: i8 = 12;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_ZL: i8 = 13;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const MSG_CLASSIC_BTN_ZR: i8 = 14;

}


impl Default for State {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wiimote_msgs__msg__State__init(&mut msg as *mut _) {
        panic!("Call to wiimote_msgs__msg__State__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for State {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__State__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__State__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__State__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for State {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for State where Self: Sized {
  const TYPE_NAME: &'static str = "wiimote_msgs/msg/State";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wiimote_msgs__msg__State() }
  }
}


#[link(name = "wiimote_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wiimote_msgs__msg__TimedSwitch() -> *const std::ffi::c_void;
}

#[link(name = "wiimote_msgs__rosidl_generator_c")]
extern "C" {
    fn wiimote_msgs__msg__TimedSwitch__init(msg: *mut TimedSwitch) -> bool;
    fn wiimote_msgs__msg__TimedSwitch__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<TimedSwitch>, size: usize) -> bool;
    fn wiimote_msgs__msg__TimedSwitch__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<TimedSwitch>);
    fn wiimote_msgs__msg__TimedSwitch__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<TimedSwitch>, out_seq: *mut rosidl_runtime_rs::Sequence<TimedSwitch>) -> bool;
}

// Corresponds to wiimote_msgs__msg__TimedSwitch
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// TimedSwitch allows sender to:
///    o turn a switch on,
///    o turn a switch off, and
///    o repeat an on/off pattern forever or for a
///          given number of times.
/// Fields (refer to definitions of constants in the definition body):
///     o switch_mode:
///         ON: turn on  (num_cycles and pulse_pattern fields are ignored)
///        OFF: turn off (num_cycles and pulse_pattern fields are ignored)
///  NO_CHANGE: leave LED in its current state
///     REPEAT: repeat an on/off pattern for as long
///             as is indicated in the num_cycles field. The
///             pattern is defined in the pulse_pattern field.
///
///     o num_cycles:
///          n>=0: run the pattern that is defined in pulse_pattern
///                n times.
///          n==FOREVER: run the pattern that is defined in pulse_pattern
///                       until a new TimedSwitch message is sent.
///
///     o pulse_pattern:
///          A series of time durations in fractions of a second. The
///          first number is the duration for having the switch on.
///          The second number is the duration for which the switch
///          is off. The third is an 'on' period again, etc.
///          A pattern is terminated with the end of the array.
///
///          Example: [1,1] specifies an on-off sequence of 1 second.

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TimedSwitch {

    // This member is not documented.
    #[allow(missing_docs)]
    pub switch_mode: i8,


    // This member is not documented.
    #[allow(missing_docs)]
    pub num_cycles: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub pulse_pattern: rosidl_runtime_rs::Sequence<f32>,

}

impl TimedSwitch {

    // This constant is not documented.
    #[allow(missing_docs)]
    pub const ON: i8 = 1;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const OFF: i8 = 0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const NO_CHANGE: i8 = -2;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const REPEAT: i8 = -1;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const FOREVER: i8 = -1;

}


impl Default for TimedSwitch {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wiimote_msgs__msg__TimedSwitch__init(&mut msg as *mut _) {
        panic!("Call to wiimote_msgs__msg__TimedSwitch__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for TimedSwitch {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__TimedSwitch__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__TimedSwitch__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wiimote_msgs__msg__TimedSwitch__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for TimedSwitch {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for TimedSwitch where Self: Sized {
  const TYPE_NAME: &'static str = "wiimote_msgs/msg/TimedSwitch";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wiimote_msgs__msg__TimedSwitch() }
  }
}


