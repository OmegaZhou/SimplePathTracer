#define TINYOBJLOADER_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include "Scene.h"
#include "../Lib\tiny_obj_loader.h"
#include "../Lib/tinyxml2.h"
#include "../ImageTool/ImageBuffer.hpp"
#include "BlinnPhongMaterial.h"
#include "BaseTexture.h"
#include "ResourceManager.h"
#include "Triangle.h"
#include "RandomEngine.h"
#include <omp.h>
#include <cstdio>
#include <cmath>
static void updateProcess(float t)
{
    char str[512] = { 0 };
    char* buffer = str;
    int val;
    sprintf(buffer, "\r[");
    buffer += 2;
    int k = std::round(20 * t);
    int j = 0;
    for (j = 0; j < k;++j) {
        sprintf(buffer, "*");
        buffer++;
    }
    for (int i = j; i < 20; ++i) {
        sprintf(buffer, " ");
        buffer++;
    }
    sprintf(buffer, "]");
    buffer++;
    sprintf(buffer, "%.2f%%", 100.0f * t);
    printf("%s", str);
}
static ZLib::Vec3f parseVec3f(const char* str)
{
    ZLib::Vec3f vec;
    assert(str);
    std::sscanf(str, "%f, %f, %f", &vec.x(), &vec.y(), &vec.z());
    return vec;
}
static std::map<std::string, ZLib::Vec3f> getLightInfos(tinyxml2::XMLDocument& doc)
{
    std::map<std::string, ZLib::Vec3f> ret;
    for (auto node = doc.FirstChildElement("light"); node != nullptr; node = node->NextSiblingElement("light")) {
        std::string material_name = node->FindAttribute("mtlname")->Value();
        auto radiance_str = node->FindAttribute("radiance")->Value();
        ret[material_name] = parseVec3f(radiance_str);
    }
    return ret;
}

static Camera getCamera(tinyxml2::XMLDocument& doc)
{
    Camera ret;
    auto node = doc.FirstChildElement("camera");
    ret.position = parseVec3f(node->FirstChildElement("eye")->FindAttribute("value")->Value());
    ret.lookat = parseVec3f(node->FirstChildElement("lookat")->FindAttribute("value")->Value());
    ret.up = parseVec3f(node->FirstChildElement("up")->FindAttribute("value")->Value());
    ret.fovy = node->FirstChildElement("fovy")->FindAttribute("value")->FloatValue();
    ret.width = node->FirstChildElement("width")->FindAttribute("value")->IntValue();
    ret.height = node->FirstChildElement("height")->FindAttribute("value")->IntValue();
    std::string type = node->FindAttribute("type")->Value();
    if (type == "perspective") {
        ret.type = Camera::Type::Perspective;
    }
    return ret;
}

