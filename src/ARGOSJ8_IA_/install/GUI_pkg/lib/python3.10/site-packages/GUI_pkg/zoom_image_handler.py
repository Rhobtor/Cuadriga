import cv2
import numpy as np
from PySide6.QtGui import QImage
from .cythonized_code.cythonize_fncs import draw_UGV_icon

class ZoomImageHandler:
    def __init__(self, map_image, icon_image, zoom_scale=1.0):
        self.map_image = map_image  # This should be a numpy array
        self.icon_image = icon_image  # This should also be a numpy array
        self.zoom_scale = zoom_scale
        self.zoom_corners = [0, map_image.shape[0], 0, map_image.shape[1]]

    def highlight_zoom_area(self, image, x, y):
        """ Highlight the area selected for zoom. """
        image[y - 10:y + 10, :, :] = [255, 255, 255]
        image[:, x - 10:x + 10, :] = [255, 255, 255]

    def calculate_zoom_scale_and_corners(self, clicked_pix_x, clicked_pix_y):
        """ Calculate the corners for the zoom area based on points closest to the origin. """
        if len(clicked_pix_x) == 2:
            # Determine the point closer to the origin (0,0)
            if clicked_pix_x[0] < clicked_pix_x[1]:
                origin_x, origin_y = clicked_pix_x[0], clicked_pix_y[0]
                width_x = clicked_pix_x[1]
            else:
                origin_x, origin_y = clicked_pix_x[1], clicked_pix_y[1]
                width_x = clicked_pix_x[0]

            # Calculate the width of the zoom area
            zoom_width = width_x - origin_x

            # Maintain the aspect ratio of the original image
            aspect_ratio = self.map.shape[0] / self.map.shape[1]  # height / width
            zoom_height = int(zoom_width * aspect_ratio)

            # Ensure zoom area is within image boundaries
            zoom_height = min(zoom_height, self.map.shape[0] - origin_y)
            zoom_width = int(zoom_height / aspect_ratio)

            self.zoom_corners = [origin_y, origin_y + zoom_height, origin_x, origin_x + zoom_width]



    def apply_zoom_with_aspect(self, image, start_x, start_y, end_x, end_y):
        if start_x >= end_x or start_y >= end_y:
            print("Invalid ROI dimensions.")
            return image  # Return the original image if ROI dimensions are invalid

        # Extract the region of interest
        roi = image[start_y:end_y, start_x:end_x]

        if roi.size == 0:
            print("Empty ROI.")
            return image  # Return the original image if ROI is empty

        # Calculate aspect ratios and continue as previously
        original_height, original_width = image.shape[:2]
        roi_height, roi_width = roi.shape[:2]
        original_aspect = original_width / original_height
        roi_aspect = roi_width / roi_height

        if roi_aspect > original_aspect:
            new_width = original_width
            new_height = int(new_width / roi_aspect)
        else:
            new_height = original_height
            new_width = int(new_height * roi_aspect)

        # Resize region of interest to new dimensions
        resized_roi = cv2.resize(roi, (new_width, new_height), interpolation=cv2.INTER_LINEAR)

        # Create a new blank image and place the resized ROI in the center
        final_image = np.zeros((original_height, original_width, 3), dtype=np.uint8)
        start_x = (original_width - new_width) // 2
        start_y = (original_height - new_height) // 2
        final_image[start_y:start_y+new_height, start_x:start_x+new_width] = resized_roi

        return final_image



    def print_UGV_pos(self, image, UGV_pix, rover_pix, base_pix, zoom_active, UGV_yaw):
        """ Draw the UGV position on the image. """
        [pix_x, pix_y] = UGV_pix
        image = draw_UGV_icon(image, pix_x, pix_y, self.icon_image, self.zoom_scale, UGV_yaw)  
        [pix_x_rover, pix_y_rover] = rover_pix
        image[(pix_x_rover - 1 ) : (pix_x_rover +1), (pix_y_rover - 1 ) : (pix_y_rover +1), :] = [0, 0, 255]
        [pix_x_base, pix_y_base] = base_pix
        image[(pix_x_base - 1 ) : (pix_x_base +1), (pix_y_base - 1 ) : (pix_y_base +1), :] = [0, 0, 255]

        if zoom_active:
            image = image[int(self.zoom_corners[0]):int(self.zoom_corners[1]), int(self.zoom_corners[2]):int(self.zoom_corners[3]), :]
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        return image

