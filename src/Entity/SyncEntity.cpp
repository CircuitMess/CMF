#include "SyncEntity.h"

SyncEntity::SyncEntity() noexcept : Super() {

}

SyncEntity::~SyncEntity() noexcept {

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
