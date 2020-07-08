// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"

AMyHUD::AMyHUD()
{
	//Load in texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> crossTex(TEXT("Texture2D'/Game/crossh.crossh'"));
	if (crossTex.Succeeded())
	{
		crosshairTexture = crossTex.Object;
	}

	crosshair3DPosition = FVector(0.0f, 0.0f, 0.0f);
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	//Set the centre of the screen by finding the bottom right corner of the screen and dividing by two
	FVector2D Centre(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	//Set crosshair centre
	FVector2D crosshairCentre(Centre.X, Centre.Y - 16.0f);

	//Create an item to render onto the screen
	if (crosshairTexture)
	{
		FCanvasTileItem TileItem(crosshairCentre, crosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		//Render the item
		Canvas->DrawItem(TileItem);
	}


	FVector WorldDirectionOfCrossHair2D;

	Canvas->Deproject(crosshairCentre, crosshair3DPosition, WorldDirectionOfCrossHair2D);

	crosshair3DPosition += WorldDirectionOfCrossHair2D;
}

FVector AMyHUD::Get3DCrosshairPosition()
{
	return crosshair3DPosition;
}



