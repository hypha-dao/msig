project(multisig)

cmake_minimum_required(VERSION 3.16)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_compile_definitions(USE_LOGGING)

add_contract( multisig multisig multisig.cpp
                ../document-graph/src/document_graph/document_graph.cpp 
                ../document-graph/src/document_graph/document.cpp 
                ../document-graph/src/document_graph/content_wrapper.cpp
                ../document-graph/src/document_graph/content.cpp
                ../document-graph/src/document_graph/edge.cpp
                ../document-graph/src/document_graph/util.cpp 
                ../document-graph/src/logger/logger.cpp 
 )

target_include_directories( multisig PUBLIC ${CMAKE_SOURCE_DIR}/../include ${CMAKE_SOURCE_DIR}/../document-graph/include )
target_ricardian_directory( multisig ${CMAKE_SOURCE_DIR}/../ricardian )