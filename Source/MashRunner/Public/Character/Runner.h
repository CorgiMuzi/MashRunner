// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Runner.generated.h"

class UInputMappingContext;
class UInputAction;
class UCurveFloat;
class UPaperFlipbook;
class USoundBase;

UCLASS()
class ARunner : public APaperCharacter
{
	GENERATED_BODY()

	friend class ARunnerAIController;
	// ============================
	// AActor
	// ============================
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// ============================
	// ARunner
	// ============================
public:
	ARunner();

	void OnWinnerAnnounced();
	/// Return current speed ratio in [0.f, MaxSpeed]
	/// @return Current speed ratio [0.f, 1.f]
	float GetCurrentSpeedRatio();

	UPROPERTY(EditAnywhere, Category="Runner")
	float DecelerationUnit{0.f};
	UPROPERTY(EditDefaultsOnly, Category="Runner")
	TObjectPtr<UCurveFloat> DecelerationRate;
	UPROPERTY(EditAnywhere, Category="Runner")
	float AccelerationUnit{0.f};
	UPROPERTY(EditDefaultsOnly, Category="Runner")
	TObjectPtr<UCurveFloat> AccelerationRate;
	UPROPERTY(EditAnywhere, Category="Runner")
	float MaxSpeed{0.f};

private:
	UPROPERTY(EditDefaultsOnly, Category="Runner|Camera")
	TObjectPtr<class UCameraComponent> ViewCamera;
	UPROPERTY(EditDefaultsOnly, Category="Runner|Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	// ============================
	// ARunner - Inputs
	// ============================
public:
	void AddCharacterMappingContext(const APlayerController* PlayerController) const;
	float GetCurrentAccelerationRate() const;

private:
	void LeftButtonPress();
	void RightButtonPress();
	
	UPROPERTY(EditDefaultsOnly, Category="Runner|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Runner|Input")
	TObjectPtr<UInputAction> LeftButtonAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Runner|Input")
	TObjectPtr<UInputAction> RightButtonAction;

	enum class ELastPressedButton 
	{
		LPB_Left,
		LPB_Right,
		LPB_None
	};
	ELastPressedButton LastPressedButton{ELastPressedButton::LPB_None};

	UPROPERTY(VisibleAnywhere, Category="Runner|Input")
	bool bCanRun{false};

	// ============================
	// ARunner - Animation
	// ============================
public:
	UPROPERTY(EditDefaultsOnly, Category="Runner|Animation")
	TObjectPtr<UPaperFlipbook> IdleFlipbook;
	UPROPERTY(EditDefaultsOnly, Category="Runner|Animation")
	TObjectPtr<UPaperFlipbook> RunFlipbook;

	UPROPERTY(EditDefaultsOnly, Category="Runner|Animation")
	TObjectPtr<USoundBase> FootstepSound;

private:
	UPROPERTY(VisibleAnywhere, Category="Runner|Animation")
	bool bFootstepSoundPlayed{false};
};