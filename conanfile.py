from conans import ConanFile, CMake, tools

class YourLibraryConan(ConanFile):
    name = "nanojsonc"
    version = "1.0.0"
    license = "YourLicense"
    url = "https://github.com/saadshams/nanojsonc"
    description = "Your library description"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/saadshams/nanojsonc.git")
        self.run("cd nanojsonc && git checkout {0}".format(self.version))

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="nanojsonc")
        cmake.build()

    def package(self):
        # Copy headers
        self.copy("*.h", dst="include", src="nanojsonc/include")

        # Copy binaries
        self.copy("*", dst="lib", src="nanojsonc/lib")

        # Copy CMake package config files
        self.copy("*.cmake", dst="lib/cmake", src="nanojsonc/lib/cmake")

    def package_info(self):
        self.cpp_info.libs = ["nanojsonc"]
