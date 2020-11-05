# Integrating Nvidia Physx with Cinder

## Building PhysxCinder using CMake

Make sure the following are set in your environment variables:
- CINDER_PATH - path to the [cloned repository](git://github.com/cinder/Cinder.git)
- PHYSX_PATH - path to the [`{cloned repository}/physx` directory](git://github.com/NVIDIAGameWorks/PhysX.git)

Configure and generate the project then build using `cmake --build ./build/`.
When building for Windows, PhysX DLL's must be in the same directory as the executable, therefore, the runtime directory is set to `${CMAKE_CURRENT_BINARY_DIR}/externals/physx/bin/${PLATFORM_BIN_NAME}/`, e.g. `PhysxCinder\externals\physx\bin\win.x86_32.vc142.mt\debug\PhysxCinder.exe`
