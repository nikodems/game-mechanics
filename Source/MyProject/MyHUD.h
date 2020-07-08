// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/Canvas.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMyHUD();

	//Returning crosshair's position
	UFUNCTION()
	FVector Get3DCrosshairPosition();


protected:
	virtual void DrawHUD() override;

	//Storing crosshair texture
	class UTexture2D* crosshairTexture;

	//Storing crosshair position
	UPROPERTY(EditAnywhere)
	FVector crosshair3DPosition;
};
