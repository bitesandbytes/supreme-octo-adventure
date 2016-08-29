cmake_minimum_required(VERSION 3.0)
project(supreme_octo_adventure)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

set(GENERATOR_FILES
        generator/onehut/onehut.cpp
        generator/psr/psr.cpp
        generator/psr/psrMW.cpp
        generator/generator.cpp)

set(MODEL_FILES
        models/crbm/layers/conv.cpp
        models/crbm/layers/pool.cpp
        models/crbm/crbm.cpp
        models/msrbm/msrbm.cpp
        models/model.cpp)

set(SIMULATOR_FILES
        simulator/simulate.cpp
        simulator/simulateQ.cpp
        simulator/simulateQ2.cpp)

set(AGENT_FILES
        DynaQAgent.cpp
        QAgent.cpp)

set(EXP_FILES
        CRBMTestExperiment.cpp
        MSRBMTestExperiment.cpp
        QExperiment.cpp)

set(UTIL_FILES
        utils/putils.cpp
        utils/utils.cpp)

# Set executables here.
add_executable(ActualExperiment ActualExperiment.cpp ${SIMULATOR_FILES} ${GENERATOR_FILES} ${AGENT_FILES} ${MODEL_FILES} ${UTIL_FILES})
add_executable(CRBMTestExperiment CRBMTestExperiment.cpp ${SIMULATOR_FILES} ${GENERATOR_FILES} ${AGENT_FILES} ${MODEL_FILES} ${UTIL_FILES})
add_executable(MSRBMTestExperiment MSRBMTestExperiment.cpp ${SIMULATOR_FILES} ${GENERATOR_FILES} ${AGENT_FILES} ${MODEL_FILES} ${UTIL_FILES})
add_executable(QExperiment QExperiment.cpp ${SIMULATOR_FILES} ${GENERATOR_FILES} ${AGENT_FILES} ${MODEL_FILES} ${UTIL_FILES})