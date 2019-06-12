Param(
    [parameter(Mandatory=$true)]
    [string]
	$build_root
)

$ErrorActionPreference = "Stop"
$src_dir = "$(pwd)"
./create_windows_build_env.ps1 $build_root
cd $build_root
cd build.mingw.debug
cmake --build .
cmake --build . --target test
cmake --build . --target install
cd ../build.mingw.release
cmake --build .
cmake --build . --target test
cmake --build . --target install
cd ../build.vs
cmake --build . --config Debug
cmake --build . --config Release
cmake --build . --config Debug --target RUN_TESTS
cmake --build . --config Release --target RUN_TESTS
cmake --build . --config Debug --target INSTALL
cmake --build . --config Release --target INSTALL
cd $src_dir
