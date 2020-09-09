project(msig)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( msig msig msig.cpp )
target_include_directories( msig PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( msig ${CMAKE_SOURCE_DIR}/../ricardian )