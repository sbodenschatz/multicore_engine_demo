Param(
    [parameter(Mandatory=$true)]
    [string]
	$build_root
)

$src_dir = "$(pwd)"
cd $build_root
mkdir build.mingw.debug -ea 0
cd build.mingw.debug
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="$((get-item $pwd).parent.FullName)\install.mingw" $src_dir
mkdir ../build.mingw.release -ea 0
cd ../build.mingw.release
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$((get-item $pwd).parent.FullName)\install.mingw" $src_dir
mkdir ../build.vs -ea 0
cd ../build.vs
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX="$((get-item $pwd).parent.FullName)\install.vs" $src_dir
cd $src_dir
