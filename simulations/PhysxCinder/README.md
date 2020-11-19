# Integrating Nvidia Physx with Cinder

## Building PhysxCinder using CMake

Make sure the following are set in your environment variables:
- CINDER_PATH - path to the [cloned repository](git://github.com/cinder/Cinder.git)
- PHYSX_PATH - path to the [`{cloned repository}/physx` directory](git://github.com/NVIDIAGameWorks/PhysX.git)

Configure and generate the project then build using `cmake --build ./build/`.