ITexture* Scene::getTexture(const std::string& name)
{
    if (name.empty()) {
        return nullptr;
    } else {
        auto iter = textures.find(name);
        if (iter == textures.end()) {
            BaseTexture* texture = ResourceManager::getInstance().createTexture<BaseTexture>();
            texture->load(base_dir + "\\" + name);
            textures[name] = texture;
            return texture;
        } else {
            return iter->second;
        }
    }
    return nullptr;
}
IMaterial* Scene::getMaterial(const std::string& name)
{
    if (name.empty()) {
        return nullptr;
    } else {
        auto iter = materials.find(name);
        if (iter == materials.end()) {
            return nullptr;
        } else {
            return iter->second;
        }
    }
    return nullptr;
}
void Scene::loadObj(const std::string& scene_name, const std::string& base_dir)
{
    this->base_dir = base_dir;
    tinyxml2::XMLDocument doc;
    doc.LoadFile((this->base_dir + "\\" + scene_name + ".xml").c_str());
    auto light_map = getLightInfos(doc);
    camera = getCamera(doc);
    tinyobj::attrib_t _attrib;
    std::vector<tinyobj::shape_t> _shapes;
    std::vector<tinyobj::material_t> _materials;
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&_attrib, &_shapes, &_materials, &warn, &err, (base_dir+"\\"+scene_name+".obj").c_str(), base_dir.c_str());
    for (int i = 0; i < _materials.size(); ++i) {
        BlinnPhongMaterial* material = ResourceManager::getInstance().createMaterial<BlinnPhongMaterial>();
        for (int j = 0; j < 3; ++j) {
            material->ka[j] = _materials[i].ambient[j];
            material->ks[j] = _materials[i].specular[j];
            material->kd[j] = _materials[i].diffuse[j];
            material->emission[j] = _materials[i].emission[j];
        }
        material->_name = _materials[i].name;
        material->ns = _materials[i].shininess;
        material->ni = _materials[i].ior;
        material->ka_map = getTexture(_materials[i].ambient_texname);
        material->ks_map = getTexture(_materials[i].specular_texname);
        material->kd_map = getTexture(_materials[i].diffuse_texname);
        auto iter = light_map.find(_materials[i].name);
        if (iter != light_map.end()) {
            material->emission = iter->second;
        }
        this->materials[_materials[i].name] = material;
        
    }
    //std::map<std::string, FILE*> files;
    //std::map<std::string, int> offsets;
    //for (auto& m : materials) {
    //    files[m.second->name()] = fopen((m.second->name() + ".obj").c_str(), "wb");
    //    offsets[m.second->name()] = 1;
    //}
    for (size_t s = 0; s < _shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < _shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(_shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            assert(fv == 3);
            Triangle* tri = ResourceManager::getInstance().createShape<Triangle>();
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = _shapes[s].mesh.indices[index_offset + v];

                tinyobj::real_t vx = _attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = _attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = _attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                tri->v[v] = ZLib::Vec3f(vx, vy, vz);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = _attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = _attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = _attrib.normals[3 * size_t(idx.normal_index) + 2];
                    tri->normal[v] = ZLib::Vec3f(nx, ny, nz);
                    tri->normal[v].normalize();
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = _attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = _attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    tri->uv[v] = ZLib::Vec2f(tx, ty);
                }
                
                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;

            // per-face material
            int material_id = _shapes[s].mesh.material_ids[f];
            tri->material = getMaterial(_materials[material_id].name);
            tri->initAABB();
            shapes.push_back(tri);
            

            //auto file = files[tri->material->name()];
            //auto& offset = offsets[tri->material->name()];

            //for (int i = 0; i < 3; ++i) {
            //    auto& v = tri->v[i];
            //    fprintf(file, "v %lf %lf %lf\n", v[0], v[1], v[2]);
            //}
            //fprintf(file, "f %d %d %d\n", offset, offset + 1, offset + 2);
            //offset += 3;
                
            
            
            
            if (tri->material->hasEmission()) {
                lights.push_back(tri);
            }
        }
    }
   // for (auto file : files) {
   //     fclose(file.second);
   //}
}

void Scene::render(int spp, const std::string& path)
{
    ZLib::ImageBuffer image(camera.width, camera.height);
    ZLib::Matrix4f lookat_matriax = ZLib::lookat(camera.position, camera.lookat, camera.up);
    ZLib::Matrix4f inv_lookat = ZLib::Matrix4f::identity();
    using ZLib::Vec3f;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            inv_lookat[i][j] = lookat_matriax[j][i];
        }
    }
    float angle = camera.fovy / 360.0f * M_PI;
    float dis = camera.height / 2.0f / tan(angle);
    float half_w = camera.width / 2.0f;
    float half_h = camera.height / 2.0f;
    
    int proc_num = omp_get_num_procs();
    RandomEngineManager::getInstance().createEngines(proc_num);
    bvh.build(shapes, proc_num);
    int t = 0;
