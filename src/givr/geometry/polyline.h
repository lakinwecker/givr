#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"

namespace givr {
namespace geometry {

    template <PrimitiveType LineType>
    struct PolyLineGeometry {
        static_assert(
            LineType != PrimitiveType::LINE_LOOP ||
            LineType != PrimitiveType::LINE_STRIP,
            "PolyLine only supports LINE_LOOP or LINE_STRIP"
        );
        private:
            std::vector<Point> m_points;
        public:
            std::vector<Point> &points() { return m_points; }
            std::vector<Point> const &points() const { return m_points; }
            std::vector<Point> &operator*() { return m_points; }
            // TODO: expose more of the vector interface here, or
            //       the conversion operator or something
            void push_back(Point const &p) { m_points.push_back(p); }
            void clear() { m_points.clear(); }

        struct Data : public VertextArrayData<LineType> {
            std::uint16_t dimensions = 3;

            BufferUsageType verticesType;
            std::vector<float> vertices;
        };
    };

    template <PrimitiveType LineType, typename... Args>
    PolyLineGeometry<LineType> PolyLine(Args &&... args) {
        PolyLineGeometry<LineType> geometry;
        geometry.points() = {args...};
        return geometry;
    }

    template <PrimitiveType LineType>
    typename PolyLineGeometry<LineType>::Data generateGeometry(PolyLineGeometry<LineType> const &l) {
        typename PolyLineGeometry<LineType>::Data data;
        auto const &points = l.points();
        data.vertices.reserve(points.size()*3);
        for (std::size_t i = 0; i < points.size(); ++i) {
            data.vertices.push_back(points[i].value()[0]);
            data.vertices.push_back(points[i].value()[1]);
            data.vertices.push_back(points[i].value()[2]);
        }
        return data;
    }
}// end namespace geometry
}// end namespace givr
