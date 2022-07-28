/**
	Coordinate structure define 
*/
struct coord_t
{
	vec3 ux = vec3::UX;		// three axis
	vec3 uy = vec3::UY;
	vec3 uz = vec3::UZ;
	vec3 scl = vec3::ONE;		// scale	
	vec3 o;				// origin
	
	coord_t() {}
	coord_t(crvec _ux, crvec _uy, crvec _uz)
	{
		ux = _ux; uy = _uy; uz = _uz;
	}
	void rot(real ang, crvec ax)
	{
	//	o.rot(ang, ax);
		ux.rot(ang, ax);
		uy.rot(ang, ax);
		uz.rot(ang, ax);
	}
	// C1*C2*C3* ... *Cloc * Vloc （transfrom)
	vec3 operator * (crvec v)
	{
		return ux * v.x + uy * v.y + uz * v.z + o;
	}
	// V * C1 * C2 ...
	friend vec3 operator * (crvec v, const coord_t& c)
	{
		return c.ux * v.x + c.uy * v.y + c.uz * v.z + c.o;
	}
	coord_t operator * (coord_t& c)
	{
		coord_t rc;
		rc.ux = ux * c.ux.x + uy * c.ux.y + uz * c.ux.z;
		rc.uy = ux * c.uy.x + uy * c.uy.y + uz * c.uy.z;
		rc.ux = ux * c.uz.x + uy * c.uz.y + uz * c.uz.z;
		rc.o += ux * c.o.x + uy * c.o.y + uz * c.o.z;
		return rc;
	}
	// Vworld/C1/C2/C3/ ... /Cloc（projection)
	friend vec3 operator / (crvec v, const coord_t& c)
	{
		vec3 dv = v - c.o;
		return vec3(dv.dot(c.ux), dv.dot(c.uy), dv.dot(c.uz));
	}
	coord_t operator / (const coord_t& c)
	{
		coord_t rc;
		rc.ux = vec3(ux.dot(c.ux), ux.dot(c.uy), ux.dot(c.uz));
		rc.uy = vec3(uy.dot(c.ux), uy.dot(c.uy), uy.dot(c.uz));
		rc.uz = vec3(uz.dot(c.ux), uz.dot(c.uy), uz.dot(c.uz));
		rc.o -= c.o;
		return rc;
	}
	void dump()
	{
		PRINT("-------");
		PRINT("ux: " << ux.x << "," << ux.y << "," << ux.z);
		PRINT("uy: " << uy.x << "," << uy.y << "," << uy.z);
		PRINT("uz: " << uz.x << "," << uz.y << "," << uz.z);
		PRINT("scl: " << scl.x << "," << scl.y << "," << scl.z);
		PRINT("o: " << uz.x << "," << uz.y << "," << uz.z);
	}
};
inline vec3 coord2eulers(coord_t& rm)
{
	float sy = sqrt(rm.ux.x * rm.ux.x + rm.uy.x * rm.uy.x);
	bool singular = sy < 1e-6;

	float x, y, z;
	if (!singular)
	{
		x = atan2(rm.uz.y, rm.uz.z);
		y = atan2(-rm.uz.x, sy);
		z = atan2(rm.uy.x, rm.ux.x);
	}
	else
	{
		x = atan2(-rm.uy.z, rm.uy.y);
		y = atan2(-rm.uz.x, sy);
		z = 0;
	}
	return vec3(x, y, z);
}
