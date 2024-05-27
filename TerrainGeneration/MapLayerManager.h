#ifndef MAPLAYERMANAGER_H
#define MAPLAYERMANAGER_H

struct ColorRGB;
struct Vector3f;

#include "DataTypes.h"
#include <set>

class MapLayerManager final
{
public:
	MapLayerManager();
	~MapLayerManager();

	MapLayerManager(const MapLayerManager&) = delete;
	MapLayerManager(MapLayerManager&&) noexcept = delete;
	MapLayerManager& operator=(const MapLayerManager&) = delete;
	MapLayerManager& operator=(MapLayerManager&&) noexcept = delete;

	void AddLayer(const std::string& name, float height, const ColorRGB& color);
	const ColorRGB& GetColorFromValue(float value) const;
	const ColorRGB& GetDefaultColor() const;

private:

	// found on stackoverflow!
	//template< class C, class T, T C::* M >
	//struct member_comparator // predicate
	//{
	//	bool operator()(C const* x, C const* y) const
	//	{
	//		return x->*M < y->*M;
	//	}
	//};

	struct CompareHeight
	{
		bool operator()(const TerrainLayer* lhs, const TerrainLayer* rhs) const 
		{
			return lhs->height < rhs->height;
		}
	};

	// private member functions
	std::set< TerrainLayer*, CompareHeight > m_pLayers;

	const ColorRGB m_DefaultLayerColor;
};

#endif // !MAPLAYERMANAGER_H