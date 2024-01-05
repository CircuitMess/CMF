#include "AsyncEntity.h"
#include "SyncEntity.h"

AsyncEntity::AsyncEntity() noexcept : Super() {

}

AsyncEntity::~AsyncEntity() noexcept {

}

void AsyncEntity::RegisterSyncEntity(SyncEntity* entity) noexcept{
	if(entity->GetOwner() != this){
		entity->SetOwner(this);
	}

	m_RegisteredSyncEntities.insert(entity);
}

void AsyncEntity::UnregisterSyncEntity(SyncEntity* entity) noexcept {
	if(entity->GetOwner() == this){
		entity->SetOwner(nullptr);
	}

	m_RegisteredSyncEntities.erase(entity);
}

void AsyncEntity::PostInitProperties() noexcept {
	Super::PostInitProperties();
}

void AsyncEntity::Begin() noexcept {
	Super::Begin();
}

void AsyncEntity::Tick(float deltaTime) noexcept {
	Super::Tick(deltaTime);
}

void AsyncEntity::End() noexcept {
	Super::End();
}

void AsyncEntity::TickHandle() noexcept{
	Tick(0.0f); // TODO: delta time

	for(SyncEntity* entity : m_RegisteredSyncEntities){
		if(entity != nullptr){ // TODO: check ptr validity instead of nullptr
			entity->Tick(0.0f); // TODO: delta time
		}
	}
}