#pragma omp parallel num_threads(proc_num)
    {
        const auto thread_id = omp_get_thread_num();
#pragma omp for 
        for (int x = 0; x < camera.width; ++x) {
            for (int y = 0; y < camera.height; ++y) {
                auto& engine = RandomEngineManager::getInstance().getEngine(thread_id);
                
                Vec3f result;
                for (int i = 0; i < spp; ++i) {
                    Vec3f ray_dir(x + engine.sampleUniform(-0.5f, 0.5f) - half_w, y + engine.sampleUniform(-0.5f, 0.5f) - half_h, -dis);
                    ray_dir.normalize();
                    Ray ray(camera.position, inv_lookat * ray_dir);
                    //auto tmp = castRay(ray, thread_id);
                    //ZLib::clamp(0.f, 1.f, tmp);
                    //result += tmp;
                    //if(x==361&&y==629)
                    result += castRay(ray, thread_id);
                    if (std::isnan(result[0]) || std::isnan(result[1]) || std::isnan(result[2])) {
                        printf("%d %d\n", x, y);
                    }
                    //result += castRay2(ray, thread_id);
                    //auto a = castRay2(ray, thread_id);
                    //auto b = castRay(ray, thread_id);
                    //result += a;
                }
                result /= spp;
                ZLib::clamp(0.f, 1.f, result);
                result = ZLib::pow(result, 1 / 2.2f);
                ZLib::Color color;
                color.r = result[0] * 255;
                color.g = result[1] * 255;
                color.b = result[2] * 255;
                image.setColor(x, y, color);
            }
            updateProcess(1.0f * t / camera.width);
            ++t;
        }
    }
    updateProcess(1);
    printf("\n");
    image.saveImage(path);
}

Intersection Scene::find(const Ray& ray, int thread_id)
{
    //Intersection intersection;
    //intersection.t = INFINITY;
    //for (auto& shape : shapes) {
    //    Intersection tmp;
    //    if (shape->intersect(ray, tmp)) {
    //        if (tmp.t < intersection.t ) {
    //            intersection = tmp;
    //        }
    //    }
    //}
    //return intersection;
    return bvh.find(0, ray, thread_id);
}

Intersection Scene::sampleLight(float& pdf, int engine_id)
{
    Intersection ret;
    auto& engine = RandomEngineManager::getInstance().getEngine(engine_id);
    float total_area = 0;
    for (auto& light : lights) {
        total_area += light->area();
    }
    float p = total_area * engine.sampleUniform();
    float tmp_area = 0;
    for (auto& light : lights) {
        tmp_area += light->area();
        if (p <= tmp_area) {
            light->sample(ret, pdf, engine_id);
            pdf *= light->area() / total_area;
            break;
        }
    }
    return ret;
}

Intersection Scene::sampleLight2(float& pdf, int engine_id)
{
    float total_area = 0;
    Intersection ret;
    auto& engine = RandomEngineManager::getInstance().getEngine(engine_id);
    std::set<std::string> material_names;
    for (auto& light : lights) {
        material_names.insert(light->material->name());
    }
    int k = std::floor(material_names.size() * engine.sampleUniform());
    int c = 0;
    std::string name;
    for (auto& _name : material_names) {
        if (c == k) {
            name = _name;
            break;
        }
        ++c;
    }
    float tmp_area = 0;
    for (auto& light : lights) {
        if (light->material->name() == name) {
            total_area += light->area();
        }
    }
    float p = total_area * engine.sampleUniform();
    for (auto& light : lights) {
        if (light->material->name() == name) {
            tmp_area += light->area();
            if (p <= tmp_area) {
                light->sample(ret, pdf, engine_id);
                pdf *= light->area() / total_area / material_names.size();
                break;
            }
        }
        
    }
    return ret;
}

