// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create default scene root on actor
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create the Skeletal Mesh for the weapon
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AGun::GetOwnerController() const
{
	// Cast to APawn to get controller and players viewpoint from the controller
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) { return NULL; }

	return OwnerPawn->GetController();
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) { return false; }

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	
	// Get where the shot is coming from
	ShotDirection = -ViewPointRotation.Vector();

	// Set the end point for the raycast
	FVector EndPoint = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	// Ignore the owning character and their gun
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(HitResult, ViewPointLocation, EndPoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
}



void AGun::PullTrigger()
{
	// TODO: If ammo variable > 0 / do NOT trigger fire effects and trigger reload.
	if (bCanFire)
	{
		if (Ammo > 0)
		{
			--Ammo;

			// Spawn muzzle flash when trigger is pulled
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
			UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

			FHitResult HitResult;
			FVector ShotDirection;

			bool bSuccess = GunTrace(HitResult, ShotDirection);
			if (bSuccess)
			{
				// TODO: Spawn bullet impact Decal
				// TODO: Change effect/decal depending on object hit
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpact, HitResult.Location, ShotDirection.Rotation());
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletImpactSound, HitResult.Location);

				// Get the Actor hit by the raycast
				AActor* HitActor = HitResult.GetActor();

				if (HitActor)
				{
					// Construct Damage Event for Take Damage function
					FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, NULL);
					AController* OwnerController = GetOwnerController();
					HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
				}
			}
		}
		else
		{
			UGameplayStatics::SpawnSoundAttached(NoAmmoSound, Mesh, TEXT("MuzzleFlashSocket"));

			// TODO: Trigger reload animation
			// TODO: Delay fire until reload is complete
			Reload();
			TempDisableTrigger();
		}
	}

}

void AGun::Reload()
{
	if (Ammo < MaxClipAmmo)
	{
		if (CarryAmmo > 0)
		{
			UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));
			TempDisableTrigger();

			ExcessAmmo = MaxClipAmmo - Ammo;

			if (ExcessAmmo <= CarryAmmo)
			{
				CarryAmmo = CarryAmmo -= ExcessAmmo;
				CarryAmmo = FMath::Clamp(CarryAmmo, 0.f, MaxCarryAmmo);
				Ammo = MaxClipAmmo;
			}
			else
			{
				Ammo = Ammo += CarryAmmo;
				Ammo = FMath::Clamp(Ammo, 0.f, MaxClipAmmo);
				CarryAmmo = 0;
			}
		}
	}
	GetCarryAmmo();
	GetCurrentAmmo();
}

void AGun::TempDisableTrigger()
{
	bCanFire = false;
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AGun::ToggleFire, ReloadDelay);
}

void AGun::ToggleFire()
{
	bCanFire = true;
}

float AGun::GetCarryAmmo() const
{
	return CarryAmmo;
}

float AGun::GetCurrentAmmo() const
{
	return Ammo;
}

