// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GSBulletPoolManager.generated.h"

class AGSBullet;

UCLASS()
class GAMESTARTER_API UGSBulletPoolManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:

	TArray<AGSBullet*> _available_bullet;
	TArray<AGSBullet*> _active_bullet;

	TSubclassOf<AGSBullet> _bullet_instance_class;
	FTimerHandle _spawn_timer_handle;

	int32 _pool_max_size = 0;

	bool _has_been_initialized = false;
	bool _is_tickable = false;

	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;

	void addBullet();
	void initBulletPool(TSubclassOf<AGSBullet> bullet_instance_class_, int size_);

	AGSBullet* spawnBullet(FTransform transform_, FVector direction_);
};