ZLib::Vec3f Scene::castRay(const Ray& ray, int thread_id, int depth)
{
    using ZLib::Vec3f;
    using std::cout;
    using std::endl;
    float roulette_coefficient = 0.7;
    Intersection intersection = find(ray, thread_id);

    if (isinf(intersection.t)) {
        return ZLib::Vec3f(0, 0, 0);
    }
    //return Vec3f(0, 0, 1);
    //return ZLib::Vec3f(0, 0, intersection.point.z() * 0.5 + 1);
    ZLib::Vec3f l0;
    if (intersection.material->hasEmission() && depth == 0) {
        l0 = intersection.material->emit();
    }
    //if (intersection.material->canRefract()) {
    //    float pdf;
    //    bool tmp;
    //    auto wo = intersection.material->sample(-ray.dir, intersection.normal, pdf, tmp, thread_id);

    //    return castRay(Ray(intersection.point, wo), thread_id) * 0.9f / pdf;
    //}
    if (!intersection.material->canRefract()) {
        //intersection.normal = intersection.normal.dot(-ray.dir) >= 0 ? intersection.normal : -intersection.normal;
    }
    //return l0;
    Vec3f direct_l;
    {
        float pdf_sum = 0;
        Vec3f light1, light3, light2;

        float pdf_light = 0;
        Intersection pos;
        pos = sampleLight(pdf_light, thread_id);
        Vec3f p2x = (pos.point - intersection.point);
        double distance = p2x.squaredNorm();
        p2x.normalize();
        //采样光源
        if (intersection.normal.dot(p2x) > 0) {
            Intersection tmp_inter = this->find(Ray(intersection.point, p2x), thread_id);
            if ((tmp_inter.t >= (pos.point - intersection.point).norm() - 1e-3 && intersection.obj != tmp_inter.obj)) {
                //float tmp = std::max(0.0f, pos.normal.dot(-p2x) * intersection.normal.dot(p2x));
                float tmp = std::max(0.0f, intersection.normal.dot(p2x));
                //float tmp = std::abs(intersection.normal.dot(p2x));
                pdf_light *= distance / std::max(1e-7f, pos.normal.dot(-p2x));
                if (pdf_light > 0) {
                    light1 = pos.material->emit() * intersection.material->brdf(-ray.dir, p2x, intersection.uv, intersection.normal) * tmp / (pdf_light);
                }
                
            }
        }


        pdf_sum += pdf_light * pdf_light;

        float pdf_light2 = 0;
        pos = sampleLight2(pdf_light2, thread_id);
        p2x = (pos.point - intersection.point);
        distance = p2x.squaredNorm();
        p2x.normalize();
        //采样光源
        if(intersection.normal.dot(p2x)>0){
            Intersection tmp_inter = this->find(Ray(intersection.point, p2x), thread_id);
            if ((tmp_inter.t >= (pos.point - intersection.point).norm() - 1e-3 && intersection.obj != tmp_inter.obj)) {
                //float tmp = std::max(0.0f, pos.normal.dot(-p2x) * intersection.normal.dot(p2x));
                float tmp = std::max(0.0f, intersection.normal.dot(p2x));
                //float tmp = std::abs(intersection.normal.dot(p2x));
                pdf_light2 *= distance / std::max(1e-7f, pos.normal.dot(-p2x));
                if (pdf_light2 > 0) {
                    light3 = pos.material->emit() * intersection.material->brdf(-ray.dir, p2x, intersection.uv, intersection.normal) * tmp / (pdf_light2);
                }
                
            }
        }
        pdf_sum += pdf_light2 * pdf_light2;

        // 采样brdf
        float brdr_pdf = 0;
        bool is_refract;
        auto wo = intersection.material->sample(-ray.dir, intersection.normal, brdr_pdf, is_refract, thread_id);
        Ray new_ray(intersection.point, wo);
        if (!is_refract) {
            auto inter = find(new_ray, thread_id);
            if (!isinf(inter.t) && inter.obj != intersection.obj && inter.material->hasEmission() && brdr_pdf > 0) {
                auto p2x = intersection.point - inter.point;
                auto dis = p2x.dot(p2x);
                p2x.normalize();
                if (brdr_pdf > 0) {
                    light2 = inter.material->emit() * intersection.material->brdf(-ray.dir, wo, intersection.uv, intersection.normal) * std::max(0.f, intersection.normal.dot(wo)) / brdr_pdf;
                }
                
            }
        }
        pdf_sum += brdr_pdf * brdr_pdf;
        if (pdf_sum > 0) {
            direct_l = (light1 * pdf_light * pdf_light + light3 * pdf_light2 * pdf_light2 + light2 * brdr_pdf * brdr_pdf) / pdf_sum;
        }
        

    }
    //return direct_l;
    Vec3f indir_l;
    if (RandomEngineManager::getInstance().getEngine(thread_id).sampleUniform(0, 1) < roulette_coefficient) {
        float pdf;
        bool is_refract;
        
        auto wo = intersection.material->sample(-ray.dir, intersection.normal, pdf, is_refract, thread_id);
        Ray new_ray(intersection.point, wo);
        
        auto cosv = std::max(0.f, intersection.normal.dot(wo));
        /*if (is_refract) {
            return castRay(Ray(intersection.point, wo), thread_id) * 0.9f / pdf / roulette_coefficient;
        }*/
        /*if (cosv > 0.f && pdf > 0) {
            auto tmp = intersection.material->brdf(-ray.dir, wo, intersection.uv, intersection.normal) * castRay(new_ray, thread_id, depth + 1) * cosv;
            indir_l = tmp / pdf / roulette_coefficient;
        }*/
       if (is_refract) {
           indir_l = castRay(new_ray, thread_id, depth) * 0.9f / pdf / roulette_coefficient;
        } else {
            if (cosv > 0.f && pdf > 0) {
                auto tmp = intersection.material->brdf(-ray.dir, wo, intersection.uv, intersection.normal) * castRay(new_ray, thread_id, depth + 1) * cosv;
                indir_l = tmp / pdf / roulette_coefficient;
            }
        }
    }
    
    return (l0 + direct_l + indir_l);
}

