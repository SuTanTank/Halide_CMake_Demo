set_property(GLOBAL PROPERTY USE_FOLDERS ON)
function(get_all_targets _result _dir)
	get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)
	foreach(_subdir IN LISTS _subdirs)
		get_all_targets(${_result} "${_subdir}")
	endforeach()
	get_property(_sub_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
	set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
endfunction()

function(add_subdirectory_with_folder _folder_name _folder)
	add_subdirectory(${_folder} ${ARGN})
	get_all_targets(_targets "${_folder}")
	foreach(_target IN LISTS _targets)
		get_target_property(target_type ${_target} TYPE)
		if (NOT target_type STREQUAL INTERFACE_LIBRARY)
			get_target_property(folder_name ${_target} FOLDER)
			if (folder_name STREQUAL folder_name-NOTFOUND)
					set(folder_name "")
			endif()
			set_target_properties(
					${_target}
					PROPERTIES FOLDER "${_folder_name}${folder_name}"
			)
		endif()
	endforeach()
endfunction()

function(move_all_target_to_folder _folder_name)
	get_all_targets(_targets ".")
	foreach(_target IN LISTS _targets)
		get_target_property(target_type ${_target} TYPE)
		if (NOT target_type STREQUAL INTERFACE_LIBRARY)
			get_target_property(folder_name ${_target} FOLDER)
			if (folder_name STREQUAL folder_name-NOTFOUND)
					set(folder_name "")
			endif()
			set_target_properties(
					${_target}
					PROPERTIES FOLDER "${_folder_name}${folder_name}"
			)
		endif()
	endforeach()
endfunction()
