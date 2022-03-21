#define _USE_MATH_DEFINES
#include "BlinnPhongMaterial.h"
#include "RandomEngine.h"
#include "Utils.h"
#include <cmath>
ZLib::Vec3f BlinnPhongMaterial::sample(const ZLib::Vec3f& wi, const ZLib::Vec3f& normal, float& pdf, bool& is_refract, int engine_id) const
{
    auto& engine = RandomEngineManager::getInstance().getEngine(engine_id);
    float p;
    pdf = 1;
    if (!ks_map && ks == Vec3f()) {
        p = 1;
    } else {
        if (ns <= 1) {
            p = 1;
        } else {
            
            p = 1.0f / (std::log10(ns) + 1.0f);
            //p = 1;
        }
       
    }
    float refract_p = .0f;
    is_refract = false;
    if (canRefract()) {
        refract_p = 1.0f;
        Vec3f tmp_n = normal;
        float inv_ni = 1.0f / ni;
        if (normal.dot(wi) < 0) {
            tmp_n = -normal;
            inv_ni = ni;
            refract_p = 1.0f;
        }
        if (engine.sampleUniform() <= refract_p) {
            pdf *= refract_p;
            float nl = wi.dot(tmp_n);
            float tmp = 1 - inv_ni * inv_ni * (1 - nl * nl);
            Vec3f wo;
            if (tmp > 0) {
                wo = tmp_n * (inv_ni * nl - std::sqrt(tmp)) - wi * inv_ni;
            } else {
                wo = tmp_n * (2 * ni) - wi;
            }
            
            is_refract = true;
            return wo;
        }
    }
    // diffuse²ÉÑù
    if (engine.sampleUniform() < p) {
        //float cos_theta = std::abs(1 - 2 * engine.sampleUniform(0, 1));
        //float cos_theta = std::cos(std::asin(engine.sampleUniform()));
        float cos_theta = std::sqrt(1 - engine.sampleUniform());
        float sin_theta = std::sqrt(1 - cos_theta * cos_theta);
        float phi = 2 * M_PI * engine.sampleUniform(0, 1);
        ZLib::Vec3f local(sin_theta * std::cos(phi), sin_theta * std::sin(phi), cos_theta);
        auto wo = toWorld(local, normal);
        //pdf = 1 / (2 * M_PI);
        pdf = p * cos_theta / M_PI;
        if (ns > 1) {
            auto half = wi + wo;
            half.normalize();
            auto tmp_cos1 = half.dot(normal);
            auto tmp_cos2 = half.dot(wi);
            pdf += (1 - p) * (ns + 2) / (2 * M_PI) * std::pow(tmp_cos1, ns + 1) / (4 * tmp_cos2);
        }
        pdf *= (1 - refract_p);
        pdf = std::max(0.f, pdf);
        return wo;
    } else {
        float cos_theta = std::pow(1 - engine.sampleUniform(), 1.0f / (ns + 2.0f));
        float sin_theta = std::sqrt(1 - cos_theta * cos_theta);
        float phi = 2 * M_PI * engine.sampleUniform(0, 1);
        ZLib::Vec3f local(sin_theta * std::cos(phi), sin_theta * std::sin(phi), cos_theta);
        local = toWorld(local, normal);
        local.normalize();
        float tmp_cos = local.dot(wi);
        auto wo = local * 2 * tmp_cos - wi;
        //auto h = wo + wi;
        //h.normalize();
        //std::cout << h - local << std::endl;
        pdf = (1 - p) * (ns + 2) / (2 * M_PI) * std::pow(cos_theta, ns + 1);
        pdf /= (4 * tmp_cos);
        if (normal.dot(wo) <= 0) {
            pdf = 0;
        } else {
            pdf += p * cos_theta / M_PI;
        }
        pdf *= (1 - refract_p);
        return wo;
    }

}


BlinnPhongMaterial::BlinnPhongMaterial():kd_map(nullptr),ks_map(nullptr),ka_map(nullptr),ni(1),ns(1),emission()
{

}

bool BlinnPhongMaterial::canRefract() const
{
    //return false;
    return ni > 1;
}

ZLib::Vec3f BlinnPhongMaterial::brdf(const ZLib::Vec3f & wi, const ZLib::Vec3f & wo, const ZLib::Vec2f & uv, const ZLib::Vec3f & normal) const
{
    Vec3f h = (wo + wi);
    h.normalize();
    float cos_nh = std::max(0.0f, normal.dot(h));
    //Vec3f r = normal * 2 * normal.dot(wi) - wi;
    //float cos_nh = r.dot(wo);
    Vec3f diffuse = kd;
    Vec3f specular = ks;
    Vec3f ambient = ka;
    if (ka_map) {
        ambient = ka_map->sample(uv.x(), uv.y());
    }
    if (kd_map) {
        diffuse = ZLib::pow(kd_map->sample(uv.x(), uv.y()), 2.2f);
        //diffuse = kd_map->sample(uv.x(), uv.y());
    }
    if (ks_map) {
        specular = ks_map->sample(uv.x(), uv.y());
    }
    auto tmp = specular + diffuse;
    if (tmp[0] > 1 || tmp[1] > 1 || tmp[2] > 1) {
        //std::cout << "ddd";
        specular = tmp - diffuse;
    }
    specular *= std::pow(cos_nh, ns);

    return  (diffuse / M_PI) + specular * ((ns + 2.0f) * (ns + 4.0f) / (8.0f * M_PI * (ns + std::pow(2.0, -ns / 2.0))));
    //return  (diffuse / M_PI) + specular * (ns + 2) / (2 * M_PI);
}


bool BlinnPhongMaterial::hasEmission() const
{
    return emission != ZLib::Vec3f(0, 0, 0);
}

const std::string& BlinnPhongMaterial::name() const
{
    return _name;
}

ZLib::Vec3f BlinnPhongMaterial::emit() const
{
    return emission;
}


