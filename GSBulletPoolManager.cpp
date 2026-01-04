// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Bullet/GSBulletPoolManager.h"
#include "Gameplay/Bullet/GSBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"


void UGSBulletPoolManager::initBulletPool(TSubclassOf<AGSBullet> bullet_instance_class_, int size_)
{
    UWorld* world = GetWorld();

    if (!IsValid(world))
    {
        return;
    }

    _bullet_instance_class = bullet_instance_class_;
    _has_been_initialized = true;
    _available_bullet.Reserve(size_);
    _active_bullet.Reserve(size_);
    _pool_max_size = size_;
    GetWorld()->GetTimerManager().SetTimer(_spawn_timer_handle, this, &UGSBulletPoolManager::addBullet, 0.1f, true);   
}

void UGSBulletPoolManager::addBullet()
{
    if (_available_bullet.Num() == _pool_max_size)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(_spawn_timer_handle);
        }
        return;
    }

    FActorSpawnParameters spawn_parameters;
    spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FTransform transform;
    AGSBullet* bullet = GetWorld()->SpawnActor<AGSBullet>(_bullet_instance_class->GetAuthoritativeClass(), transform, spawn_parameters);
    bullet->resetBullet();
    _available_bullet.Add(bullet);
    //UE_LOG(LogTemp, Warning, TEXT("Spawn bullet %s"), *bullet->GetName());
    //DrawDebugString(GetWorld(), bullet->GetActorLocation(), bullet->GetName(), NULL, FColor::Green, 2.0f);
}

AGSBullet* UGSBulletPoolManager::spawnBullet(FTransform transform_, FVector direction_)
{
    if (_available_bullet.Num() == 0)
    {
        return nullptr;
    }
    
    AGSBullet* current_bullet = _available_bullet[_available_bullet.Num() -1];
    if (IsValid(current_bullet))
    {
        _active_bullet.Add(current_bullet);
        _available_bullet.RemoveAt(_available_bullet.Num() - 1);
        current_bullet->enableBulletLifeSpan(transform_,direction_);
        _is_tickable = true;
        return current_bullet;
    }

    return nullptr;
}

void UGSBulletPoolManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (_active_bullet.Num() == 0)
    {
        _is_tickable = false;
        return;
    }

    for (int32 i = _active_bullet.Num() - 1; i >= 0 ; i--)
    {
        AGSBullet* bullet = _active_bullet[i];
        if (!bullet->_is_active)
        {
            _active_bullet.RemoveAtSwap(i);
            _available_bullet.Add(bullet);
        }
        else
        {
            bullet->updateBulletPool(DeltaTime);
        }
    }
}


TStatId UGSBulletPoolManager::GetStatId() const
{
    return TStatId();
}


bool UGSBulletPoolManager::IsTickable() const
{
    return _is_tickable;
}

bool UGSBulletPoolManager::IsTickableInEditor() const
{
    return false;
}

bool UGSBulletPoolManager::IsTickableWhenPaused() const
{
    return false;
}

void UGSBulletPoolManager::Deinitialize()
{
    Super::Deinitialize();
    GetWorld()->GetTimerManager().ClearTimer(_spawn_timer_handle);
}