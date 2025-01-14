use cc;

use std::env;

/// Adds the required definitions to build mesa/glsl-optimizer for the
/// target platform.
fn configure(build: &mut cc::Build) -> &mut cc::Build {
    build.define("__STDC_FORMAT_MACROS", None);
    if cfg!(target_os = "linux") {
        build.define("HAVE_ENDIAN_H", None);
    }
    if cfg!(target_os = "windows") {
        build.define("_USE_MATH_DEFINES", None);
    } else {
        build.define("HAVE_PTHREAD", None);
        build.define("HAVE_TIMESPEC_GET", None);
    }

    build
}

fn main() {
    // Unset CFLAGS which are probably intended for a target build,
    // but might break building this as a build dependency if we are
    // not cross-compiling.
    let target = env::var("TARGET").unwrap();
    env::remove_var(format!("CFLAGS_{}", &target));
    env::remove_var(format!("CXXFLAGS_{}", &target));
    env::remove_var(format!("CFLAGS_{}", target.replace("-", "_")));
    env::remove_var(format!("CXXFLAGS_{}", target.replace("-", "_")));

    // On Gonk we set these flags and they end up being used here for
    // host compilation, which doesn't work.
    env::remove_var("CFLAGS");
    env::remove_var("CPPFLAGS");
    env::remove_var("CXXFLAGS");

    // Gecko has set this to override --target= to help windows cross builds,
    // but causes errors building this as a build dependency.
    env::remove_var("BINDGEN_EXTRA_CLANG_ARGS");

    configure(&mut cc::Build::new())
        .warnings(false)
        .include("glsl-optimizer/include")
        .include("glsl-optimizer/src/mesa")
        .include("glsl-optimizer/src/mapi")
        .include("glsl-optimizer/src/compiler")
        .include("glsl-optimizer/src/compiler/glsl")
        .include("glsl-optimizer/src/gallium/auxiliary")
        .include("glsl-optimizer/src/gallium/include")
        .include("glsl-optimizer/src")
        .include("glsl-optimizer/src/util")
        .file("glsl-optimizer/src/compiler/glsl/glcpp/glcpp-lex.c")
        .file("glsl-optimizer/src/compiler/glsl/glcpp/glcpp-parse.c")
        .file("glsl-optimizer/src/compiler/glsl/glcpp/pp_standalone_scaffolding.c")
        .file("glsl-optimizer/src/compiler/glsl/glcpp/pp.c")
        .file("glsl-optimizer/src/util/blob.c")
        .file("glsl-optimizer/src/util/half_float.c")
        .file("glsl-optimizer/src/util/hash_table.c")
        .file("glsl-optimizer/src/util/mesa-sha1.c")
        .file("glsl-optimizer/src/util/ralloc.c")
        .file("glsl-optimizer/src/util/set.c")
        .file("glsl-optimizer/src/util/sha1/sha1.c")
        .file("glsl-optimizer/src/util/softfloat.c")
        .file("glsl-optimizer/src/util/string_buffer.c")
        .file("glsl-optimizer/src/util/strtod.c")
        .compile("glcpp");

    configure(&mut cc::Build::new())
        .warnings(false)
        .include("glsl-optimizer/include")
        .include("glsl-optimizer/src/mesa")
        .include("glsl-optimizer/src/mapi")
        .include("glsl-optimizer/src/compiler")
        .include("glsl-optimizer/src/compiler/glsl")
        .include("glsl-optimizer/src/gallium/auxiliary")
        .include("glsl-optimizer/src/gallium/include")
        .include("glsl-optimizer/src")
        .include("glsl-optimizer/src/util")
        .file("glsl-optimizer/src/mesa/program/dummy_errors.c")
        .file("glsl-optimizer/src/mesa/program/symbol_table.c")
        .file("glsl-optimizer/src/mesa/main/extensions_table.c")
        .file("glsl-optimizer/src/mesa/main/imports.c")
        .file("glsl-optimizer/src/compiler/shader_enums.c")
        .compile("mesa");

    configure(&mut cc::Build::new())
        .cpp(true)
        .warnings(false)
        .include("glsl-optimizer/include")
        .include("glsl-optimizer/src/mesa")
        .include("glsl-optimizer/src/mapi")
        .include("glsl-optimizer/src/compiler")
        .include("glsl-optimizer/src/compiler/glsl")
        .include("glsl-optimizer/src/gallium/auxiliary")
        .include("glsl-optimizer/src/gallium/include")
        .include("glsl-optimizer/src")
        .include("glsl-optimizer/src/util")
        .file("glsl-optimizer/src/compiler/glsl_types.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ast_array_index.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ast_expr.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ast_function.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ast_to_hir.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ast_type.cpp")
        .file("glsl-optimizer/src/compiler/glsl/builtin_functions.cpp")
        .file("glsl-optimizer/src/compiler/glsl/builtin_types.cpp")
        .file("glsl-optimizer/src/compiler/glsl/builtin_variables.cpp")
        .file("glsl-optimizer/src/compiler/glsl/generate_ir.cpp")
        .file("glsl-optimizer/src/compiler/glsl/glsl_lexer.cpp")
        .file("glsl-optimizer/src/compiler/glsl/glsl_optimizer.cpp")
        .file("glsl-optimizer/src/compiler/glsl/glsl_parser_extras.cpp")
        .file("glsl-optimizer/src/compiler/glsl/glsl_parser.cpp")
        .file("glsl-optimizer/src/compiler/glsl/glsl_symbol_table.cpp")
        .file("glsl-optimizer/src/compiler/glsl/hir_field_selection.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_array_refcount.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_basic_block.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_builder.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_clone.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_constant_expression.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_equals.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_expression_flattening.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_function_can_inline.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_function_detect_recursion.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_function.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_hierarchical_visitor.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_hv_accept.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_print_glsl_visitor.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_print_visitor.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_reader.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_rvalue_visitor.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_set_program_inouts.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_unused_structs.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_validate.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir_variable_refcount.cpp")
        .file("glsl-optimizer/src/compiler/glsl/ir.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_atomics.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_functions.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_interface_blocks.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_uniform_block_active_visitor.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_uniform_blocks.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_uniform_initializers.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_uniforms.cpp")
        .file("glsl-optimizer/src/compiler/glsl/link_varyings.cpp")
        .file("glsl-optimizer/src/compiler/glsl/linker_util.cpp")
        .file("glsl-optimizer/src/compiler/glsl/linker.cpp")
        .file("glsl-optimizer/src/compiler/glsl/loop_analysis.cpp")
        .file("glsl-optimizer/src/compiler/glsl/loop_unroll.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_blend_equation_advanced.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_buffer_access.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_const_arrays_to_uniforms.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_cs_derived.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_discard_flow.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_discard.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_distance.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_if_to_cond_assign.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_instructions.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_int64.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_jumps.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_mat_op_to_vec.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_named_interface_blocks.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_noise.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_offset_array.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_output_reads.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_packed_varyings.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_packing_builtins.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_shared_reference.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_subroutine.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_tess_level.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_texture_projection.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_ubo_reference.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_variable_index_to_cond_assign.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_vec_index_to_cond_assign.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_vec_index_to_swizzle.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_vector_derefs.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_vector_insert.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_vector.cpp")
        .file("glsl-optimizer/src/compiler/glsl/lower_vertex_id.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_algebraic.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_array_splitting.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_conditional_discard.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_constant_folding.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_constant_propagation.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_constant_variable.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_copy_propagation_elements.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_dead_builtin_variables.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_dead_builtin_varyings.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_dead_code_local.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_dead_code.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_dead_functions.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_flatten_nested_if_blocks.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_flip_matrices.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_function_inlining.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_if_simplification.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_minmax.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_rebalance_tree.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_redundant_jumps.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_structure_splitting.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_swizzle.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_tree_grafting.cpp")
        .file("glsl-optimizer/src/compiler/glsl/opt_vectorize.cpp")
        .file("glsl-optimizer/src/compiler/glsl/propagate_invariance.cpp")
        .file("glsl-optimizer/src/compiler/glsl/s_expression.cpp")
        .file("glsl-optimizer/src/compiler/glsl/serialize.cpp")
        .file("glsl-optimizer/src/compiler/glsl/shader_cache.cpp")
        .file("glsl-optimizer/src/compiler/glsl/standalone_scaffolding.cpp")
        .file("glsl-optimizer/src/compiler/glsl/string_to_uint_map.cpp")
        .compile("glsl_optimizer");
}
