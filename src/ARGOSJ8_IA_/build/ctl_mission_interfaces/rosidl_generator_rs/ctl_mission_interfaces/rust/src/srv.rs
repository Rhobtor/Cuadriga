#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};




// Corresponds to ctl_mission_interfaces__srv__ChangeMode_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeMode_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub transition: i8,

}



impl Default for ChangeMode_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ChangeMode_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ChangeMode_Request {
  type RmwMsg = super::srv::rmw::ChangeMode_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        transition: msg.transition,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      transition: msg.transition,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      transition: msg.transition,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ChangeMode_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeMode_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,

}



impl Default for ChangeMode_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ChangeMode_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ChangeMode_Response {
  type RmwMsg = super::srv::rmw::ChangeMode_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        success: msg.success,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      success: msg.success,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      success: msg.success,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__GetMode_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetMode_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetMode_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::GetMode_Request::default())
  }
}

impl rosidl_runtime_rs::Message for GetMode_Request {
  type RmwMsg = super::srv::rmw::GetMode_Request;

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


// Corresponds to ctl_mission_interfaces__srv__GetMode_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetMode_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub mode: i8,

}



impl Default for GetMode_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::GetMode_Response::default())
  }
}

impl rosidl_runtime_rs::Message for GetMode_Response {
  type RmwMsg = super::srv::rmw::GetMode_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        mode: msg.mode,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      mode: msg.mode,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      mode: msg.mode,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ChangeSpeed_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeSpeed_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub speed: f32,

}



impl Default for ChangeSpeed_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ChangeSpeed_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ChangeSpeed_Request {
  type RmwMsg = super::srv::rmw::ChangeSpeed_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        speed: msg.speed,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      speed: msg.speed,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      speed: msg.speed,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ChangeSpeed_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeSpeed_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub succes: bool,

}



impl Default for ChangeSpeed_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ChangeSpeed_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ChangeSpeed_Response {
  type RmwMsg = super::srv::rmw::ChangeSpeed_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        succes: msg.succes,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      succes: msg.succes,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      succes: msg.succes,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigSimpleCtl_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigSimpleCtl_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigSimpleCtl_Request {
  type RmwMsg = super::srv::rmw::ConfigSimpleCtl_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        k_angle: msg.k_angle,
        points_in_window: msg.points_in_window,
        k_position: msg.k_position,
        v_forward: msg.v_forward,
        l_ahead_dist: msg.l_ahead_dist,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      k_angle: msg.k_angle,
      points_in_window: msg.points_in_window,
      k_position: msg.k_position,
      v_forward: msg.v_forward,
      l_ahead_dist: msg.l_ahead_dist,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      k_angle: msg.k_angle,
      points_in_window: msg.points_in_window,
      k_position: msg.k_position,
      v_forward: msg.v_forward,
      l_ahead_dist: msg.l_ahead_dist,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigSimpleCtl_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigSimpleCtl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigSimpleCtl_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigSimpleCtl_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigSimpleCtl_Response {
  type RmwMsg = super::srv::rmw::ConfigSimpleCtl_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        ack: msg.ack,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      ack: msg.ack,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      ack: msg.ack,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__GetPossibleTransitions_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetPossibleTransitions_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub structure_needs_at_least_one_member: u8,

}



impl Default for GetPossibleTransitions_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::GetPossibleTransitions_Request::default())
  }
}

impl rosidl_runtime_rs::Message for GetPossibleTransitions_Request {
  type RmwMsg = super::srv::rmw::GetPossibleTransitions_Request;

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


// Corresponds to ctl_mission_interfaces__srv__GetPossibleTransitions_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct GetPossibleTransitions_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub possible_transitions: Vec<i32>,

}



impl Default for GetPossibleTransitions_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::GetPossibleTransitions_Response::default())
  }
}

impl rosidl_runtime_rs::Message for GetPossibleTransitions_Response {
  type RmwMsg = super::srv::rmw::GetPossibleTransitions_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        possible_transitions: msg.possible_transitions.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        possible_transitions: msg.possible_transitions.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      possible_transitions: msg.possible_transitions
          .into_iter()
          .collect(),
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ChangeController_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeController_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub controller_type: std::string::String,

}



impl Default for ChangeController_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ChangeController_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ChangeController_Request {
  type RmwMsg = super::srv::rmw::ChangeController_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        controller_type: msg.controller_type.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        controller_type: msg.controller_type.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      controller_type: msg.controller_type.to_string(),
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ChangeController_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ChangeController_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,

}



