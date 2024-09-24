# ABB Externally Guided Motion C++ Wrapper
A C++ wrapper for Linux to control an ABB robot via the Externally Guided Motion (EGM) interface.

The build process relies only on git and cmake. The following build pipeline is written for Linux, but Visual Studio with WSL can also be used.

Windows is only needed to get the EGM instruction set, which is shipped with the ABB RobotStudio software.

## Other useful sources
- https://github.com/madelinegannon/abb_egm_hello_world
- https://github.com/fronchetti/egm-for-abb-robots
- [Application Manual RW7](https://github.com/geludwig/abb-externally-guided-motion/blob/main/documentation/ExternallyGuidedMotionRW7-en.pdf)

## Disclaimer
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Tested on
- Ubuntu 22.04 LTS
- Ubuntu 22.04 LTS patched with PREEMPT_RT kernel

## Prerequisites
- ABB robot and hardware controller with Externally Guided Motion AddOn or RobotStudio for testing
- Windows system with RobotStudio installed
- Linux system (preferably with PREEMPT_RT kernel)

## Get sources
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
mkdir ../src/inc
cp -a src/google/. ../src/inc/google
mkdir ../src/lib
cp libprotobuf.a ../src/lib
cd ..
```
### ABB Proto Instruction Set
TODO

Precompiled instruction set (egm.pb.cpp and egm.pb.h) is included for now.

## Build demo project
The included demo (demo.cpp, demo.h) has all the necessary instructions to initiate an EGM connection and do a cartesic movement. In the demo, its a movement from [x1000,y0,z1000] to [x1500,y0,z1000].

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
