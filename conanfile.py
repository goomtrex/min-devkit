import os
from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import cmake_layout, CMake
from conan.tools.build import check_min_cppstd

class XTALMath__Conan(ConanFile):
	required_conan_version = ">=2.0.0"

	# Info:
	name    = "min-devkit--xtal"
	version = "0.0.0"
	license = "Boost Software License 1.0"

	url     = "https://github.com/synthetic-methods/min-devkit--xtal"
	author  = "GoomTrex goomtrex@gmail.com"
	
	topics  = ("C++20"
	, "Template Metaprogramming", "TMP", "CRTP", "std::ranges", "range-v3"
	, "Real-Time", "Scheduling", "Stream Processing", "Signal Processing", "DSP", "FFT"
	, "Embedded", "Audio", "AudioUnit", "AudioKit", "JUCE", "VST", "AU"
	, "Modular", "Functors", "Combinators"
	, "VTable", "Branchless", "Control"
	)
	description = """
	min8-xtal is a cross-platform library for mathemusical Digital Signal Processing (DSP).
	"""

	# Build:
	settings = "os", "arch", "compiler", "build_type"
	generators = "CMakeDeps", "CMakeToolchain"
	
#	def build(self):
#		cmake = CMake(self)
#		self.run('cmake --build .')

	def layout(self):
		cmake_layout(self)

	def validate(self):
		check_min_cppstd(self, 20)

	def requirements(self):
		self.requires("xtal/0.0.0", transitive_headers=True)

	# Package:
	no_copy_source = True
	exports_sources = "include*.h*", "include*.i*", "include/module.modulemap", "CMakeLists.txt", "UNLICENSE.*"

	def package(self):
		for glob in self.exports_sources:
			copy(self, glob, self.source_folder, self.package_folder)

	def package_id(self):
		self.info.clear()

	def package_info(self):
		self.cpp_info.bindirs = []
		self.cpp_info.libdirs = []

