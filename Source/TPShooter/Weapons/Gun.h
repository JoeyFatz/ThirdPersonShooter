// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class TPSHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger();
	void Reload();

	/** Sends ammo counts to the HUD UMG widget */
	UFUNCTION(BlueprintPure)
	float GetCarryAmmo() const;
	UFUNCTION(BlueprintPure)
	float GetCurrentAmmo() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** Declare default actor components */
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	/** Declare particle effects */
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	UParticleSystem* BulletImpact;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere)
	USoundBase* NoAmmoSound;
	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere)
	USoundBase* BulletImpactSound;

	AController* GetOwnerController() const;

	bool GunTrace(FHitResult& HitResult, FVector& ShotDirection);

	/************************************************************************/
	/*                 Weapon Reload/Ammo Functionality                     */
	/************************************************************************/

	/** Disables firing while the player is reloading */
	FTimerHandle ReloadTimer;
	
	UFUNCTION()
	void ToggleFire();
	UFUNCTION()
	void TempDisableTrigger();
	
	UPROPERTY()
	bool bCanFire = true;
	UPROPERTY(EditAnywhere)
	float ReloadDelay = 1.25f;

	/** Sets the weapon firing range and damage */
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;
	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	/** Sets and tracks the weapon ammo count and max carry limits */
	UPROPERTY(EditAnywhere)
	float Ammo = 15.f;
	UPROPERTY(EditAnywhere)
	float MaxCarryAmmo = 120.f;
	UPROPERTY()
	float CarryAmmo = 20.f;
	UPROPERTY(EditAnywhere)
	float MaxClipAmmo = 15.f;
	UPROPERTY()
	float ExcessAmmo = 0.f;

};
