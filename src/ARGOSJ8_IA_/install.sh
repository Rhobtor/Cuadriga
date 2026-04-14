#!/bin/bash

git clone git@github.com:Robotics-Mechatronics-UMA/lcm_pkg.git

# Assuming the main directory is named lcm_pkg after the repo, but change if different
cd lcm_pkg

# Create a build directory and enter it
mkdir build && cd build

# Run cmake
cmake ..

# Compile the software
make

# Install the software
sudo make install

# Update the dynamic linker run-time bindings
sudo ldconfig -v

echo "LCM installation complete."
cd ../..
rm -rf lcm_pkg

git clone git@github.com:geographiclib/geographiclib.git

# Assuming the main directory is named lcm_pkg after the repo, but change if different
cd geographiclib

# Create a build directory and enter it
mkdir build && cd build

# Run cmake
cmake ..

# Compile the software
make

# Install the software
sudo make install


echo "geographiclib installation complete."
cd ../..
rm -rf geographiclib
rosdep install --from-paths src --ignore-src -r -y
apt-get install -y libxcb-xinerama0 libxcb-cursor0
apt-get install -y python3-pip
pip install cvxpy
pip3 install transforms3d
pip3 install PySide6
pip3 install utm
pip3 install pyproj
