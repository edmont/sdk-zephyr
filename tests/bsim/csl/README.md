# Setup sdk-nrf project #

Follow this instruction:
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/getting_started/installing.html

Beyond system and Python packages listed in “nRF Connect SDK - getting started” tutorial, the following packages are required to build Softdevice Controller and MPSL:

```
apt install doxygen
pip install GitPython
```

When basic project is established, then download additional groups:

```
west config manifest.group-filter "+babblesim,+dragoon,+nrf-802154"
west update
```

# Build BabbleSim components #

```
export ZEPHYR_BASE=${HOME}/repos/ncs/zephyr
export BSIM_OUT_PATH=${HOME}/repos/ncs/tools/bsim
export BSIM_COMPONENTS_PATH=${HOME}/repos/ncs/tools/bsim/components

cd ${HOME}/repos/ncs/tools/bsim/
make everything -j 8

cd ${HOME}/repos/ncs
```

# Build and run Networking tests #

```
export ZEPHYR_BASE=${HOME}/repos/ncs/zephyr
export BSIM_OUT_PATH=${HOME}/repos/ncs/tools/bsim
export BSIM_COMPONENTS_PATH=${HOME}/repos/ncs/tools/bsim/components

export WORK_DIR=/tmp/bsim_out
export cmake_args="-DCONFIG_COVERAGE=y -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCONFIG_BT_LL_SOFTDEVICE_BUILD_TYPE_SRC_DEBUG=y -DCONFIG_MPSL_BUILD_TYPE_SRC_DEBUG=y -DCONFIG_NRF_802154_SOURCE_INTERNAL=y"
${HOME}/repos/ncs/zephyr/tests/bsim/csl/compile.sh

export RESULTS_FILE=${WORK_DIR}/bsim_results.xml
export SEARCH_PATH=${HOME}/repos/ncs/zephyr/tests/bsim/csl
```

# Run test #

```
${HOME}/repos/ncs/zephyr/tests/bsim/csl/csl_test.sh
```

# Get pcap

```
${BSIM_COMPONENTS_PATH}/ext_2G4_phy_v1/dump_post_process/csv2pcap_15.4.py -o /tmp/mytrace.pcap ${HOME}/repos/ncs/tools/bsim/results/csl_test/d_2G4*.Tx.csv
```

# Debug

Replace the execution of one of the devices in the test script by a manual run of the binary in GDB.

Increment `EXECUTE_TIMEOUT` to a large enough value for manual debugging.

```
cd ${HOME}/repos/ncs/tools/bsim/bin/ # To make encryption work
gdb --args ${WORK_DIR}/tests/bsim/csl/bs_nrf52_bsim_tests_bsim_csl__prj_conf/zephyr/zephyr.exe -v=2 -s="csl_test" -d=1 -RealEncryption=1 -testid=parent -argstest 0 20 120
gdb --args ${WORK_DIR}/tests/bsim/csl/bs_nrf52_bsim_tests_bsim_csl__prj_conf/zephyr/zephyr.exe -v=2 -s="csl_test" -start_offset=10e6 -d=0 -RealEncryption=1 -testid=child -argstest 2
```
