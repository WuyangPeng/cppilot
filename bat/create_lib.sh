#!/bin/bash

# 获取脚本的绝对路径
script_path="$(readlink -f "$0")"

# 脚本所在的目录
script_dir="$(dirname "$script_path")"

# 脚本父目录
root_dir="$(dirname "$script_dir")"

# 定义关键子目录变量
deps_dir="${root_dir}/deps"

echo "deps_dir path is: ${deps_dir}"

cd ${deps_dir}

#! 编译boost库
boost_version="1.88.0"
boost_dir="boost_$(echo ${boost_version} | sed 's/\./_/g')"

echo "boost version: ${boost_version}"

boost_installed_txt="${deps_dir}/boost_installed.txt"
boost_download_txt="${deps_dir}/boost_download.txt"

if [ ! -f ${boost_installed_txt} ]; then

	if [ ! -f ${boost_dir}.tar.gz ]; then
		wget https://archives.boost.io/release/${boost_version}/source/${boost_dir}.tar.gz
	fi 
	
	if [ ! -f ${boost_download_txt} ]; then
		tar -zxvf ${boost_dir}.tar.gz
		rm -rf boost
		mv ${boost_dir} boost
		touch ${boost_download_txt}
	fi 
	
	cd boost

	if [ ! -f b2 ]; then
		./bootstrap.sh --with-libraries=all --with-toolset=gcc
	fi 
	
	./b2 install --prefix=${deps_dir}/boost/stage/ cxxflags="-std=c++20"
	touch ${boost_installed_txt}
	
fi 

#! 编译llama.cpp库

llama_cpp_installed_txt="${deps_dir}/llama_cpp_installed.txt"
llama_cpp_clone_txt="${deps_dir}/llama_cpp_clone.txt"

if [ ! -f ${llama_cpp_installed_txt} ]; then

    cd ${deps_dir}/
	
	if [ ! -f ${llama_cpp_clone_txt} ]; then
	
		rm -rf llama.cpp

		git clone https://github.com/ggml-org/llama.cpp
		
		if [ $? -eq 0 ]; then 
		
			cd llama.cpp
	 
			touch ${llama_cpp_clone_txt}
	
		fi
	
	fi 
	
	if [ -f ${llama_cpp_clone_txt} ]; then
	
		cd ${deps_dir}/llama.cpp
	
		mkdir -p build
		cd build
		mkdir -p debug
		cd debug

		cmake ../.. -DCMAKE_BUILD_TYPE=Debug
		make 
		
		cd ${deps_dir}/llama.cpp/build
		mkdir -p release
		cd release

		cmake ../.. -DCMAKE_BUILD_TYPE=Release
		make 
	 
		if [ $? -eq 0 ]; then 
	 
			touch ${llama_cpp_installed_txt}
	
		fi
	
	fi
	
fi
