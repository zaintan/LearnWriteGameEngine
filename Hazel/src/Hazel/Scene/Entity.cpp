#include "hzpch.h"
#include "Hazel/Scene/Entity.h"

namespace Hazel {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}