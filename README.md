# abb-externally-guided-motion
A C++ wrapper for Linux to control an ABB robot via the Externally Guided Motion (EGM) interface.

## Disclaimer
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Prerequisites
- ABB robot and hardware controller with Externally Guided Motion AddOn OR RobotStudio for testing
- Windows OS with ABB RobotStudio installed
- Linux OS (preferably with PREEMPT_RT kernel)

## Get sources
### Download Project
```
git clone https://github.com/geludwig/abb-externally-guided-motion
cd abb-externally-guided-motion/
```
### Google Protobuf
The EGM API is based on Google Protobuf. To build the *libprotobuf.a* lib, use the following commands.
```
git clone -b v28.2 https://github.com/protocolbuffers/protobuf.git
cd protobuf/
git submodule update --init --recursive
cmake .
cmake --build . --parallel 10
ctest --verbose
```
Copy the google source files and lib to the project src folder.
```
mkdir ../src/inc
cp -a src/google/. ../src/inc/google
mkdir ../src/lib
cp libprotobuf.a ../src/lib
```
### ABB Proto Instruction set
TODO
Precompiled instruction set (egm.pb.cpp and egm.pb.h) is included for now.

### Build project
Check that the demo project has the following folder structure.
```
.
├── CMakeLists.txt
├── CMakePresets.json
├── demo.cpp
├── demo.h
├── src
│   ├── egm
│   │   ├── egm.pb.cpp
│   │   ├── egm.pb.h
│   │   ├── EgmPositions.h
│   │   ├── ...
│   ├── inc
│   │   ├── google
│   │   │   ├── ...
│   └── lib
│   │   ├── libprotobuf.a
```

Build project using cmake.
```
mkdir build
cmake -B build -DCMAKE_BUILD_TYPE=Linux-Release
cmake --build build
```
