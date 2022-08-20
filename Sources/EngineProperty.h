//
// Created by boris on 8/8/2022.
//

#ifndef VENGINE3D_ECS_PROPERTY_H
#define VENGINE3D_ECS_PROPERTY_H
static constexpr auto MEMORY_MANAGER_CHUNK_SIZE = 256 * 1024 * 1024ull; // in bytes
static constexpr auto SYSTEM_MEMORY_BUFFER_SIZE = 10 * 1024 * 1024ull;
static constexpr auto ENTITY_CHUNK_SIZE = 3;	  // entity number
static constexpr auto COMPONENT_CHUNK_SIZE = 3; // components number
static constexpr auto TABLE_GROW = 1024;

static constexpr float FLOAT_EPSILON = 0.000001f;



#endif // VENGINE3D_ECS_PROPERTY_H
