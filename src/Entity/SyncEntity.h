#ifndef CMF_SYNCENTITY_H
#define CMF_SYNCENTITY_H

#include "Entity.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

class AsyncEntity;

/**
 * @brief 
 */
class SyncEntity : public Entity {
	GENERATED_BODY(SyncEntity, Entity)

public:
	/**
	 * @brief 
	 */
	explicit SyncEntity() noexcept;

	/**
	 * @brief 
	 */
	virtual ~SyncEntity() noexcept override;

	/**
	 * @brief 
	 * @param deltaTime 
	 */
	virtual void tick(float deltaTime) noexcept override;

	/**
	 * @brief 
	 */
	virtual void begin() noexcept override;

	/**
	 * @brief 
	 * @param reason 
	 */
	virtual void end(EndReason reason) noexcept override;

protected:
	/**
	 * @brief 
	 */
	virtual void postInitProperties() noexcept override;

	/**
	 * @brief 
	 * @param oldOwner 
	 */
	virtual void onOwnerChanged(Object* oldOwner) noexcept override;

private:
	// TODO remove or change to Object
	WeakObjectPtr<AsyncEntity> ownerEntity;
};

#endif //CMF_SYNCENTITY_H