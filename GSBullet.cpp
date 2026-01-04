// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Bullet/GSBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Gameplay/Bullet/GSBulletPoolManager.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
AGSBullet::AGSBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGSBullet::BeginPlay()
{
    Super::BeginPlay();
    _projectile_movement_component = FindComponentByClass<UProjectileMovementComponent>();
    _bullet_pool_manager = GetWorld()->GetSubsystem<UGSBulletPoolManager>();
}

void AGSBullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(_life_span_timer_handle);
    Super::EndPlay(EndPlayReason);
}

// Called every frame
void AGSBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGSBullet::enableBulletLifeSpan(FTransform transform_, FVector direction_)
{
    _is_active = true;
    SetActorHiddenInGame(false);
    SetActorLocation(transform_.GetLocation());
    SetActorRotation(direction_.Rotation());
    _current_velocity = GetRootComponent()->GetForwardVector() * _initial_speed_pool_bullet;
	GetWorldTimerManager().SetTimer(_life_span_timer_handle, this, &AGSBullet::resetBullet, _bullet_life_span, false);
}

void AGSBullet::resetBullet()
{
    _is_active = false;
    SetActorEnableCollision(false);
    SetActorTickEnabled(false);
    SetActorHiddenInGame(true);
    if (_projectile_movement_component)
    {
        _projectile_movement_component->StopMovementImmediately();
        _projectile_movement_component->Deactivate();
    }
}

void AGSBullet::updateBulletPool(float delta_time_)
{
    if (!_is_active)
    {
        return;
    }

    FHitResult out_hit;
    TArray<AActor*> ignore;
    ignore.Add(this);
    FHitResult hit_result;

    ECollisionChannel collision_channel = ECollisionChannel::ECC_WorldDynamic;
    FCollisionShape sphere_shape = FCollisionShape::MakeSphere(4.0f);

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(sphereSingleAttackTrace), false, this);
    FCollisionResponseParams ResponseParam;
    ResponseParam.CollisionResponse.SetResponse(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    ResponseParam.CollisionResponse.SetResponse(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    ResponseParam.CollisionResponse.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Block);
    QueryParams.AddIgnoredActor(this);

    //bool hit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + _current_velocity * delta_time_ * _speed_pool_bullet, 20.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignore, EDrawDebugTrace::ForDuration, hit_result, true);
    bool hit = GetWorld()->SweepSingleByChannel(out_hit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.0f,
        FQuat::Identity, collision_channel, sphere_shape, QueryParams, ResponseParam);

    if (hit)
    {
      DrawDebugString(GetWorld(), out_hit.Location, out_hit.GetActor()->GetName(), NULL, FColor::Red, 2.0f);
      //UE_LOG(LogTemp, Warning, TEXT("COLLISION %s"), *out_hit.GetActor()->GetName());
      resetBullet();
      return;
    }
    SetActorLocation(GetActorLocation() + _current_velocity * delta_time_ * _speed_pool_bullet);
}




