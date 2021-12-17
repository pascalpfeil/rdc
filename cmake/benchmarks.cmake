include(benchmark)

file(GLOB BENCHMARK_CC "${PROJECT_SOURCE_DIR}/bench/*_benchmark.cc")
foreach(benchfile ${BENCHMARK_CC})
    get_filename_component(benchname ${benchfile} NAME_WE)
    string(REPLACE ".cc" "" benchname ${benchname} )
    add_executable(${benchname} ${benchfile} )
    target_link_libraries(
        ${benchname}
            benchmark
            rdc
    )
endforeach(benchfile ${BENCHMARK_CC})