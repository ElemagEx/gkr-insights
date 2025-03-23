add_subdirectory(gkr-mediator)

#add_executable(gkr_main "main.cpp" "minimal-ws-broker.c")

#if(requirements)
#    #target_include_directories(gkr_main
#    target_link_libraries(gkr_main gkr_core gkr_log gkr_mediator)
#    target_link_libraries(gkr_main)#(websockets_shared)
#    #add_dependencies(gkr_main websockets_shared)
#    #target_link_libraries(gkr_main libweb
#
#    #if (websockets_shared)
#        #target_link_libraries(gkr_main websockets_shared ${LIBWEBSOCKETS_DEP_LIBS})
#    #else()
#        #target_link_libraries(gkr_main websockets) #${LIBWEBSOCKETS_DEP_LIBS}
#    #endif()
#else()
#    message(FATAL_ERROR "Requirements are not met")
#endif()

#add_executable(gkr_main "main.cpp" "minimal-ws-broker.c")
#
#if(requirements)
#    #target_include_directories(gkr_main
#    target_link_libraries(gkr_main gkr_core gkr_log gkr_mediator)
#    target_link_libraries(gkr_main)#(websockets_shared)
#    #add_dependencies(gkr_main websockets_shared)
#    #target_link_libraries(gkr_main libweb
#
#    #if (websockets_shared)
#        #target_link_libraries(gkr_main websockets_shared ${LIBWEBSOCKETS_DEP_LIBS})
#    #else()
#        #target_link_libraries(gkr_main websockets) #${LIBWEBSOCKETS_DEP_LIBS}
#    #endif()
#else()
#    message(FATAL_ERROR "Requirements are not met")
#endif()
