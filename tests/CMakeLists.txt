project(shapereality_test)

add_subdirectory(external/googletest)

add_executable(shapereality_test
        #entity
        entity/hierarchy.cpp
        entity/registry.cpp
        entity/view.cpp
        entity/sparse_set.cpp
        entity/serialize_registry.cpp

        #math
        math/bounds.cpp
        math/matrix.cpp
        math/plane.cpp
        math/quaternion.cpp
        math/ray.cpp
        math/vector.cpp

        #asset
        asset/async.cpp
        asset/thread_pool.cpp
        asset/thread_pool_v2.cpp
        asset/async_2.cpp
        asset/asset_database.cpp
        asset/move_import_result_data.cpp

        #reflection
        reflection/graph_based_reflection_json.cpp
        reflection/enum.cpp
        reflection/polymorphism.cpp
        reflection/enum_info.cpp
        reflection/binary_json_hybrid.cpp
        asset/handles.cpp
        asset/copy_import_result.cpp
        asset/asset_handle_any.cpp
        reflection/unique_any.cpp
        math/initializer.cpp
)

target_link_libraries(shapereality_test thread_pool entity math reflection json gtest gtest_main asset import_gltf import_texture)