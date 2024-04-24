// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "VMotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RightHandControllerComp = CreateDefaultSubobject<UVMotionControllerComponent>("RightHandControllerComp");
	RightHandControllerComp->SetupAttachment(GetRootComponent());
	RightHandControllerComp->SetTrackingMotionSource("Right");

	LeftHandControllerComp = CreateDefaultSubobject<UVMotionControllerComponent>("LeftHandControllerComp");
	LeftHandControllerComp->SetupAttachment(GetRootComponent());
	LeftHandControllerComp->SetTrackingMotionSource("Left");

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("RightHandMesh");
	RightHandMesh->SetupAttachment(RightHandControllerComp);

	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LeftHandMesh");
	LeftHandMesh->SetupAttachment(LeftHandControllerComp);

	ViewCam = CreateDefaultSubobject<UCameraComponent>("ViewCam");
	ViewCam->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSubsystem)
		{
			InputSubsystem->RemoveMappingContext(InputMappingContext);
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComp)
	{
		EnhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AVRCharacter::Move);
		EnhancedInputComp->BindAction(TurnInputAction, ETriggerEvent::Triggered, this, &AVRCharacter::Turn);

		EnhancedInputComp->BindAction(TeleportInputAction, ETriggerEvent::Started, this, &AVRCharacter::StartTeleport);
		EnhancedInputComp->BindAction(TeleportInputAction, ETriggerEvent::Ongoing, this, &AVRCharacter::TeleportTargeting);
		EnhancedInputComp->BindAction(TeleportInputAction, ETriggerEvent::Triggered, this, &AVRCharacter::CommitTeleport);
	}
}

void AVRCharacter::Move(const FInputActionValue& InputActionVal)
{	
	FVector2D MoveInput = InputActionVal.Get<FVector2D>();
	MoveInput.Normalize();

	FVector Fwd = ViewCam->GetForwardVector();
	Fwd.Z = 0;
	Fwd.Normalize();

	FVector Right = FVector::CrossProduct(FVector::UpVector, Fwd);
	AddMovementInput(MoveInput.Y * Fwd + MoveInput.X * Right);
}

void AVRCharacter::Turn(const FInputActionValue& InputActionVal)
{
	float TurnAmt = InputActionVal.Get<float>();
	AddControllerYawInput(TurnAmt * TurnSpeed);
}

void AVRCharacter::StartTeleport()
{
	PrintMsgOnScreen("Start");
}

void AVRCharacter::TeleportTargeting()
{
	PrintMsgOnScreen("Targeting");
}

void AVRCharacter::CommitTeleport()
{
	PrintMsgOnScreen("Commit");
}

void AVRCharacter::PrintMsgOnScreen(const FString& Msg)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3,FColor::Red, Msg);
	}
}

