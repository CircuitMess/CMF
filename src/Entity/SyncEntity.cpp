#include "SyncEntity.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

SyncEntity::SyncEntity(AsyncEntity* owner/* = nullptr*/) noexcept : Super() {
	SetOwner(owner);
}

SyncEntity::~SyncEntity() noexcept {
	m_Owner = nullptr;
}

void SyncEntity::SetOwner(AsyncEntity* owner) noexcept {
	AsyncEntity* oldOwner = m_Owner;

	if(owner == nullptr){
		m_Owner = ApplicationStatics::GetApplication();
	}else{
		m_Owner = owner;
	}

	OnOwnerChanged(oldOwner);
}

AsyncEntity* SyncEntity::GetOwner() const noexcept {
	return m_Owner;
}

void SyncEntity::PostInitProperties() noexcept {
	Super::PostInitProperties();
}

void SyncEntity::Begin() noexcept {
	Super::Begin();
}

void SyncEntity::Tick(float deltaTime) noexcept {
	Super::Tick(deltaTime);
}

void SyncEntity::End() noexcept {
	Super::End();
}

void SyncEntity::OnOwnerChanged(AsyncEntity* oldOwner) noexcept {
	// TODO: remove this from old owner tick
	// TODO: register this for new owner tick
}