ZLib::Vec3f Scene::castRay2(const Ray& ray, int thread_id)
{
    using ZLib::Vec3f;
    struct PathNode
    {
        PathNode(){}
        PathNode(const Intersection& _pos, const float& _pdf, const Vec3f& _wi, bool _is_refract, const Vec3f& _l):
            pos(_pos), pdf(_pdf),wi(_wi), is_refract(_is_refract), L0(_l), trans(1,1,1)
        {}
        Intersection pos;
        float pdf;
        ZLib::Vec3f wi;
        bool is_refract;
        Vec3f L0;
        Vec3f trans;
    };
    float roulette_coefficient = 0.8;
    float rr = 1;
    int bounces = 0;
    auto engine = RandomEngineManager::getInstance().getEngine(thread_id);
    while (engine.sampleUniform() < roulette_coefficient) {
        rr *= roulette_coefficient;
        ++bounces;
    }
    bounces = bounces > 0 ?  1 : 0;
    std::vector<PathNode> camera_path;
    std::vector<PathNode> light_path;

    Intersection pos = find(ray, thread_id);
    camera_path.push_back(PathNode(pos, 1, -ray.dir, false, pos.material->emit()));
    for (int i = 0; i < bounces; ++i) {
        auto& node = camera_path.front();
        float pdf;
        bool is_refract;
        auto wo = node.pos.material->sample(node.wi, node.pos.normal, pdf, is_refract, thread_id);
        if (!is_refract && node.pos.normal.dot(wo) < 0) {
            break;
        }
        
        auto new_pos = find(Ray(pos.point, wo), thread_id);
        if (std::isinf(new_pos.t)) {
            break;
        }
        auto wi = -wo;
        PathNode new_node(new_pos, pdf, wi, is_refract, new_pos.material->emit());
        Vec3f brdf;
        if (node.is_refract) {
            new_node.trans = node.trans / new_node.pdf / roulette_coefficient;
        } else {
            brdf = node.pos.material->brdf(node.wi, wo, node.pos.uv, node.pos.normal);
            new_node.trans = node.trans * brdf * std::max(0.f, node.pos.normal.dot(wo)) / new_node.pdf / roulette_coefficient;
        }
        new_node.L0 = node.L0 + new_node.trans * new_node.L0 / M_PI;
        
        new_node.pdf *= node.pdf;
        camera_path.push_back(std::move(new_node));
    }

    float light_pdf;
    auto light_start = sampleLight(light_pdf, thread_id);
    float origin_light_pdf = light_pdf;
    light_path.push_back(PathNode(light_start, light_pdf, light_start.normal, false, light_start.material->emit() / light_pdf));
    for (int i = 1; i < bounces; ++i) {
        auto& node = light_path.front();
        float pdf;
        bool is_refract;
        auto wo = node.pos.material->sample(node.wi, node.pos.normal, pdf, is_refract, thread_id);
        if (!is_refract && node.pos.normal.dot(wo) < 0) {
            break;
        }
        auto new_pos = find(Ray(pos.point, wo), thread_id);
        if (std::isinf(new_pos.t)) {
            break;
        }
        
        auto wi = -wo;
        if (light_path.size() == 1) {
            auto p2x = new_pos.point - node.pos.point;
            auto dis = p2x.squaredNorm();
            p2x.normalize();
            auto tmp = dis / std::max(1e-5f, node.pos.normal.dot(p2x));
            node.pdf *= tmp;
            node.L0 /= tmp;
            node.trans = Vec3f();
        }
        PathNode new_node(new_pos, pdf, wi, is_refract, node.pos.material->emit());
        if (new_node.is_refract) {
            new_node.trans = node.L0 + node.trans / pdf / roulette_coefficient;
        } else {
            new_node.trans = node.L0 + node.trans * node.pos.material->brdf(node.wi, wo, node.pos.uv, node.pos.normal) * std::max(0.f,node.pos.normal.dot(wo)) / pdf / roulette_coefficient;
            
        }
        new_node.pdf *= node.pdf;
        light_path.push_back(std::move(new_node));
    }
    if (camera_path.size() + light_path.size() - 2 < bounces) {
        return Vec3f(0, 0, 0);
    }
    float total_pdf = 0;
    std::vector<Vec3f> results;
    std::vector<float> pdf2;
    for (int i = 0; i < camera_path.size(); ++i) {
        if (i + light_path.size() >= bounces) {
            if (i == bounces) {
                //continue;
                auto& camera_node = camera_path[i];
                results.push_back(camera_node.L0);
                pdf2.push_back(camera_node.pdf* camera_node.pdf);
                total_pdf += pdf2.back();
                continue;
            }
            int j = bounces - i - 1;
            
            auto& camera_node = camera_path[0];
            j = 0;
            auto& light_node = light_path[j];
            
            auto c2l = light_node.pos.point - camera_node.pos.point;
            c2l.normalize();
            Vec3f re;
            float light_pdf = light_node.pdf;
            auto L0 = light_node.L0;
            if (j == 0) {
                auto p2l = light_node.pos.point - camera_node.pos.point;
                auto dis = p2l.squaredNorm();
                p2l.normalize();
                light_pdf = origin_light_pdf * dis / std::max(1e-5f, light_node.pos.normal.dot(-p2l));
                L0 *= light_node.pdf;
                L0 /= light_pdf;
            }
            if (camera_node.is_refract || light_node.is_refract) {
                results.push_back(re);
                pdf2.push_back(camera_node.pdf* camera_node.pdf* light_pdf* light_pdf);
                total_pdf += pdf2.back();
                continue;
            }
            if (c2l.dot(-light_node.pos.normal) > 0 && c2l.dot(camera_node.pos.normal) > 0) {
                Ray new_ray(camera_node.pos.point, c2l);
                auto new_pos = find(new_ray, thread_id);
                if ((new_pos.t >= (camera_node.pos.point - light_node.pos.point).norm() - 1e-3 && light_node.pos.obj != new_pos.obj)) {
                    auto cosv = std::max(0.f, camera_node.pos.normal.dot(c2l));
                    auto cosv2 = std::max(0.f, light_node.pos.normal.dot(-c2l));
                    auto l1 = L0 + light_node.trans * light_node.pos.material->brdf(light_node.wi, -c2l, light_node.pos.uv, light_node.pos.normal) * cosv2;
                    re = l1 * camera_node.pos.material->brdf(camera_node.wi, c2l, camera_node.pos.uv, camera_node.pos.normal) * camera_node.trans * cosv + camera_node.L0;
                } 
            } 
            results.push_back(re);
            pdf2.push_back(camera_node.pdf* camera_node.pdf* light_pdf* light_pdf);
            total_pdf += pdf2.back();
        }
    }
    Vec3f re;
    for (int i = 0; i < results.size(); ++i) {
        re += results[i] * pdf2[i];
    }
    if (total_pdf != 0) {
        re /= total_pdf;
    }
    if (re[0] > 1) {
        int k = 0;
    }
    return re;
}