impl Default for ChangeController_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ChangeController_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ChangeController_Response {
  type RmwMsg = super::srv::rmw::ChangeController_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        success: msg.success,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      success: msg.success,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      success: msg.success,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigPurePursuitCtrl_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigPurePursuitCtrl_Request {
  type RmwMsg = super::srv::rmw::ConfigPurePursuitCtrl_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        v_forward: msg.v_forward,
        l_ahead_dist: msg.l_ahead_dist,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      v_forward: msg.v_forward,
      l_ahead_dist: msg.l_ahead_dist,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      v_forward: msg.v_forward,
      l_ahead_dist: msg.l_ahead_dist,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigPurePursuitCtrl_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigPurePursuitCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigPurePursuitCtrl_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigPurePursuitCtrl_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigPurePursuitCtrl_Response {
  type RmwMsg = super::srv::rmw::ConfigPurePursuitCtrl_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        ack: msg.ack,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      ack: msg.ack,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      ack: msg.ack,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigStanleyCtrl_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigStanleyCtrl_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigStanleyCtrl_Request {
  type RmwMsg = super::srv::rmw::ConfigStanleyCtrl_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        v_forward: msg.v_forward,
        l_ahead_dist: msg.l_ahead_dist,
        k_error_lineal: msg.k_error_lineal,
        k_error_angular: msg.k_error_angular,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      v_forward: msg.v_forward,
      l_ahead_dist: msg.l_ahead_dist,
      k_error_lineal: msg.k_error_lineal,
      k_error_angular: msg.k_error_angular,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      v_forward: msg.v_forward,
      l_ahead_dist: msg.l_ahead_dist,
      k_error_lineal: msg.k_error_lineal,
      k_error_angular: msg.k_error_angular,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigStanleyCtrl_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigStanleyCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigStanleyCtrl_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigStanleyCtrl_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigStanleyCtrl_Response {
  type RmwMsg = super::srv::rmw::ConfigStanleyCtrl_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        ack: msg.ack,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      ack: msg.ack,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      ack: msg.ack,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigDynamicPureCtrl_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicPureCtrl_Request {
  type RmwMsg = super::srv::rmw::ConfigDynamicPureCtrl_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        look_ahead_dis: msg.look_ahead_dis,
        max_v_forward: msg.max_v_forward,
        max_ang_acc: msg.max_ang_acc,
        max_ang_dec: msg.max_ang_dec,
        max_lin_acc: msg.max_lin_acc,
        max_lin_dec: msg.max_lin_dec,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      look_ahead_dis: msg.look_ahead_dis,
      max_v_forward: msg.max_v_forward,
      max_ang_acc: msg.max_ang_acc,
      max_ang_dec: msg.max_ang_dec,
      max_lin_acc: msg.max_lin_acc,
      max_lin_dec: msg.max_lin_dec,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      look_ahead_dis: msg.look_ahead_dis,
      max_v_forward: msg.max_v_forward,
      max_ang_acc: msg.max_ang_acc,
      max_ang_dec: msg.max_ang_dec,
      max_lin_acc: msg.max_lin_acc,
      max_lin_dec: msg.max_lin_dec,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicPureCtrl_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigDynamicPureCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigDynamicPureCtrl_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigDynamicPureCtrl_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicPureCtrl_Response {
  type RmwMsg = super::srv::rmw::ConfigDynamicPureCtrl_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        ack: msg.ack,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      ack: msg.ack,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      ack: msg.ack,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigDynamicLAPureCtrl_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicLAPureCtrl_Request {
  type RmwMsg = super::srv::rmw::ConfigDynamicLAPureCtrl_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        look_ahead_v_gain: msg.look_ahead_v_gain,
        max_v_forward: msg.max_v_forward,
        max_ang_acc: msg.max_ang_acc,
        max_ang_dec: msg.max_ang_dec,
        max_lin_acc: msg.max_lin_acc,
        max_lin_dec: msg.max_lin_dec,
        speed_pow: msg.speed_pow,
        min_look_ahead_d: msg.min_look_ahead_d,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      look_ahead_v_gain: msg.look_ahead_v_gain,
      max_v_forward: msg.max_v_forward,
      max_ang_acc: msg.max_ang_acc,
      max_ang_dec: msg.max_ang_dec,
      max_lin_acc: msg.max_lin_acc,
      max_lin_dec: msg.max_lin_dec,
      speed_pow: msg.speed_pow,
      min_look_ahead_d: msg.min_look_ahead_d,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      look_ahead_v_gain: msg.look_ahead_v_gain,
      max_v_forward: msg.max_v_forward,
      max_ang_acc: msg.max_ang_acc,
      max_ang_dec: msg.max_ang_dec,
      max_lin_acc: msg.max_lin_acc,
      max_lin_dec: msg.max_lin_dec,
      speed_pow: msg.speed_pow,
      min_look_ahead_d: msg.min_look_ahead_d,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigDynamicLAPureCtrl_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigDynamicLAPureCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigDynamicLAPureCtrl_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigDynamicLAPureCtrl_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigDynamicLAPureCtrl_Response {
  type RmwMsg = super::srv::rmw::ConfigDynamicLAPureCtrl_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        ack: msg.ack,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      ack: msg.ack,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      ack: msg.ack,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigRegulatedPureCtrl_Request::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigRegulatedPureCtrl_Request {
  type RmwMsg = super::srv::rmw::ConfigRegulatedPureCtrl_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        look_ahead_dis: msg.look_ahead_dis,
        v_forward: msg.v_forward,
        r_min: msg.r_min,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      look_ahead_dis: msg.look_ahead_dis,
      v_forward: msg.v_forward,
      r_min: msg.r_min,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      look_ahead_dis: msg.look_ahead_dis,
      v_forward: msg.v_forward,
      r_min: msg.r_min,
    }
  }
}


// Corresponds to ctl_mission_interfaces__srv__ConfigRegulatedPureCtrl_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ConfigRegulatedPureCtrl_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub ack: bool,

}



impl Default for ConfigRegulatedPureCtrl_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::ConfigRegulatedPureCtrl_Response::default())
  }
}

impl rosidl_runtime_rs::Message for ConfigRegulatedPureCtrl_Response {
  type RmwMsg = super::srv::rmw::ConfigRegulatedPureCtrl_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        ack: msg.ack,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      ack: msg.ack,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      ack: msg.ack,
    }
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


