// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackField.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;
class UBoxComponent;

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
	

private:
	void SetupGroundBoxSettings(UBoxComponent* InBox) const;
	
	UPROPERTY(EditDefaultsOnly, Category="TrackField")
	int32  TrackNum{1};
	UPROPERTY(EditDefaultsOnly, Category="TrackField")
	TObjectPtr<UPaperSprite> TrackSprite;
	UPROPERTY(VisibleDefaultsOnly, Category="TrackField")
	TObjectPtr<USceneComponent> TrackGroup;
	UPROPERTY(VisibleDefaultsOnly, Category="TrackField|GroundBox")
	TObjectPtr<UBoxComponent> GroundBoxTop;
	UPROPERTY(EditDefaultsOnly, Category="TrackField|GroundBox")
	float TopBoxY{0.f};
	UPROPERTY(VisibleDefaultsOnly, Category="TrackField|GroundBox")
	TObjectPtr<UBoxComponent> GroundBoxBottom;
	UPROPERTY(EditDefaultsOnly, Category="TrackField|GroundBox")
	float BottomBoxY{0.f};
	UPROPERTY(EditDefaultsOnly, Category="TrackField|GroundBox")
	float GroundWidth{10.f};
};