ZLib::Vec3f Scene::castRay3(const Ray& init_ray, int thread_id)
{
    using vec3 = ZLib::Vec3f;
    vec3 color;
    vec3 throughput(1.0f, 1.0f, 1.0f);
    vec3 emissive_color;
    vec3 wo, wi;
    Ray ray = init_ray;
    const float Krr = 0.8;
    bool emissive_flag = true;

    for (int bounce = 0;; bounce++) {
        Intersection rec = find(ray, thread_id);
        if (isinf(rec.t)) {
            break;
        }

        wo = -ray.dir;

        // material self emissive
        if (rec.material->hasEmission()) {
            if (emissive_flag && rec.normal.dot(ray.dir) < 0)
                color += throughput * rec.material->emit();
            break;
        }

        // glass material
        if (rec.material->canRefract()) {
            float pdf;
            bool tmp;
            wi = rec.material->sample(wo, rec.normal, pdf, tmp, thread_id);
            throughput *= 0.9;
            ray = Ray(rec.point, wi);
            continue;
        }

        emissive_flag = false;
        rec.normal = rec.normal.dot(wo) > 0 ? rec.normal : -rec.normal;

        vec3 direct_l;
        {
            using ZLib::Vec3f;
            float pdf_sum = 0;
            Vec3f light1, light3, light2;

            float pdf_light = 0;
            Intersection pos;
            pos = sampleLight(pdf_light, thread_id);
            Vec3f p2x = (pos.point - rec.point);
            double distance = p2x.squaredNorm();
            p2x.normalize();
            //采样光源
            if (rec.normal.dot(p2x) > 0) {
                Intersection tmp_inter = this->find(Ray(rec.point, p2x), thread_id);
                if ((tmp_inter.t >= (pos.point - rec.point).norm() - 1e-3 && rec.obj != tmp_inter.obj)) {
                    //float tmp = std::max(0.0f, pos.normal.dot(-p2x) * intersection.normal.dot(p2x));
                    float tmp = std::max(0.0f, rec.normal.dot(p2x));
                    pdf_light *= distance / std::max(1e-7f, pos.normal.dot(-p2x));
                    light1 = pos.material->emit() * rec.material->brdf(-ray.dir, p2x, rec.uv, rec.normal) * tmp / (pdf_light);
                }

            }
            pdf_sum += pdf_light * pdf_light;

            float pdf_light2 = 0;
            pos = sampleLight2(pdf_light2, thread_id);
            p2x = (pos.point - rec.point);
            distance = p2x.squaredNorm();
            p2x.normalize();
            //采样光源
            if (rec.normal.dot(p2x) > 0) {
                Intersection tmp_inter = this->find(Ray(rec.point, p2x), thread_id);
                if ((tmp_inter.t >= (pos.point - rec.point).norm() - 1e-3 && rec.obj != tmp_inter.obj)) {
                    //float tmp = std::max(0.0f, pos.normal.dot(-p2x) * intersection.normal.dot(p2x));
                    float tmp = std::max(0.0f, rec.normal.dot(p2x));
                    pdf_light2 *= distance / std::max(1e-7f, pos.normal.dot(-p2x));
                    light3 = pos.material->emit() * rec.material->brdf(-ray.dir, p2x, rec.uv, rec.normal) * tmp / (pdf_light2);
                }
            }
            pdf_sum += pdf_light2 * pdf_light2;

            // 采样brdf
            float brdr_pdf = 0;
            bool is_refract;
            auto wo = rec.material->sample(-ray.dir, rec.normal, brdr_pdf, is_refract, thread_id);
            Ray new_ray(rec.point, wo);
            if (!is_refract) {
                auto inter = find(new_ray, thread_id);
                if (!isinf(inter.t) && inter.obj != rec.obj && inter.material->hasEmission() && brdr_pdf > 0) {
                    auto p2x = rec.point - inter.point;
                    auto dis = p2x.dot(p2x);
                    p2x.normalize();
                    light2 = inter.material->emit() * rec.material->brdf(-ray.dir, wo, rec.uv, rec.normal) * std::max(0.f, rec.normal.dot(wo)) / brdr_pdf;
                }
            }
            pdf_sum += brdr_pdf * brdr_pdf;

            direct_l = (light1 * pdf_light * pdf_light + light3 * pdf_light2 * pdf_light2 + light2 * brdr_pdf * brdr_pdf) / pdf_sum;
        }
        color += throughput * direct_l;
        float pdf;
        bool tmp;
        wi = rec.material->sample(wo, rec.normal, pdf, tmp, thread_id);
        if (wi.dot( rec.normal) > 0 && pdf > 0.0f) {
            ray = Ray(rec.point, wi);
            throughput *= rec.material->brdf(wo, wi, rec.uv, rec.normal) * wi.dot( rec.normal) / pdf;
        } else {
            // throughput will be all zero, stop
            break;
        }

        if (bounce >= 3) {
            float rr = 0;
            for (int i = 0; i < 3; ++i) {
                 rr = std::max(rr, throughput[i]);
            }
            if (RandomEngineManager::getInstance().getEngine(thread_id).sampleUniform(0, 1) < rr)
                throughput /= rr;
            else
                break;
        }
    }
    return color;
}