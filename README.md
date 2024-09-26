# ABB Externally Guided Motion C++ Wrapper
A C++ wrapper for Linux to control an ABB robot via the Externally Guided Motion (EGM) interface.

The build pipeline is written for Linux and relies only on git and cmake, but Visual Studio with WSL can also be used.

Windows is only needed to get the EGM Proto Instruction Set through RobotStudio. Otherwise you can use the links provided under [ABB EGM Proto Instruction Set](https://github.com/geludwig/abb-externally-guided-motion/tree/main?tab=readme-ov-file#abb-egm-proto-instruction-set).

## Other useful sources
- https://github.com/madelinegannon/abb_egm_hello_world
- https://github.com/fronchetti/egm-for-abb-robots
- [Application Manual RW7](https://github.com/geludwig/abb-externally-guided-motion/blob/main/documentation/ExternallyGuidedMotionRW7-en.pdf)

## Disclaimer
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Tested on
- Ubuntu 22.04 LTS (patched with PREEMPT_RT kernel)

## Limitations
Only cartesic coordinates, quaternions and speedref is implemented at the moment.

## Prerequisites
- ABB robot and hardware controller with Externally Guided Motion AddOn or RobotStudio for testing
- Linux system (preferably with PREEMPT_RT kernel)

## Build sources
### Download Project
```
git clone https://github.com/geludwig/abb-externally-guided-motion
cd abb-externally-guided-motion/egmdemo
```
### Google Protobuf
The EGM API is based on Google Protobuf. To build the *libprotobuf.a* lib, use the following commands. In the following code block, release v28.2 is specified, but newer releases should also work. Check that the current dir is *egmdemo/* !
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
mkdir ../src/inc && cp -a src/google/. ../src/inc/google
mkdir ../src/lib && cp libprotobuf.a ../src/lib
cd ..
```
### ABB EGM Proto Instruction Set
Use the correct RobotWare (aka hardware controller) for your robot, for example *OmniCore* or *IRC5*.

The RobotWare contains the EGM Proto Instruction Set which needs to be compiled into C++ source files. You can get it via RobotStudio or use the links below. Keep in mind, that these links may be not the latest available version.

[RobotWare OmniCore 7.15.2](https://robotstudiocdn.azureedge.net/distributionpackages/RobotWare/ABB.RobotWare-7.15.2.rspak)

[RobotWare IRC5 6.15.7032](https://robotstudiocdn.azureedge.net/distributionpackages/RobotWare/ABB.RobotWare-6.15.7032.rspak)

Use the following commands to compile *egm.proto* to *egm.pb.cpp* *and egm.pb.h*. Check that the current dir is *egmdemo/* !

```
mkdir abbproto && cd abbproto
unzip ABB.RobotWare-6.15.7032.rspak
cp ABB.RobotWare-6.15.7032/RobotPackages/RobotWare_RPK_6.15.7032/utility/Template/EGM/egm.proto .
cp ../protobuf/protoc .
./protoc egm.proto --cpp_out=./
cp egm.pb.h ../src/egm/egm.pb.h && cp egm.pb.cc ../src/egm/egm.pb.cpp
cd ..
```

## Build demo project
The included demo (*demo.cpp, demo.h*) has all the necessary instructions to initiate an EGM connection and do a cartesic movement. In the demo, its a movement from [x1000,y0,z1000] to [x1500,y0,z1000].

Tool configuration, coordinate system selection, offset frames, etc. is all done inside the RAPID code. Its wise to check everything inside RobotStudio first, before running it on a real robot.

Check that the demo project has the following folder structure.
```
.
├── egmdemo
│   ├── CMakeLists.txt
│   ├── CMakePresets.json
│   ├── demo.cpp
│   ├── demo.h
│   ├── src
│   │   ├── egm
│   │   │   ├── egm.pb.cpp
│   │   │   ├── egm.pb.h
│   │   │   ├── EgmPositions.h
│   │   │   ├── ...
│   │   ├── inc
│   │   │   └── google
│   │   │   │   └── protobuf
│   │   │   │   │   ├── compiler
│   │   │   │   │   ├── io
│   │   │   │   │   ├── ...
│   │   └── lib
│   │   │   └── libprotobuf.a
```

Build project using cmake. Check that the current dir is *egmdemo/* !
```
mkdir build
cmake -B build -DCMAKE_BUILD_TYPE=Linux-Release
cmake --build build
```

Run.
```
./build/egmdemo
```

## What does ... ?
- EgmPositions.h : Cartesic feedback and target positions of the robot as structure.
- EgmSocket.h : UDP socket functions as structure.
- EgmProto.h : Send and receive messages from the robot.

Because everything is defined as structure, its easy to talk to multiple robots. Just initialize each robot with its own structure.
```
// Robot 0
EgmPositionsFeedback egmFeedback0;
EgmPositionsTarget egmTarget0;
UdpSocket socket0;
socket0.port = 6510;
receiveRobotMessage(socket0, egmFeedback0);
sendRobotMessage(socket0, egmTarget0);

// Robot 1
EgmPositionsFeedback egmFeedback1;
EgmPositionsTarget egmTarget1;
UdpSocket socket1;
socket1.port = 6511;
receiveRobotMessage(socket1, egmFeedback1);
sendRobotMessage(socket1, egmTarget1);
```

## ABB Robot Program
The following example RAPID code is needed to control the robot with EGM. Look up the ABB Externally Guided Motion Application Manual for a more detailed explanation.
```
PROC EGM()
	MotionProcessModeSet(LOW_SPEED_STIFF_MODE);
	MoveJ StartPos, v100, fine, tool0, \WObj0;
	
	EGMReset egmID1;
	EGMGetID egmID1;
	
	egmSt1:=EGMGetState(egmID1);
	IF egmSt1 <= EGM_STATE_CONNECTED THEN
		EGMSetupUC ROB_1, egmID1, "EGMConfig", "EGMDevice:" \pose \CommTimeout:=60;
	ENDIF
	
	EGMActPose egmID1 \StreamStart \Tool:=tool0 \WObj:=wobj0,
		corrFrameEGM, EGM_FRAME_BASE,
		tool0.tframe, EGM_FRAME_Base,
		\x:=egm_minmax_lin1 \y:=egm_minmax_lin1
		\z:=egm_minmax_lin1 \rx:=egm_minmax_rot1
		\ry:=egm_minmax_rot1 \rz:=egm_minmax_rot1
		\SampleRate:=4 MaxSpeedDeviation:=99;
	
	EGMRunPose egmID1, EGM_STOP_HOLD \NoWaitCond \x \y \z \Rx \Ry \Rz \CondTime:=99 \RampInTime:=0.01 \RampOutTime:=0.01;
	
	egmSt1:=EGMGetState(egmID1);
	
	IF egmSt1 = EGM_STATE_CONNECTED THEN
		EGMReset egmID1;
	ENDIF
ENDPROC
```
