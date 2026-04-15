#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to cuadriga_ctl_platform_interfaces__msg__CmdThrottleMsg
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

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::CmdThrottleMsg::default())
  }
}

impl rosidl_runtime_rs::Message for CmdThrottleMsg {
  type RmwMsg = super::msg::rmw::CmdThrottleMsg;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        throttle: msg.throttle,
        steering: msg.steering,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      throttle: msg.throttle,
      steering: msg.steering,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      throttle: msg.throttle,
      steering: msg.steering,
    }
  }
}


// Corresponds to cuadriga_ctl_platform_interfaces__msg__KinematicDebugMsg
/// KinematicDebugMsg.msg
///
/// Estado de depuracion del modelo cinemático usado para generar
/// el mando low-level a partir de cmd_vel.
///
/// Todos los campos escalarizados de throttle/steering se publican en las
/// mismas unidades que CmdThrottleMsg. Las aceleraciones solicitadas se
/// expresan antes de aplicar la saturacion configurada.

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::KinematicDebugMsg::default())
  }
}

impl rosidl_runtime_rs::Message for KinematicDebugMsg {
  type RmwMsg = super::msg::rmw::KinematicDebugMsg;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        throttle: msg.throttle,
        steering: msg.steering,
        requested_throttle_acc: msg.requested_throttle_acc,
        requested_steering_acc: msg.requested_steering_acc,
        throttle_limited: msg.throttle_limited,
        steering_limited: msg.steering_limited,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      throttle: msg.throttle,
      steering: msg.steering,
      requested_throttle_acc: msg.requested_throttle_acc,
      requested_steering_acc: msg.requested_steering_acc,
      throttle_limited: msg.throttle_limited,
      steering_limited: msg.steering_limited,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      throttle: msg.throttle,
      steering: msg.steering,
      requested_throttle_acc: msg.requested_throttle_acc,
      requested_steering_acc: msg.requested_steering_acc,
      throttle_limited: msg.throttle_limited,
      steering_limited: msg.steering_limited,
    }
  }
}


