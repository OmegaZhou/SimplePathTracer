#include "../Interface/IMaterial.h"
#include "../Interface/ITexture.h"

class BlinnPhongMaterial :public IMaterial
{
	typedef ZLib::Vec3f Vec3f;
public:
	// 通过 IMaterial 继承
	virtual ZLib::Vec3f sample(const ZLib::Vec3f& wi, const ZLib::Vec3f& normal, float& pdf, bool& is_refract, int engine_id) const override;
	virtual ZLib::Vec3f brdf(const ZLib::Vec3f& wi, const ZLib::Vec3f& wo, const ZLib::Vec2f& uv, const ZLib::Vec3f& normal) const override;
	virtual bool hasEmission() const override;
	virtual const std::string& name() const override;
	virtual ZLib::Vec3f emit() const override;
	virtual bool canRefract() const override;
	BlinnPhongMaterial();


	Vec3f kd;
	Vec3f ks;
	Vec3f ka;
	float ni;
	float ns;
	// 代表radiance
	Vec3f emission;
	const ITexture* kd_map;
	const ITexture* ka_map;
	const ITexture* ks_map;
	std::string _name;

};