#include "AsyncEntity.h"

AsyncEntity::AsyncEntity() noexcept : Super() {

}

AsyncEntity::~AsyncEntity() noexcept {

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
