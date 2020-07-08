// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <list>
#include "BaseProjectile.h"
#include "MyHUD.h"
#include "Engine/World.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"


UCLASS(Blueprintable)
class MYPROJECT_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void FireProjectile();

private:
	//A reference to the projectile after is has been fired
	UPROPERTY()
		class ABaseProjectile* firedProjectile;

	//A reference to the projectile's mesh after it has been fired
	UPROPERTY()
		UStaticMeshComponent* firedProjectileMesh;

	//The current timer since a projectile has been fired (Defaulted to 100 so a projectile can fire immediately after starting the game
	UPROPERTY(EditAnywhere)
		float currentFireTimer = 100.0f;

	//Player Controller
	UPROPERTY(EditAnywhere)
		APlayerController* playerController;

	//HUD class
	UPROPERTY(EditAnywhere)
		AMyHUD* myHUDClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Number of projectile to fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int numOfProjToFire;

	//The projectile's timer, - how often a projectile can fire
	UPROPERTY()
		float projFireTimer = 1.0f;

	//Projectile bounce mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool projectileRicochet = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool projectilePierce = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool projectileSplit = false;

	//Function for spawning projectiles for Split Shot
	UFUNCTION()
		void SplitShot(AActor* actor);

	//What projectile to spawn?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseProjectile> projectileToSpawn;
};
