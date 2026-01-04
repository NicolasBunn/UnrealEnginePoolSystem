// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSBullet.generated.h"

class UProjectileMovementComponent;
class UGSBulletPoolManager;

UCLASS()
class GAMESTARTER_API AGSBullet : public AActor
{
	GENERATED_BODY()

public:
	FVector _current_velocity;
	TObjectPtr<UProjectileMovementComponent> _projectile_movement_component;
	TObjectPtr<UGSBulletPoolManager> _bullet_pool_manager;
	FTimerHandle _life_span_timer_handle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _bullet_life_span = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _speed_pool_bullet = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _initial_speed_pool_bullet = 50.0f;
	bool _is_active = false;

	// Sets default values for this actor's properties
	AGSBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void enableBulletLifeSpan(FTransform transform_, FVector direction_);
	void resetBullet();

	void updateBulletPool(float delta_time_);








};
