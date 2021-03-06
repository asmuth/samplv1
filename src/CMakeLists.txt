set(NAME samplv1)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file (cmake_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set (HEADERS
  samplv1.h
  samplv1_config.h
  samplv1_filter.h
  samplv1_formant.h
  samplv1_resampler.h
  samplv1_sample.h
  samplv1_wave.h
  samplv1_ramp.h
  samplv1_list.h
  samplv1_fx.h
  samplv1_reverb.h
  samplv1_param.h
  samplv1_sched.h
  samplv1_tuning.h
  samplv1_programs.h
  samplv1_controls.h
)

set (SOURCES
  samplv1.cpp
  samplv1_config.cpp
  samplv1_formant.cpp
  samplv1_resampler.cpp
  samplv1_sample.cpp
  samplv1_wave.cpp
  samplv1_param.cpp
  samplv1_sched.cpp
  samplv1_tuning.cpp
  samplv1_programs.cpp
  samplv1_controls.cpp
)

qt5_wrap_cpp (MOC_SOURCES ${HEADERS})


set (HEADERS_UI
  samplv1_ui.h
  samplv1widget.h
  samplv1widget_env.h
  samplv1widget_filt.h
  samplv1widget_sample.h
  samplv1widget_wave.h
  samplv1widget_param.h
  samplv1widget_keybd.h
  samplv1widget_preset.h
  samplv1widget_status.h
  samplv1widget_spinbox.h
  samplv1widget_programs.h
  samplv1widget_controls.h
  samplv1widget_control.h
  samplv1widget_palette.h
  samplv1widget_config.h
)

set (SOURCES_UI
  samplv1_ui.cpp
  samplv1widget.cpp
  samplv1widget_env.cpp
  samplv1widget_filt.cpp
  samplv1widget_sample.cpp
  samplv1widget_wave.cpp
  samplv1widget_param.cpp
  samplv1widget_keybd.cpp
  samplv1widget_preset.cpp
  samplv1widget_status.cpp
  samplv1widget_spinbox.cpp
  samplv1widget_programs.cpp
  samplv1widget_controls.cpp
  samplv1widget_control.cpp
  samplv1widget_palette.cpp
  samplv1widget_config.cpp
)

set (FORMS_UI
  samplv1widget.ui
  samplv1widget_control.ui
  samplv1widget_palette.ui
  samplv1widget_config.ui
)

set (RESOURCES_UI
  samplv1.qrc
)

qt5_wrap_ui (UI_SOURCES_UI ${FORMS_UI})
qt5_wrap_cpp (MOC_SOURCES_UI ${HEADERS_UI})
qt5_add_resources (QRC_SOURCES_UI ${RESOURCES_UI})


set (HEADERS_LV2
  samplv1_lv2.h
  samplv1_lv2ui.h
  samplv1widget_lv2.h
)

set (SOURCES_LV2
  samplv1_lv2.cpp
  samplv1_lv2ui.cpp
  samplv1widget_lv2.cpp
)

qt5_wrap_cpp (MOC_SOURCES_LV2 ${HEADERS_LV2})


set (HEADERS_JACK
  samplv1_nsm.h
  samplv1_jack.h
  samplv1widget_jack.h
)

set (SOURCES_JACK
  samplv1_nsm.cpp
  samplv1_jack.cpp
  samplv1widget_jack.cpp
)

qt5_wrap_cpp (MOC_SOURCES_JACK ${HEADERS_JACK})


add_library (${NAME} STATIC
  ${MOC_SOURCES}
  ${SOURCES}
)

add_library (${NAME}_ui STATIC
  ${UI_SOURCES_UI}
  ${MOC_SOURCES_UI}
  ${QRC_SOURCES_UI}
  ${SOURCES_UI}
)

add_library (${NAME}_lv2 MODULE
  ${MOC_SOURCES_LV2}
  ${SOURCES_LV2}
)

add_executable (${NAME}_jack
  ${MOC_SOURCES_JACK}
  ${SOURCES_JACK}
)


set_target_properties (${NAME}       PROPERTIES CXX_STANDARD 11)
set_target_properties (${NAME}_ui    PROPERTIES CXX_STANDARD 11)
set_target_properties (${NAME}_lv2   PROPERTIES CXX_STANDARD 11)
set_target_properties (${NAME}_jack  PROPERTIES CXX_STANDARD 11)

target_link_libraries (${NAME}       PUBLIC Qt5::Core Qt5::Xml)
target_link_libraries (${NAME}_ui    PUBLIC Qt5::Widgets ${NAME})
target_link_libraries (${NAME}_lv2   PRIVATE ${NAME}_ui)
target_link_libraries (${NAME}_jack  PRIVATE ${NAME}_ui)

if (CONFIG_SNDFILE)
  target_link_libraries (${NAME} PRIVATE ${SNDFILE_LIBRARIES})
endif ()

if (CONFIG_LIBRUBBERBAND)
  target_link_libraries (${NAME} PRIVATE ${RUBBERBAND_LIBRARIES})
endif ()

if (CONFIG_JACK)
  target_link_libraries (${NAME}_jack PRIVATE ${JACK_LIBRARIES})
endif ()

if (CONFIG_ALSA_MIDI)
  target_link_libraries (${NAME}_jack PRIVATE ${ALSA_LIBRARIES})
endif ()

if (CONFIG_LIBLO)
  target_link_libraries (${NAME}_jack PRIVATE ${LIBLO_LIBRARIES})
endif ()


if (UNIX AND NOT APPLE AND NOT CONFIG_DEBUG)
  add_custom_command(TARGET ${NAME}_lv2  POST_BUILD
    COMMAND strip lib${NAME}_lv2.so)
  add_custom_command(TARGET ${NAME}_jack POST_BUILD
    COMMAND strip ${NAME}_jack)
endif ()


if (UNIX AND NOT APPLE)
  if (CONFIG_PREFIX MATCHES $ENV{HOME})
    set (CONFIG_LV2DIR ${CONFIG_PREFIX}/.lv2)
  else ()
    set (CONFIG_LV2DIR ${CONFIG_LIBDIR}/lv2)
  endif ()
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${NAME}_lv2.so
     RENAME ${NAME}.so
     DESTINATION ${CONFIG_LV2DIR}/${NAME}.lv2)
  install (FILES ${NAME}.lv2/manifest.ttl
     ${NAME}.lv2/${NAME}.ttl
     ${NAME}.lv2/${NAME}_ui.ttl
     DESTINATION ${CONFIG_LV2DIR}/${NAME}.lv2)
  install (TARGETS ${NAME}_jack RUNTIME
     DESTINATION ${CMAKE_INSTALL_BINDIR})
  install (FILES ${NAME}.desktop
     DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
  install (FILES images/${NAME}.png
     DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/apps)
  install (FILES images/${NAME}.svg
     DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
  install (FILES appdata/${NAME}.appdata.xml
     DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
  install (FILES mimetypes/${NAME}.xml
     DESTINATION ${CMAKE_INSTALL_DATADIR}/mime/packages)
  install (FILES mimetypes/application-x-${NAME}-preset.png
     DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/mimetypes)
  install (FILES mimetypes/application-x-${NAME}-preset.svg
     DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/mimetypes)
endif ()
