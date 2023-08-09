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

export WORK_DIR=${HOME}/repos/ncs/outcomes/bsim_net
export cmake_args="-DCONFIG_COVERAGE=y -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCONFIG_BT_LL_SOFTDEVICE_BUILD_TYPE_SRC_DEBUG=y -DCONFIG_MPSL_BUILD_TYPE_SRC_DEBUG=y -DCONFIG_NRF_802154_SOURCE_INTERNAL=y"
./zephyr/tests/bsim/csl/compile.sh

export RESULTS_FILE=${HOME}/repos/ncs/outcomes/bsim_net/bsim_results.xml
export SEARCH_PATH=${HOME}/repos/ncs/zephyr/tests/bsim/net
```

# Run test #

```
./zephyr/tests/bsim/csl/csl_test.sh
```

# Get pcap

```
${BSIM_COMPONENTS_PATH}/ext_2G4_phy_v1/dump_post_process/csv2pcap_15.4.py -o /tmp/mytrace.pcap ${HOME}/repos/ncs/tools/bsim/results/csl_test/d_2G4*.Tx.csv
```