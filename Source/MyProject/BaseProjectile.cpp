// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "MyActor.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create mesh component
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	//Set mesh as root component
	RootComponent = staticMesh;

	//Generate events when hitting objects
	staticMesh->SetNotifyRigidBodyCollision(true);

	//Generate events when overlapping objects
	staticMesh->bGenerateOverlapEvents = true;

	//Set collision profile
	staticMesh->BodyInstance.SetCollisionProfileName("Projectile");

	//Enable physics
	staticMesh->SetSimulatePhysics(true);

	//Disable gravity
	staticMesh->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//Add mesh to hit events
	staticMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	//Add mesh to overlap events
	staticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlapBegin);

}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (this)
		{
			//If ricochet mode is active
			if (ricochetMode)
			{
				//Increase ricochet count
				currentRicochetCount++;

				//If the projectile has ricocheted 3 times then destroy it
				if (currentRicochetCount >= 3)
				{
					Destroy();
				}

				//If pierce mode is active
				if (pierceMode)
				{
					//Clear overlap list so the projectile can pierce again
					overlapList.clear();
				}		
			}
			//If ricochet mode is not active then destroy the projectile
			else
			{
				Destroy();
			}
		}
	}
}

void ABaseProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (this)
		{
			bool alreadyTriggered = false;
			//Iterate through list of overlapped actors, if the list contains the actor currently overlapping with, set alreadyTriggered to true
			for (auto const& i : overlapList)
			{
				if (i == OtherActor)
				{
					alreadyTriggered = true;
				}
			}

			//If haven't collided with this actor yet
			if (!alreadyTriggered)
			{

				//If pierce active
				if (pierceMode)
				{
					//Increase pierce count
					currentPierceCount += 1;

					//If pierce count exceeded max
					if (currentPierceCount >= 3)
					{
						//If split mode isn't active, destroy
						if (!splitMode)
						{
							Destroy();
						}
					}
				}

				//If split mode is active
				if (splitMode)
				{
					if (!pierceMode || currentPierceCount >= 3)
					{
						if (launcherActor)
						{
							launcherActor->SplitShot(OtherActor);
						}
						Destroy();
					}
				}

				if (!pierceMode && !splitMode)
				{
					Destroy();
				}

				//Add this actor to list of actors already overlapped with
				overlapList.push_back(OtherActor);
			}
		}
	}
}

void ABaseProjectile::SetRicochetMode(bool ricochet)
{
	ricochetMode = ricochet;
}

void ABaseProjectile::SetPierceMode(bool pierce)
{
	pierceMode = pierce;
}

void ABaseProjectile::SetSplitMode(bool split)
{
	splitMode = split;
}

void ABaseProjectile::SetLauncherActor(AMyActor* launcher)
{
	launcherActor = launcher;
}