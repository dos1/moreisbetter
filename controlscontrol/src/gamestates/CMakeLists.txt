MACRO(GAMESTATE name)

	add_library("libsuperderpy-controlscontrol-${name}" SHARED "${name}.c")

	SET_TARGET_PROPERTIES("libsuperderpy-controlscontrol-${name}" PROPERTIES PREFIX "")

	target_link_libraries("libsuperderpy-controlscontrol-${name}" ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_FONT_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES} ${ALLEGRO5_AUDIO_LIBRARIES} ${ALLEGRO5_ACODEC_LIBRARIES} ${ALLEGRO5_IMAGE_LIBRARIES} m libsuperderpy-controlscontrol)

	install(TARGETS "libsuperderpy-controlscontrol-${name}" DESTINATION ${LIB_INSTALL_DIR})

ENDMACRO()

GAMESTATE("dosowisko")
GAMESTATE("menu")

GAMESTATE("loading")
