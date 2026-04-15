import math
from typing import Tuple

# Lightweight local tangent-plane approximation.
# Returns x East (m), y North (m) relative to an origin lat/lon.
# Good for “local around robot” distances (meters to a few km).
_EARTH_RADIUS_M = 6378137.0  # WGS84 semi-major axis


def ll2xy(lat_deg: float, lon_deg: float, origin_lat_deg: float, origin_lon_deg: float) -> Tuple[float, float]:
    lat0 = math.radians(float(origin_lat_deg))
    lon0 = math.radians(float(origin_lon_deg))
    lat1 = math.radians(float(lat_deg))
    lon1 = math.radians(float(lon_deg))

    dlat = lat1 - lat0
    dlon = lon1 - lon0

    # Use mean latitude for slightly better accuracy away from the origin.
    lat_mean = 0.5 * (lat0 + lat1)

    x_east_m = dlon * math.cos(lat_mean) * _EARTH_RADIUS_M
    y_north_m = dlat * _EARTH_RADIUS_M
    return (x_east_m, y_north_m)
