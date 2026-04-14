# cython: language_level=3
import numpy as np
cimport numpy as cnp
import cv2
from libc.stdint cimport int32_t
from cython cimport boundscheck, wraparound
from scipy.ndimage import rotate

@boundscheck(False)
@wraparound(False)
def draw_UGV_icon(cnp.ndarray[cnp.uint8_t, ndim=3] image_array, int pix_x, int pix_y,
                  cnp.ndarray[cnp.uint8_t, ndim=3] j8_icon, double zoom_scale, double UGV_yaw):
    cdef int icon_width = j8_icon.shape[1]
    cdef int icon_length = j8_icon.shape[0]
    cdef int x_coor, y_coor, n, m
    cdef cnp.ndarray[cnp.uint8_t, ndim=3] resized_icon_array
    cdef double angle_degrees = UGV_yaw * 180.0 / np.pi
    cdef (int, int) center = (icon_width // 2, icon_length // 2)
    cdef cnp.ndarray rotation_matrix = cv2.getRotationMatrix2D(center, angle_degrees, 1.0)

    # Use memory views for efficient array manipulation
    cdef cnp.uint8_t[:, :, :] image = image_array
    cdef cnp.uint8_t[:, :, :] j8_icon_view = j8_icon
    cdef cnp.uint8_t[:, :, :] resized_icon
    cdef cnp.uint8_t[:, :, :] rotated_icon

    
    #resized_icon = j8_icon_view

    # Rotate using OpenCV
    rotated_icon = rotate(j8_icon_view, angle_degrees)

    cdef int h = rotated_icon.shape[0]
    cdef int w = rotated_icon.shape[1]
    cdef int x_offset = pix_x - h // 2
    cdef int y_offset = pix_y - w // 2

    # Efficient pixel assignment
    for n in range(h):
        for m in range(w):
            if rotated_icon[n, m, 0] != 0 or rotated_icon[n, m, 1] != 0 or rotated_icon[n, m, 2] !=0:
                x_coor = x_offset + n
                y_coor = y_offset + m
                if 0 <= x_coor < image.shape[0] and 0 <= y_coor < image.shape[1]:
                    image[x_coor, y_coor, 0] = rotated_icon[n, m, 0]
                    image[x_coor, y_coor, 1] = rotated_icon[n, m, 1]
                    image[x_coor, y_coor, 2] = rotated_icon[n, m, 2]

    return image_array

