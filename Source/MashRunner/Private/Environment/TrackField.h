// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackField.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;

UCLASS()
class ATrackField : public AActor
{
	GENERATED_BODY()

	// ============================
	// AActor
	// ============================
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// ============================
	// ATrackField
	// ============================
public:	
	ATrackField();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TrackField")
	int32  TrackNum{1};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TrackField")
	TObjectPtr<UPaperSprite> TrackSprite;

private:
	TObjectPtr<USceneComponent> TrackGroup;
};
