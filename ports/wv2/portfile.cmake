vcpkg_from_github(
	OUT_SOURCE_PATH SOURCE_PATH
	REPO docwire/wv2
	REF 4b81231eb5c72f4bfcb39634633473bd1ea78fdf
	SHA512 40b244ac6156710370dd0cc2fec2d011377ce1cd118d87d0a7d5cd76dea2f081039a96009b401952a75631ee77e5f9a7419cd5a047aaa08f6c895291ba41c5ef
	HEAD_REF master
)

vcpkg_list(SET options)

if(VCPKG_TARGET_IS_OSX)
	vcpkg_list(APPEND options "-DCMAKE_CXX_STANDARD=17")
endif()

vcpkg_cmake_configure(
	SOURCE_PATH "${SOURCE_PATH}"
	OPTIONS ${options}
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(CONFIG_PATH lib/wvWare)

vcpkg_download_distfile(PRIVATE_HEADERS_ARCHIVE
	URLS "http://silvercoders.com/download/3rdparty/wv2-0.2.3_patched_4-private_headers.tar.bz2"
	FILENAME "wv2-0.2.3_patched_4-private_headers.tar.bz2"
	SHA512 f45ec5e13dd681a20ad458931d474a75e9b2cba967c1614f63a6180124c2a2b5a3e61f3121d0d47a6d96505f00702d057436a410e42b752d3afaf2dbae1c14f6
)

vcpkg_extract_source_archive_ex(
	OUT_SOURCE_PATH PRIVATE_HEADERS_SOURCE_PATH
	ARCHIVE ${PRIVATE_HEADERS_ARCHIVE}
)

file(INSTALL ${PRIVATE_HEADERS_SOURCE_PATH}/ DESTINATION ${CURRENT_PACKAGES_DIR}/include/${PORT} FILES_MATCHING PATTERN "*.h")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/COPYING.LIB" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

if(VCPKG_LIBRARY_LINKAGE STREQUAL "static")
	file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/bin" "${CURRENT_PACKAGES_DIR}/debug/bin")
endif()
