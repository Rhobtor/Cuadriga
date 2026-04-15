import utm
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped
import pyproj
import math

class PathUtils:
    def __init__(self, UTM_x_up_north, UTM_y_up_north, m_to_pix_x, m_to_pix_y):
        self.UTM_x_up_north = UTM_x_up_north
        self.UTM_y_up_north = UTM_y_up_north
        self.m_to_pix_x = m_to_pix_x
        self.m_to_pix_y = m_to_pix_y
        # Define the UTM projection and WGS84 datum for use in UTM to latlon conversion
        self.utm_proj = pyproj.Proj(proj='utm', zone=30, ellps='WGS84', datum='WGS84', south=False)
        self.wgs84_proj = pyproj.Proj(proj='latlong', datum='WGS84')


    def convert_path_to_utm(self, path):
        utm_path = Path()
        utm_path.header = path.header  # Copy the header from the original path
        for pose_stamped in path.poses:
            latitude = pose_stamped.pose.position.x
            longitude = pose_stamped.pose.position.y

            # Convert latitude and longitude to UTM
            easting, northing, zone_number, zone_letter = utm.from_latlon(latitude, longitude)

            # Create a new PoseStamped for the UTM path
            utm_pose_stamped = PoseStamped()
            utm_pose_stamped.header.frame_id = 'utm'
            utm_pose_stamped.pose.position.x = easting
            utm_pose_stamped.pose.position.y = northing
            # Optionally, you can also encode zone information in the pose

            utm_path.poses.append(utm_pose_stamped)

        return utm_path
    
    def convert_pixel_path_to_latlon(self, pixel_path):
        gps_path = Path()
        gps_path.header.frame_id = 'll' # Assuming you have a similar header structure

        for pix_x, pix_y in pixel_path:
            # Convert pixels to UTM
            utm_x, utm_y = self.pix_to_UTM(pix_y, pix_x)
            print('utm_x ' + str(utm_x/10))
            print('utm_y ' + str(utm_y/10))

            # Convert UTM to latitude and longitude using pyproj
            longitude, latitude = pyproj.transform(self.utm_proj, self.wgs84_proj, utm_y, utm_x)

            # Create a new PoseStamped for the GPS path
            latlon_pose = PoseStamped()
            latlon_pose.header.frame_id = 'gps'
            latlon_pose.pose.position.x = latitude
            latlon_pose.pose.position.y = longitude

            gps_path.poses.append(latlon_pose)

        return gps_path

    
    def UTM_to_pix(self, utm_x, utm_y):
        pix_x = int((self.UTM_x_up_north - utm_x) / self.m_to_pix_x)
        pix_y = int((utm_y - self.UTM_y_up_north) / self.m_to_pix_y)
        return pix_x, pix_y
    
    def pix_to_UTM(self, pix_x, pix_y):
        utm_x = self.UTM_x_up_north - pix_x * self.m_to_pix_x
        utm_y = pix_y * self.m_to_pix_y + self.UTM_y_up_north
        return utm_x, utm_y

    def interpolate_point(self, start, end, fraction):
        """Interpolates a point between start and end."""
        return (start[0] + (end[0] - start[0]) * fraction, start[1] + (end[1] - start[1]) * fraction)

    def resample_path(self, points, max_distance=10):
        if not points:
            return []

        resampled = [points[0]]
        last_point = points[0]

        for next_point in points[1:]:
            # Calculate the distance between the last point and the next point
            distance = math.sqrt((next_point[0] - last_point[0]) ** 2 + (next_point[1] - last_point[1]) ** 2)
            if distance > max_distance:
                # Number of segments needed
                num_segments = int(distance / max_distance)
                for i in range(1, num_segments + 1):
                    fraction = i / (num_segments + 1)
                    interpolated = self.interpolate_point(last_point, next_point, fraction)
                    resampled.append(interpolated)
            
            resampled.append(next_point)
            last_point = next_point

        return resampled