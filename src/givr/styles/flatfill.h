#pragma once

#include "../buffer_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {

    struct flatfill_params {
        vec3f colour;
    };

    struct flatfill_instanced_render_context
        : public instanced_render_context,
          public flatfill_params
    {
        void set_uniforms(std::unique_ptr<program> const &p) const;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;
    };

    struct flatfill_render_context
        : public render_context,
          public flatfill_params
    {
        void set_uniforms(std::unique_ptr<program> const &p) const;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;
    };

    struct flatfill : public flatfill_params {
        using instanced_render_context = flatfill_instanced_render_context;
        using render_context = flatfill_render_context;
    };


    template <typename GeometryT>
    buffer_data fill_buffers(GeometryT const &g, flatfill const &) {
        buffer_data data;
        typename GeometryT::data d = generate_geometry(g);
        static_assert(has_vertices<GeometryT>::value, "The flatfill style requires vertices. The geometry you are using does not provide them.");
        static_assert(has_indices<GeometryT>::value, "The flatfill style requires indices. The geometry you are using does not provide them.");
        data.add_vertices(d.vertices);
        data.add_indices(d.indices);
        return std::move(data);
    }

    template <typename RenderContextT, typename GeometryT>
    RenderContextT get_context(GeometryT &, flatfill const &f) {
        auto ctx = RenderContextT{};
        ctx.shader_program = std::make_unique<program>(
            shader{ctx.get_vertex_shader_source(), GL_VERTEX_SHADER},
            shader{ctx.get_fragment_shader_source(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = primitive_type::TRIANGLES;
        ctx.colour = f.colour;
        return ctx;
    }

    template <typename GeometryT>
    flatfill::instanced_render_context
    get_instanced_context(GeometryT &g, flatfill const &f) {
        return get_context<flatfill::instanced_render_context, GeometryT>(g, f);
    }

    template <typename GeometryT>
    flatfill::render_context
    get_context(GeometryT &g, flatfill const &f) {
        return get_context<flatfill::render_context, GeometryT>(g, f);
    }

    template <typename ViewContextT>
    void draw(flatfill::instanced_render_context &ctx, ViewContextT const &view_ctx) {
        draw_instanced(ctx, view_ctx, [&ctx](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(flatfill::render_context &ctx, ViewContextT const &view_ctx, mat4f model=mat4f(1.f)) {
        draw_array(ctx, view_ctx, [&ctx, &model](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
            program->set_mat4("model", model);
        });
    }
};// end namespace givr
