// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "VMotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

}

