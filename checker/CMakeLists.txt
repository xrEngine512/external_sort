cmake_minimum_required(VERSION 2.8)

AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

add_executable(checker ${SOURCE_FILES})
