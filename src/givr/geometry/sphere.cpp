#include "sphere.h"

#include <cassert>
#include <cmath>

using Sphere = givr::Sphere;

Sphere::Data givr::generateGeometry(Sphere const &s) {
    Sphere::Data data;

    // TODO: should make this reserve the space necessary for the sphere.

    struct point { float v[3]; };
    //Azimuth: 0 to 2pi, Altitude: 0 to pi
    auto sphereFunc = [](float azi, float alt) {
        return point{ std::cos(azi)*std::sin(alt), std::cos(alt), std::sin(azi)*std::sin(alt) };
    };

    for (size_t azi = 0; azi < s.azimuthPoints; azi++) {
        for (size_t alt = 0; alt < s.altitudePoints; alt++) {
            if (azi < s.azimuthPoints - 1 && alt < s.altitudePoints - 1) {
                data.indices.push_back(alt + azi * s.altitudePoints);
                data.indices.push_back((alt + 1) + azi * s.altitudePoints);
                data.indices.push_back((alt + 1) + (azi + 1)* s.altitudePoints);

                data.indices.push_back(alt + azi * s.altitudePoints);
                data.indices.push_back((alt + 1) + (azi + 1)*s.altitudePoints);
                data.indices.push_back(alt + (azi + 1)*s.altitudePoints);
            }
            else if(alt < s.altitudePoints - 1){
                data.indices.push_back(alt + azi * s.altitudePoints);
                data.indices.push_back((alt + 1) + azi * s.altitudePoints);
                data.indices.push_back((alt + 1));

                data.indices.push_back(alt + azi * s.altitudePoints);
                data.indices.push_back((alt + 1));
                data.indices.push_back(alt);
            }
            float u = float(azi) / float(s.azimuthPoints - 1);
            float v = float(alt) / float(s.altitudePoints - 1);
            //Make uniform distribution
            v = acos(1 - 2.f*v) / M_PI;     //DELETE THIS IF SPHERES LOOK BAD

            point spherePoint = sphereFunc(2.f*M_PI*u, M_PI*v);
            data.vertices.push_back(spherePoint.v[0]);
            data.vertices.push_back(spherePoint.v[1]);
            data.vertices.push_back(spherePoint.v[2]);

            data.normals.push_back(spherePoint.v[0]);
            data.normals.push_back(spherePoint.v[1]);
            data.normals.push_back(spherePoint.v[2]);

            data.uvs.push_back(u);
            data.uvs.push_back(v);
        }
    }

    assert(data.vertices.size() == data.normals.size());

    return data;
}

