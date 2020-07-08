// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"
#include "MyHUD.h"


// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	numOfProjToFire = 1;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		playerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	}

	myHUDClass = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	currentFireTimer += DeltaTime;
}

void AMyActor::FireProjectile()
{
	//Is not null?
	if (projectileToSpawn)
	{
		//Enough time has passed
		if (currentFireTimer > projFireTimer)
		{
			FVector crossPos = FVector(0.0f, 0.0f, 0.0f);
			if (myHUDClass)
			{
				//Get crosshair position
				crossPos = myHUDClass->Get3DCrosshairPosition();
			}

			//Calculate vector from player to crosshair
			FVector direction = playerController->GetPawn()->GetActorLocation() - crossPos;
			//Don't want z value
			direction = FVector(direction.X, direction.Y, 0);
			FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();

			//Rotate player towards the crosshair
			playerController->GetPawn()->SetActorRotation(Rot);

			//Distance between spawned projectiles
			int offset = 0;

			//Loop through the number of projectiles to fire
			for (int i = 0; i < numOfProjToFire; i++)
			{
				if (i > 0)
				{
					//% - modulo, if the remainder of i / 2 = 0
					//Checks if i is positive or negative
					if (i % 2 == 0)
					{
						offset += ((i) * 100);
					}
					else
					{
						offset -= ((i) * 100);
					}
				}

				//Spawn projectile
				firedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(projectileToSpawn, playerController->GetPawn()->GetActorLocation() + (playerController->GetPawn()->GetActorRightVector() * offset) + (playerController->GetPawn()->GetActorForwardVector() * 50), FRotator::ZeroRotator);


				if (firedProjectile)
				{
					//Set firing modes
					firedProjectile->SetRicochetMode(projectileRicochet);
					firedProjectile->SetPierceMode(projectilePierce);
					firedProjectile->SetSplitMode(projectileSplit);

					//Give projectile reference to the launcher
					firedProjectile->SetLauncherActor(this);

					//Adjust firing speed
					projFireTimer = firedProjectile->projectileFireSpeed;

					//Get mesh
					firedProjectileMesh = (UStaticMeshComponent*)firedProjectile->GetRootComponent();

					//Apply impulse
					if (firedProjectileMesh)
					{
						firedProjectileMesh->AddImpulse(playerController->GetPawn()->GetActorForwardVector() * firedProjectile->projectileTravelSpeed);
					}
				}
			}

			//Reset fire delay
			currentFireTimer = 0;
		}
	}
}

void AMyActor::SplitShot(AActor* actor)
{
	int offset = -60;

	float angleOffset = 1;

	for (int i = 0; i < 2; i++)
	{
		if (i == 1)
		{
			offset = 60;

			angleOffset *= -1;
		}

		//RightVector is actually the forward vector - ForwardVector is the left vector
		firedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(projectileToSpawn, actor->GetActorLocation() - ((actor->GetActorRightVector() * 100) + (actor->GetActorForwardVector() * offset)), FRotator::ZeroRotator);

		if (firedProjectile)
		{
			firedProjectile->SetRicochetMode(projectileRicochet);
			firedProjectile->SetPierceMode(projectilePierce);

			firedProjectileMesh = (UStaticMeshComponent*)firedProjectile->GetRootComponent();

			if (firedProjectileMesh)
			{
				firedProjectileMesh->AddImpulse(((-actor->GetActorRightVector() + (actor->GetActorForwardVector() * angleOffset)) * firedProjectile->projectileTravelSpeed));
			}
		}
	}
}
