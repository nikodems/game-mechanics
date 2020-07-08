// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class MYPROJECT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Collision resolution
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Overlap resolution
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//Mesh of projectile
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* staticMesh;

	//Number of times the projectile has ricochet
	UPROPERTY()
		int currentRicochetCount = 0;

	//Number of times the projectile has pierced
	UPROPERTY()
		int currentPierceCount = 0;

	//Is pierce mode enabled?
	UPROPERTY()
		bool pierceMode = false;

	//Is the bounce mode enabled?
	UPROPERTY()
		bool ricochetMode = false;

	//Is split mod enabled?
	UPROPERTY()
		bool splitMode = false;

	std::list<AActor*> overlapList;

	class AMyActor* launcherActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Set mode values
	void SetRicochetMode(bool ricochet);

	void SetPierceMode(bool pierce);

	void SetSplitMode(bool split);

	void SetLauncherActor(class AMyActor* launcher);

	//Damage of projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float baseDamage = 100.0f;

	//How fast projectile travels
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float projectileTravelSpeed = 50000.0f;

	//How often projectile fires
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float projectileFireSpeed = 0.5f;
};
