## Camera config

Este paquete puede publicar imágenes comprimidas desde una cámara RTSP.
La configuración se controla con parámetros del nodo (`camera`, `camera_url`,
`camera_topic`, `image_compression_ratio`, `resize_image`).

Dependencia útil para debug visual:

`sudo apt install ffmpeg`

Test camera (is neccesary to use -X option in ssh)

`ffplay rtsp://192.168.0.61/axis-media/media.amp?camera=1&streamprofile=Bandwidth`